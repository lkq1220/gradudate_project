package com.lkq.fafu.baidu_map.data_save;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;


import com.lkq.fafu.baidu_map.R;

import java.util.List;


public class DataListViewAdapter extends ArrayAdapter<DataSave>{

	private Context context;
	private int layoutId;

	public DataListViewAdapter(Context context, int layoutId, List<DataSave> list) {
		super(context, layoutId, list);
		this.context = context;
		this.layoutId = layoutId;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		TextView tv_time;
		TextView tv_1;
		TextView tv_2;
		TextView tv_3;
		ViewContainer vc=null;

		if (convertView==null) {
			convertView = LayoutInflater.from(context).inflate(layoutId, null);
			tv_time = (TextView) convertView.findViewById(R.id.tv_time);
			tv_1 = (TextView) convertView.findViewById(R.id.tv_1);
			tv_2 = (TextView) convertView.findViewById(R.id.tv_2);
			tv_3 = (TextView) convertView.findViewById(R.id.tv_3);

			vc = new ViewContainer();
			vc.setTv_time(tv_time);
			vc.setTv_1(tv_1);
			vc.setTv_2(tv_2);
			vc.setTv_3(tv_3);

			convertView.setTag(vc);
		}else {
			vc = (ViewContainer) convertView.getTag();
			tv_time = vc.getTv_time();
			tv_1 = vc.getTv_1();
			tv_2 = vc.getTv_2();
			tv_3 = vc.getTv_3();
		}

		DataSave data = getItem(position);
		tv_time.setText(data.getTime());
		tv_1.setText(data.getTv_1());
		tv_2.setText(data.getTv_2());
		tv_3.setText(data.getTv_3());

		return convertView;
	}


	public class ViewContainer{

		private TextView tv_time;
		private TextView tv_1;
		private TextView tv_2;
		private TextView tv_3;

		public TextView getTv_time() {
			return tv_time;
		}
		public void setTv_time(TextView tv_time) {
			this.tv_time = tv_time;
		}


		public TextView getTv_1() {
			return tv_1;
		}
		public void setTv_1(TextView tv_1) {
			this.tv_1 = tv_1;
		}


		public TextView getTv_2() {
			return tv_2;
		}
		public void setTv_2(TextView tv_2) {
			this.tv_2 = tv_2;
		}


		public TextView getTv_3() {
			return tv_3;
		}
		public void setTv_3(TextView tv_3) {
			this.tv_3 = tv_3;
		}

	}


}
