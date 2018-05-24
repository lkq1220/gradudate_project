//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : json_format.h
//  Date :     2018-03-01 21:51
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _JSON_FORMAT_H
#define _JSON_FORMAT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

//******************************************************************************
// fn : JSON_Temp
//
// brief : 将具本的温度值转换成相应的json格式
//
// param : buf ->存放json格式数据地址
//         tempValue -> 具体的温度值
//
// return : 转换之后json数据长度
uint16_t JSON_Temp(char* buf,float tempValue);  
 
//******************************************************************************
// fn : JSON_Humi
//
// brief : 将具本的湿度值转换成相应的json格式
//
// param : buf ->存放json格式数据地址
//         humiValue -> 具体的湿度值
//
// return : 转换之后json数据长度
uint16_t JSON_Humi(char* buf,float humiValue); 

//******************************************************************************
// fn : JSON_gps
//
// brief : 将经纬度信息转换成相应的json格式
//
// param : buf ->存放json格式数据地址
//         longitude -> 经度
//         latiude -> 纬度
//
// return : 转换之后json数据长度
uint16_t JSON_gps(char* buf,double longitude,double latitude); 
  
#ifdef __cplusplus
}
#endif
#endif   //_KEY_HANDLE_H