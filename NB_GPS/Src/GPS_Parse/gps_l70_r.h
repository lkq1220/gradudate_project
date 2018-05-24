//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : gps_l70_r.h
//  Date :     2018-1-15 22:11
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _GPS_L70_R_H
#define _GPS_L70_R_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include <stdint.h>
  
typedef enum
{
  GPS_TYPE_RMC
}gps_type_e;
  
typedef void (*gps_receive_cb)(char*, uint16_t);

//gps定位信息更新通知回调。主要向APP层回调
typedef void (*gps_notify_cb)(gps_type_e);

//******************************************************************************
// typedef 
//
typedef struct
{
  char nmea_id[6];           //nmea message id
  char utc[11];              //utc时间。hhmmss
  char state[2];             // A=定位，V=未定位
  char latitude_value[10];   //纬度值
  char latitude[2];          // N 或S
  char longitude_value[11];  //经度值。dddmm.mmmm
  char longitude[2];         // W 或 E
  char speed[10];            //Speed over ground in knots
  char cog[7];               //方位角，度
  char date[7];              //日期，DDMMYY
  char magnetic_angle[3];    //磁偏角  一直为空
  char magnetic_direction[2]; //方位   一真为空
  char mode[2];               //模式。A = 自动，D=差分，E=估测，N=无效
}gps_gprmc_t;

typedef gps_gprmc_t* GpsrmcHandle;

typedef struct
{
  double longtitude;  //经度值
  char   longtitude_char;
  double latitude;    //纬度值 
  char   latitude_char;
  
}gps_info_t;

typedef gps_info_t* GpsHandle;

//******************************************************************************
// fn : GPS_Init
//
// brief : 实始化GPS模块
//
// param : cb->更新回调
//
// return : none
extern void GPS_Init(gps_notify_cb cb);

//******************************************************************************
// fn : GPS_Get
//
// brief : 获取GPS定位信息
//
// param : none
//
// return : 存放定位信息地址。无效定位信息将返回null
extern GpsrmcHandle GPS_Get(void);

//******************************************************************************
// fn : GPS_GetInfo
//
// brief : 获取GPS定位信息
//
// param : none
//
// return : 存放定位信息地址。无效定位信息将返回null
extern GpsHandle GPS_GetInfo(void);

void GPS_handle_msg(char*msg, uint16_t len);
#ifdef __cplusplus
}
#endif
#endif   //_GPS_L70_R_H