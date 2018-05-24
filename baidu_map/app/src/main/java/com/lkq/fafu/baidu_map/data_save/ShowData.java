package com.lkq.fafu.baidu_map.data_save;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ListView;


import com.lkq.fafu.baidu_map.R;
import com.lkq.fafu.baidu_map.until.DBUtils;

import java.util.List;


/**
 * Created by alienware on 2017/3/29.
 */
public class ShowData extends Activity {

    private DataListViewAdapter listViewAdapter;
    private ListView listView;
    private Button CleanButton = null;


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.showdata_main);
        BindRecvView();
        CleanButton.setOnClickListener(new CleanListListener());


        DBUtils dbUtils = DBUtils.getDbUtils(this);

        List<DataSave> list = dbUtils.getAllData();
        System.err.println("List<Data> list====>" + list);
        listViewAdapter = new DataListViewAdapter(this, R.layout.item, list);
        listView.setAdapter(listViewAdapter);
    }


    public void BindRecvView(){
        listView = (ListView) findViewById(R.id.list_data);
        CleanButton = (Button)findViewById(R.id.cleanData);
    }

    class CleanListListener implements View.OnClickListener{
        @Override
        public void onClick(View v) {
            deleteDatabase("LKQ.db");
            listView.setAdapter(null);
        }
    }


}
