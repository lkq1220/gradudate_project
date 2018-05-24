package com.lkq.fafu.baidu_map.data_save;

import android.os.Parcel;
import android.os.Parcelable;

import com.j256.ormlite.field.DatabaseField;


public class DataSave implements Parcelable {

	@DatabaseField(generatedId = true)
	private int id;
	@DatabaseField
	private String time;
	@DatabaseField
	private String tv_1;
	@DatabaseField
	private String tv_2;
	@DatabaseField
	private String tv_3;

	public DataSave() {
	}

	public DataSave(Parcel source) {
		readFromParcel(source);
	}



	private void readFromParcel(Parcel source) {
		time = source.readString();
		tv_1 = source.readString();
		tv_2 = source.readString();
		tv_3 = source.readString();
	}

	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(id);
		dest.writeString(time);
		dest.writeString(tv_1);
		dest.writeString(tv_2);
		dest.writeString(tv_3);
	}

	public static final Creator<DataSave> CREATOR = new Creator<DataSave>(){

		@Override
		public DataSave createFromParcel(Parcel source) {
			return new DataSave(source);
		}

		@Override
		public DataSave[] newArray(int size) {
			return new DataSave[size];
		}

	};

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getTime() {
		return time;
	}

	public void setTime(String time) {
		this.time = time;
	}

	public String getTv_1() {
		return tv_1;
	}

	public void setTv_1(String tv_1) {
		this.tv_1 = tv_1;
	}

	public String getTv_2() {
		return tv_2;
	}

	public void setTv_2(String tv_2) {
		this.tv_2 = tv_2;
	}
	public String getTv_3() {
		return tv_3;
	}

	public void setTv_3(String tv_3) {
		this.tv_3 = tv_3;
	}
}
