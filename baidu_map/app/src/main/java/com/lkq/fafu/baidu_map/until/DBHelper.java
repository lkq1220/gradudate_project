package com.lkq.fafu.baidu_map.until;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

import com.j256.ormlite.android.apptools.OrmLiteSqliteOpenHelper;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.TableUtils;

import com.lkq.fafu.baidu_map.data_save.DataSave;


public class DBHelper extends OrmLiteSqliteOpenHelper{
	
    private static final String DATABASE_NAME = "LKQ.db";
    private static final int DATABASE_VERSION = 1;  	
    private static DBHelper instance;
    
	private DBHelper(Context context){  
	        super(context, DATABASE_NAME, null, DATABASE_VERSION);  
	    }  
	 
	  @Override  
	public void onCreate(SQLiteDatabase sqLiteDatabase,  
	            ConnectionSource connectionSource) {  
	        try {  
	            TableUtils.createTable(connectionSource, DataSave.class);
	        } catch (Exception e) {  
	            e.printStackTrace();  
	        }  
	    }  
	  
	    @Override  
	    public void onUpgrade(SQLiteDatabase sqLiteDatabase,  
	            ConnectionSource connectionSource, int oldVer, int newVer) {  
	        try {  
	            TableUtils.dropTable(connectionSource, DataSave.class, true);
	            onCreate(sqLiteDatabase, connectionSource);  
	        } catch (Exception e) {  
	            e.printStackTrace();  
	        }  
	    }  	 
	    
	    public static synchronized DBHelper getHelper(Context context){
	        if (instance == null)  
	        {  
	            synchronized (DBHelper.class)  
	            {  
	                if (instance == null)  
	                    instance = new DBHelper(context);  
	            }  
	        }  
	        return instance;  
	    }
	   
}
