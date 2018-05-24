package com.lkq.fafu.baidu_map;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BaiduMap.OnMapClickListener;
import com.baidu.mapapi.map.BaiduMap.OnMarkerClickListener;
import com.baidu.mapapi.map.BaiduMapOptions;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.InfoWindow.OnInfoWindowClickListener;
import com.baidu.mapapi.map.InfoWindow;
import com.baidu.mapapi.map.MapPoi;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.Marker;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.map.OverlayOptions;
import com.baidu.mapapi.model.LatLng;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import com.lkq.fafu.baidu_map.DeepAI.MyTSF;
import com.lkq.fafu.baidu_map.Get_GPS.Bean;
import com.lkq.fafu.baidu_map.data_save.DataSave;
import com.lkq.fafu.baidu_map.data_save.ShowData;
import com.lkq.fafu.baidu_map.udp_core.udp_core;
import com.lkq.fafu.baidu_map.until.DBUtils;

import java.lang.ref.WeakReference;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends Activity {
    private LinearLayout mapLayout;
    private MapView mMapView;
    private BaiduMap mBaiduMap;
    boolean isFirstLoc = true;
    private BitmapDescriptor bitmap;
    private InfoWindow mInfoWindow;
    private LocationClient mLocClient;
    private ImageButton btn_Datasearch;
    public static Context context;

    private float[] input_data={0,0,0,0,0};

    private static final String TAG = "lkq";

    public MyLocationListenner myListener = new MyLocationListenner();

    private final MyHandler myHandler = new MyHandler(this);

    private udp_core client = null;

    private static String ZigbeeT1_Data=null,ZigbeeT2_Data=null,TRL_Data=null,Result_shidi="?";
    private String ipname;

    boolean connectFlag = false;
    boolean GPSFlag = false;

    private DBUtils dbUtils;

    private class MyHandler extends Handler {
        private final WeakReference<MainActivity> mActivity;
        public MyHandler(MainActivity activity){
            mActivity = new WeakReference<MainActivity>(activity);
        }
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what){

                case 1:
                    if (msg.obj.toString().equals("[iotxx:ok]")&&connectFlag==false)
                    {
                        Log.i("TAG", "连接成功");
                        Toast.makeText(MainActivity.this, "连接成功", Toast.LENGTH_LONG).show();
                        connectFlag=true;
                        Timer GPStimer = new Timer();
                        GPStimer.schedule(new TimerTask() {
                            @Override
                            public void run() {
                                client.send("GPS");
                                if (GPSFlag==true){
                                    this.cancel();
                                }
                            }
                        },1000,12000);
                    }
                    if (connectFlag==true) {
                        //T:20.8C RH:72.8% L0:L=390.22LU
                        if (msg.obj.toString().substring(0, 2).equals("D1")) {
                            ZigbeeT1_Data="T1"+msg.obj.toString().substring(4,10)+"H1"+msg.obj.toString().substring(12,16);
                            //System.out.println(Float.parseFloat(ZigbeeT1_Data.substring(3, 7)));
                           //System.out.println(Float.parseFloat(ZigbeeT1_Data.substring(11,13))/100);
                        } else if (msg.obj.toString().substring(0, 2).equals("D2")) {
                            ZigbeeT2_Data=" T2"+msg.obj.toString().substring(4,10)+"H2"+msg.obj.toString().substring(12,16);
                        } else if (msg.obj.toString().substring(0, 2).equals("T:")) {
                            TRL_Data=msg.obj.toString().substring(0,17)+msg.obj.toString().substring(20, 29) + "X";
                            //System.out.println(Float.parseFloat(TRL_Data.substring(2, 6)));
                            //System.out.println(Float.parseFloat(TRL_Data.substring(11, 15))/100);
                            //System.out.println(Float.parseFloat(TRL_Data.substring(19, 26))/1500);
                        } else if (msg.obj.toString().substring(0, 1).equals("{")) {
                            Gson gson = new Gson();
                            String response = msg.obj.toString();
                            JsonObject jsonObject = new JsonParser().parse(response).getAsJsonObject();
                            JsonObject xy_data = jsonObject.get("data").getAsJsonObject();
                            Bean.DataBean gps_data = gson.fromJson(xy_data, Bean.DataBean.class);
                            String gps_data_data_x = gps_data.getX();
                            String gps_data_data_y = gps_data.getY();
                            System.out.println(gps_data_data_x);
                            System.out.println(gps_data_data_y);
                            double la = Double.parseDouble(gps_data_data_x);
                            double lg = Double.parseDouble(gps_data_data_y);
                            if (la!=0&&lg!=0){
                                GPSFlag=true;
                            }else {
                                System.out.println("not true location");
                            }
                            getLocationByLL(lg, la);

                        }else if (msg.obj.toString().equals("GPS")){
                            client.send("GPS");
                        }
                    }

                    break;
                case 2:
                    //txt_Recv.setText(udpRcvStrBuf.toString());
                    break;
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        context = this;
        bindReceiver();//注册broadcastReceiver接收器

        mapLayout = (LinearLayout)findViewById(R.id.map);



        Intent GetIpIntent = getIntent();
        Bundle data = GetIpIntent.getExtras();
        ipname = data.getString("ipname");

        try {
            udp_connect(ipname);
            send_reg_packet();
        } catch (Exception e) {
            e.printStackTrace();
        }

        SDKInitializer.initialize(getApplicationContext()); //初始化地图sdk
        initMap();

        DataSaveTimer.schedule(DataSaveTimeTask,60000,120000);  //saveData 60S -60000

        dbUtils = DBUtils.getDbUtils(this);

        setListener();



    }


    private View.OnClickListener listener = new View.OnClickListener(){
        @Override
        public void onClick(View view) {
            Intent DataIntent = new Intent(MainActivity.this, ShowData.class);
            startActivity(DataIntent);
        }
    };

    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.hasExtra("udpRcvMsg"))  {
                Message message = new Message();
                message.obj = intent.getStringExtra("udpRcvMsg");
                message.what = 1;
                Log.i("主界面Broadcast","收到"+message.obj.toString());
                myHandler.sendMessage(message);
            }
        }
    };

    private void bindReceiver(){
        try {
            IntentFilter udpRcvIntentFilter = new IntentFilter("udpRcvMsg");
            registerReceiver(broadcastReceiver, udpRcvIntentFilter);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void udp_connect(String ipname){
        //建立线程池
        ExecutorService exec = Executors.newCachedThreadPool();
        client = new udp_core(ipname);
        exec.execute(client);
    }

    private void send_reg_packet(){
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                //if (edit_Send.getText().toString()!=""){
                client.send("ep=NBCMJ6PZ6PQAEUN5&pw=123456");
                //}

            }
        });
        thread.start();
    }

    Timer DataSaveTimer = new Timer();    //数据记录定时器

    TimerTask DataSaveTimeTask = new TimerTask() {
        @Override
        public void run() {
            //System.out.println("enter ..............................");

            DataSave dataSave = new DataSave();
            SimpleDateFormat df = new SimpleDateFormat("HH:mm:ss");
            dataSave.setTv_1(ZigbeeT1_Data);
            dataSave.setTv_2(ZigbeeT2_Data);
            dataSave.setTv_3(TRL_Data);
            dataSave.setTime(df.format(new Date()));

            dbUtils.create(dataSave);
        }
    };

    private void initMap() {
        BaiduMapOptions options = new BaiduMapOptions();
        options.compassEnabled(false); // 不允许指南针
        options.zoomControlsEnabled(false); // 不显示缩放按钮
        mMapView = new MapView(this, options); // 创建一个map对象
        mapLayout.addView(mMapView); // 把map添加到界面上
        mBaiduMap = mMapView.getMap(); // 获取BaiduMap对象
        mMapView.removeViewAt(1); // 去掉百度logo
        mBaiduMap.setMyLocationEnabled(true); //显示我的位置
        mBaiduMap.setMaxAndMinZoomLevel(18,15); //地图的最大最小缩放比例3-18
        mLocClient = new LocationClient(this); //地图在tabhost中，请传入getApplicationContext()
        mLocClient.registerLocationListener(myListener); //绑定定位监听
        LocationClientOption option = new LocationClientOption(); //配置参数
        option.setOpenGps(true);
        option.setAddrType("all"); //设置使其可以获取具体的位置，把精度纬度换成具体地址
        option.setCoorType("bd09ll");
        option.setScanSpan(1000);
        mLocClient.setLocOption(option);
        mLocClient.start();  //开始定位

    }

    public class MyLocationListenner implements BDLocationListener {  //定位

        @Override
        public void onReceiveLocation(BDLocation location) {
            Log.d("hck", "定位定位");
            if (location == null || mMapView == null)
                return;
            MyLocationData locData = new MyLocationData.Builder()
                    .accuracy(location.getRadius()).direction(100)
                    .latitude(location.getLatitude())
                    .longitude(location.getLongitude()).build();
            mBaiduMap.setMyLocationData(locData);
            if (isFirstLoc) {
                Log.d("hck", "定位定位成功");
                isFirstLoc = false;
                LatLng ll = new LatLng(location.getLatitude(),
                        location.getLongitude());
                MapStatusUpdate u = MapStatusUpdateFactory.newLatLng(ll);
                mBaiduMap.animateMapStatus(u);
                //mark(ll.latitude, ll.longitude, location.getAddrStr());
            }

        }

        public void onReceivePoi(BDLocation poiLocation) {

        }
    }

    private void setListener() {
        mBaiduMap.setOnMapClickListener(new OnMapClickListener() {  //点击地图事件
            @Override
            public boolean onMapPoiClick(MapPoi arg0) {
                return false;
            }

            @Override
            public void onMapClick(LatLng arg0) {
                mBaiduMap.hideInfoWindow();//影藏气泡
            }
        });
        mBaiduMap.setOnMarkerClickListener(new OnMarkerClickListener() { //点击覆盖物事件

            @Override
            public boolean onMarkerClick(Marker arg0) {
                showLocation(arg0);
                return false;
            }
        });
    }

    private void showLocation(final Marker marker) {  //显示气泡
        // 创建InfoWindow展示的view

        LatLng pt = null;
        double latitude, longitude;
        latitude = marker.getPosition().latitude;
        longitude = marker.getPosition().longitude;

        View view = LayoutInflater.from(this).inflate(R.layout.map_item, null); //自定义气泡形状
        TextView textView = (TextView) view.findViewById(R.id.my_postion);
        btn_Datasearch = (ImageButton)view.findViewById(R.id.search_info);
        btn_Datasearch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View arg0) {
                Intent DataIntent = new Intent(MainActivity.this, ShowData.class);
                startActivity(DataIntent);
            }
        });

        view.setDrawingCacheEnabled(true);
        pt = new LatLng(latitude + 0.0004, longitude + 0.00005);
        try {
            Result_shidi=AI_Result();
        } catch (Exception e) {
            e.printStackTrace();
        }
        textView.setText(ZigbeeT1_Data+ZigbeeT2_Data+"\n"+TRL_Data+" Res:"+Result_shidi);

        // 定义用于显示该InfoWindow的坐标点
        // 创建InfoWindow的点击事件监听者
        OnInfoWindowClickListener listener = new OnInfoWindowClickListener() {
            public void onInfoWindowClick() {
                mBaiduMap.hideInfoWindow();//影藏气泡
            }
        };
        // 创建InfoWindow
        mInfoWindow = new InfoWindow(view, pt,1);
        mBaiduMap.showInfoWindow(mInfoWindow); //显示气泡

    }

    private void mark(double latitude, double longitude, String title) {//显示覆盖物

        // 定义Maker坐标点
        LatLng point = new LatLng(latitude, longitude);
        // 构建Marker图标

        bitmap = BitmapDescriptorFactory.fromResource(R.drawable.pointe_map);

        // 构建MarkerOption，用于在地图上添加Marker
        OverlayOptions option = new MarkerOptions().position(point)
                .icon(bitmap);
        // 在地图上添加Marker，并显示
        Marker marker = (Marker) mBaiduMap.addOverlay(option);
        marker.setTitle(title);
        Bundle bundle = new Bundle();

        bundle.putSerializable("recore", "ddd");
        marker.setExtraInfo(bundle);
    }

    /*
     *根据经纬度前往
     */
    public void getLocationByLL(double la, double lg)
    {
        mBaiduMap.setMyLocationEnabled(true); //不显示我的位置，样覆盖物代替
        //地理坐标的数据结构
        LatLng latLng = new LatLng(la, lg);
        //描述地图状态将要发生的变化,通过当前经纬度来使地图显示到该位置
        MapStatusUpdate msu = MapStatusUpdateFactory.newLatLng(latLng);
        mBaiduMap.setMapStatus(msu);
        mark(la, lg, "湿地位置");
    }

    public String AI_Result()
    {
        float temp=0.0f;
        int   index=0;
        String AI_Result_S="?";

        input_data[0]=Float.parseFloat(ZigbeeT1_Data.substring(3, 7))/100;
        input_data[2]=Float.parseFloat(TRL_Data.substring(2, 6))/100;
        input_data[3]=(Float.parseFloat(TRL_Data.substring(11, 15))/100);



        if ((ZigbeeT1_Data.substring(11,12).equals('0'))){
            input_data[1]=(Float.parseFloat(ZigbeeT1_Data.substring(12,13))/100);
        }else {
            input_data[1]=(Float.parseFloat(ZigbeeT1_Data.substring(11,13))/100);
        }

        if (TRL_Data.substring(25,26).equals("L")){
            input_data[4]=(Float.parseFloat(TRL_Data.substring(19, 25))/1500);
        }else {
            input_data[4]=(Float.parseFloat(TRL_Data.substring(19, 26))/1500);
        }


        for (index=0;index<5;index++){
            Log.i(TAG, String.valueOf(input_data[index]));
        }

        DecimalFormat df = new DecimalFormat("0.00%");
        MyTSF mytsf=new MyTSF(getAssets(),input_data);
        float[] result=mytsf.getAddResult();

        for (int i=0;i<result.length;i++){
            Log.i(TAG, "click01: "+result[i] );
            if (0.5f<=result[i]&&result[i]<=1.0f)
            {
                temp=result[i];
                index=i;
            }
        }
        if (index==0){
            AI_Result_S="Good";
            System.out.println(df.format(temp));
        }else if (index==1){
            AI_Result_S="Bad";
            System.out.println(df.format(temp));
        }else if (index==2){
            AI_Result_S="Normal";
            System.out.println(df.format(temp));
        }

        return AI_Result_S;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //在activity执行onDestroy时执行mMapView.onDestroy()，实现地图生命周期管理
        mMapView.onDestroy();
    }
    @Override
    protected void onResume() {
        super.onResume();
        //在activity执行onResume时执行mMapView. onResume ()，实现地图生命周期管理
        mMapView.onResume();
    }
    @Override
    protected void onPause() {
        super.onPause();
        //在activity执行onPause时执行mMapView. onPause ()，实现地图生命周期管理
        mMapView.onPause();
    }

}
