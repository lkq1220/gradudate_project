//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : json_format.c
//  Date :     2018-03-01 22:03
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include <stdio.h>
#include "json_format.h"

//******************************************************************************
// fn : JSON_Temp
//
// brief : 将具本的温度值转换成相应的json格式
//
// param : buf ->存放json格式数据地址
//         tempValue -> 具体的温度值
//
// return : 转换之后json数据长度
//
// 注意，buf 的长度一定要足够长.Temp Json长度至少要35长度
// 转换之后如下形式
//{
//	"dataType": "TEMP",
//	"data": "32.5"
//}
uint16_t JSON_Temp(char* buf,float tempValue)
{
	uint16_t msgLen = 0;
	if(buf == NULL)
	{
		return 0;
	}
	msgLen = sprintf(buf,"{\"dataType\":\"TEMP\",\"data\":\"%0.1f\"}",tempValue);
	return msgLen;
}
//******************************************************************************
// fn : JSON_Humi
//
// brief : 将具本的湿度值转换成相应的json格式
//
// param : buf ->存放json格式数据地址
//         humiValue -> 具体的湿度值
//
// return : 转换之后json数据长度
//
// 注意，buf 的长度一定要足够长.Temp Json长度至少要35长度
// 转换之后如下形式
//{
//	"dataType": "HUM",
//	"data": "60"
//}
uint16_t JSON_Humi(char* buf,float humiValue)
{
  uint16_t msgLen = 0;
	if(buf == NULL)
	{
		return 0;
	}
	msgLen = sprintf(buf,"{\"dataType\":\"HUM\",\"data\":\"%.1f\"}",humiValue);
	return msgLen;
}
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
// 转换之后如下形式
//{
//	"dataType": "GPS",
//	"data": {
//		"x": "120.28749",   ->经度值 
//		"y": "31.564869"    ->纬度值
//	}
//}
uint16_t JSON_gps(char* buf,double longitude,double latitude)
{
  uint16_t msgLen = 0;
	if(buf == NULL)
	{
		return 0;
	}
	msgLen = sprintf(buf,"{\"dataType\":\"GPS\",\"data\":{\"x\":\"%0.6f\",\"y\":\"%0.6f\"}}",longitude,latitude);
	return msgLen;  
}