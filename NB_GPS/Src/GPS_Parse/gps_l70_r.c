//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : led_blink.h
//  Date :     2018-1-09 10:30
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include <string.h>
#include <stdlib.h>

#include "gps_l70_r.h"
#include "gpio.h"
#include "usart.h"

#define NMEA_GPGSA  "$GPGSA"
#define NMEA_GPGGA  "$GPGGA"
#define NMEA_GPGSV  "$GPGSV"
#define NMEA_GPRMC  "$GPRMC"
#define NMEA_GPVTG  "$GPVTG"
#define NMEA_GPGLL  "$GPGLL"

static gps_notify_cb gps_app_cb;
static gps_gprmc_t   gps_gprmc;
static gps_info_t    gps_info;
//******************************************************************************
// 内部函数声明区
//
//static void GPS_handle_msg(char*msg, uint16_t len);

//解析GPRMC 数据格式
static void GPS_parse_rmc(char *msg,uint16_t len);
//提取单个数据域
static uint16_t GPS_ExtractField(char *dst,char*src,uint16_t offset,uint8_t max_len);
//将转换相应的定位数据，存放到gps_info中
static uint8_t GPS_Convert(void);
//******************************************************************************
// fn : GPS_Init
//
// brief : 实始化GPS模块
//
// param : none
//
// return : none
void GPS_Init(gps_notify_cb cb)
{
  gps_app_cb = cb;
  //复位缓存
  memset(&gps_gprmc,0,sizeof(gps_gprmc_t));
  HAL_UARTDMA_Init(GPS,(void*)GPS_handle_msg,0);
  //打开gps电源
  Bsp_gps_en();
}
//******************************************************************************
// fn : GPS_Get
//
// brief : 获取GPS定位信息
//
// param : none
//
// return : 存放定位信息地址。无效定位信息将返回null
GpsrmcHandle GPS_Get(void)
{
  if( gps_gprmc.state[0] == 'A')
  {
    return &gps_gprmc;
  }
  return NULL;
}
//******************************************************************************
// fn : GPS_GetInfo
//
// brief : 获取GPS定位信息
//
// param : none
//
// return : 存放定位信息地址。无效定位信息将返回null
extern GpsHandle GPS_GetInfo(void)
{
  return &gps_info;
}
//******************************************************************************
// fn : GPS_handle_msg
//
// brief : 处理接收GPS输出信息
//
// param : msg -> NMEA 协议数据
//         len -> 数据长度
//
// return : none
void GPS_handle_msg(char*msg, uint16_t len)
{
  char *pRmc = 0;
  char *pEnd = 0;
  pRmc = strstr(msg,NMEA_GPRMC);
  
  if(pRmc)
  {
    //找到GPRMC NMEA协议数据
    pEnd = strchr(pRmc,'\r');
    if(pEnd)
    {
      *pEnd = 0;
      //printf("RMC=%s\r\n",pRmc);
      //printf("len=%d\r\n",(uint16_t)(pEnd - pRmc));
      GPS_parse_rmc(pRmc,(uint16_t)(pEnd - pRmc));
      
      GPS_Convert();
      if(gps_app_cb)
      {
        gps_app_cb(GPS_TYPE_RMC);
      }
    }
    
  }
}
//******************************************************************************
// fn : GPS_parse_rmc
//
// brief : 处理接收GPRMC输出信息
//
// param : msg -> GPRMC协议数据起始地址
//         len -> 数据长度
//
// return : none
void GPS_parse_rmc(char *msg,uint16_t len)
{
  uint16_t offset = 0;
  if(msg == NULL || len < 10)
  {
    return;
  }
  
  offset = GPS_ExtractField(gps_gprmc.nmea_id,msg,offset,sizeof(gps_gprmc.nmea_id)-1);
  //printf("len=%d,P_ID=%s,offset=%d\r\n",len,gps_gprmc.nmea_id,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.utc,msg,offset,sizeof(gps_gprmc.utc)-1);
  }
  //printf("utc=%s,offset=%d\r\n",gps_gprmc.utc,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.state,msg,offset,sizeof(gps_gprmc.state)-1);
  }
  //printf("state=%s,offset=%d\r\n",gps_gprmc.state,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.latitude_value,msg,offset,sizeof(gps_gprmc.latitude_value)-1);
  }
  //printf("latitude=%s,offset=%d\r\n",gps_gprmc.latitude_value,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.latitude,msg,offset,sizeof(gps_gprmc.latitude)-1);
  }
  //printf("latitude=%s,offset=%d\r\n",gps_gprmc.latitude,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.longitude_value,msg,offset,sizeof(gps_gprmc.longitude_value)-1);
  }
  //printf("longitude=%s,offset=%d\r\n",gps_gprmc.longitude,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.longitude,msg,offset,sizeof(gps_gprmc.longitude)-1);
  }
  //printf("longitude=%s,offset=%d\r\n",gps_gprmc.longitude,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.speed,msg,offset,sizeof(gps_gprmc.speed)-1);
  }
  //printf("speed=%s,offset=%d\r\n",gps_gprmc.speed,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.cog,msg,offset,sizeof(gps_gprmc.cog)-1);
  }
  //printf("cog=%s,offset=%d\r\n",gps_gprmc.cog,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.date,msg,offset,sizeof(gps_gprmc.date)-1);
  }
  //printf("date=%s,offset=%d\r\n",gps_gprmc.date,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.magnetic_angle,msg,offset,sizeof(gps_gprmc.magnetic_angle)-1);
  }
  //printf("magnetic_angle=%s,offset=%d\r\n",gps_gprmc.magnetic_angle,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.magnetic_direction,msg,offset,sizeof(gps_gprmc.magnetic_direction)-1);
  }
  //printf("magnetic_direction=%s,offset=%d\r\n",gps_gprmc.magnetic_direction,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.mode,msg,offset,sizeof(gps_gprmc.mode)-1);
  }
  //printf("mode=%s,offset=%d\r\n",gps_gprmc.mode,offset);
}
//******************************************************************************
// fn : GPS_ExtractField
//
// brief : 提取单个数据哉
//
// param : dst -> 目标数据地址
//         src -> 原数据地址
//         offset -> 数据偏移
//         max_len -> 最大提取数据个数
//
// return : none
static uint16_t GPS_ExtractField(char *dst,char*src,uint16_t offset,uint8_t max_len)
{
  uint8_t j = 0;
  if(*(src + offset) == '$' ||
     *(src + offset) == ',')
  {
    offset++;
  }
  
  j = 0;
  while((src[offset] != ',') && (src[offset] != '*') && (j < max_len))
  {
    dst[j] = src[offset];
    j++;
    offset++;
  }
  
  dst[j] = 0;
  
  
  return offset;
}
//******************************************************************************
// fn : GPS_Convert
//
// brief : 将定位信息进行转换
//
// param : none
//
// return : none
uint8_t GPS_Convert(void)
{
  double tmp = 0;
  uint32_t dd = 0;
  if( gps_gprmc.state[0] == 'A')
  {
    //GPRMC的纬度值格式为ddmm.mmmm,要转换成dd.dddddd
    gps_info.latitude_char = gps_gprmc.latitude[0];
    tmp = atof(gps_gprmc.latitude_value); 
    dd = (uint32_t)(tmp / 100);  //取度数整数
    gps_info.latitude = dd + ((tmp - dd * 100)/60);
    
    //GPRMC的经度格式为dddmm.mmmm，要转换成dd.dddddd
    gps_info.longtitude_char = gps_gprmc.longitude[0];
    tmp = atof(gps_gprmc.longitude_value);
    dd = (uint32_t)(tmp / 100);
    gps_info.longtitude = dd + ((tmp - dd * 100)/60);
    return 1;
  }
  else
  {
    gps_info.latitude_char = 'X';
    gps_info.latitude = 0.0;
    
    gps_info.longtitude_char = 'Y';
    gps_info.longtitude = 0.0;
  }
  return 0;
}
