//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_Board_Cfg.c
//  Date     : 2017-12-21 5:45
//  Version  : V0001
// 历史记录  : 1.第一次创建
//
// 说明：
// NB_IOT与平台相关的配置变量定义
//******************************************************************************
#include "usart.h"

#include "timer_user_poll.h"

#include "NB_Board.h"
#include "NB_BC95.h"

extern int NB_MsgreportCb(msg_types_t,int ,char*);
//==============================================================================
//串口接口函数与NB要求的函数接口形不同，在这里进行了接口参数转接
//******************************************************************************
// fn : HAL_UART_NB_Init
//
// brief : 打开NB_95使用串口
//
// param : cb -> 串口数据接收回调函数
//         baud -> 波特率
//
// return : none
//注意，此时波特率没有起作用，
void HAL_UART_NB_Init(bc95_receive_cb cb, uint32_t baud)
{
  HAL_UARTDMA_Init(NB,(void*)cb,baud);
}

//******************************************************************************
// fn : HAL_UART_NB_Write
//
// brief : 数据通过串口发关
//
// param : buf -> 发送数据缓存地址
//         len -> 有效数据长度
//
// return : none
void HAL_UART_NB_Write(uint8_t*buf,uint16_t len)
{
  HAL_UART_Write(NB,buf,len);
}

//******************************************************************************
// fn : HAL_UART_NB_Close
//
// brief : 关闭所有用的串口
//
// param : none
//
// return : none
void HAL_UART_NB_Close(void)
{
  HAL_UART_Close(NB);
}
//==============================================================================
com_fxnTable  com_fxn = 
{
  .openFxn = HAL_UART_NB_Init,
  .sendFxn = HAL_UART_NB_Write,
  .closeFxn = HAL_UART_NB_Close
};

time_fxnTable time_fxn = 
{
  .initTimerFxn = MX_TIM_Set,
  .startTimerFxn = MX_TIM_Start,
  .stopTimerFxn = MX_TIM_Stop
};

hw_object_t  HWAtrrs_object = 
{
  .baudrate = 9600,
  .uart_fxnTable = &com_fxn,
  .timer_fxnTable = &time_fxn
};

NB_Config  nb_config = 
{
  .fxnTablePtr = NULL,
  .object = (void*)&HWAtrrs_object,
  .AppReceCB = NB_MsgreportCb,
  .log = NULL
};
