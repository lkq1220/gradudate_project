package com.lkq.fafu.baidu_map.until;

import android.content.Context;

import com.j256.ormlite.dao.Dao;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;

import com.lkq.fafu.baidu_map.data_save.DataSave;


public class DBUtils {
	
	public static Dao<DataSave, Integer> dataDao;
	private static DBUtils instance;
	
	 private DBUtils(Context context) {  
	        if (dataDao == null) {  
	            DBHelper dbHelper = DBHelper.getHelper(context);  
	            try {  
	            	dataDao = dbHelper.getDao(DataSave.class);
	            } catch (SQLException e) {  
	                e.printStackTrace();  
	            }  
	        }  
	    } 
	 
	 public static DBUtils getDbUtils(Context context){
		 if (instance==null) {
			instance = new DBUtils(context);
		}
		 return instance;
	 }
	 
	 /** 
	  * 插入数据,如果数据存在则进行更新 
	  *
	  */  	 
	public void create(DataSave dataSave){
		try {
			dataDao.create(dataSave);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	/** 
     * 连续进行插入，如果存在则更新 
     */ 
	public void creates(List<DataSave> lists){
        try {  
            for (DataSave dataSave : lists) {
                dataDao.createOrUpdate(dataSave);
            }  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  		
	}
	
	/**
	 * OrmLite提供的批处理任务方法进行一个连续插入多个数据
	 * @return
	 */
	public void createsUseOrmLite(final List<DataSave> lists){
	      try {  
	            dataDao.callBatchTasks(new Callable<Void>() {  
	  
	                @Override  
	                public Void call() throws Exception {  
	                    for (DataSave dataSave : lists) {
	                        dataDao.createOrUpdate(dataSave);
	                    }  
	                    return null;  
	                }  
	            });  
	        } catch (Exception e) {  
	            e.printStackTrace();  
	        }  	
	}
	
	public List<DataSave> getAllData(){
		List<DataSave> lists = new ArrayList<DataSave>();
		try {
			lists = dataDao.queryForAll();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return lists;
	}

}
