package com.lkq.fafu.baidu_map.User_MainActivity;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;


public class UserDataManager {
	private DatabaseHelper dbHelper;
	public UserDataManager(Context context){
		dbHelper=new DatabaseHelper(context);
	}

	//登录用
	public boolean login(String username,String password){
		SQLiteDatabase sdb=dbHelper.getReadableDatabase();
		String sql="select * from user where username=? and password=?";
		Cursor cursor=sdb.rawQuery(sql, new String[]{username,password});
		if(cursor.moveToFirst()==true){
			cursor.close();
			return true;
		}
		return false;
	}
	//注册用
	public boolean register(UserData user){
		SQLiteDatabase sdb=dbHelper.getReadableDatabase();
		String sql="insert into user(username,password,age,sex) values(?,?,?,?)";
		Object obj[]={user.getUsername(),user.getPassword()};
		sdb.execSQL(sql, obj);
		return true;
	}
}
