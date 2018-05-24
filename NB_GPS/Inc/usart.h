/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */
#include "NB_BC95.h"
#include "gps_l70_r.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


/* USER CODE BEGIN Private defines */
typedef enum 
{
  NB,
  GPS
}UART_TYPE;
/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);

void MX_LPUART1_UART_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void HAL_UART3DMA_Init_M(void);
void HAL_UART3_RegisterCb_M(void* cb);
/* USER CODE BEGIN Prototypes */

void HAL_UARTDMA_Init( UART_TYPE type,void* cb,uint32_t baud);

//******************************************************************************
// fn : USART1_UART_Write
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
void HAL_UART_Write(UART_TYPE type,uint8_t*buf,uint16_t len); 
void HAL_UART3_Write_M(uint8_t*buf,uint16_t len);
//******************************************************************************
// fn : USART1_UART_Read
//
// brief : 通过串口读取数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
uint16_t HAL_UART_Read(UART_TYPE type,uint8_t*buf,uint16_t len);
uint16_t HAL_UART3_Read_M(uint8_t*buf,uint16_t len);
//******************************************************************************
// fn : USART1_UART_Poll
//
// brief : 轮询串口事件
//
// param : none
//
// return : 事件。见dma_rece_cfg.h中定义
uint8_t HAL_UART_Poll(void);
uint8_t HAL_UART3_Poll_M(void);
//******************************************************************************
// fn : USART1_UART_RxBufLen
//
// brief : 返回当前接收缓存区中有效的数据长度
//
// param : none
//
// return : none
uint16_t HAL_UART_RxBufLen(UART_TYPE type);
uint16_t HAL_UART3_RxBufLen_M(void);
//******************************************************************************
// fn : HAL_UART_Close
//
// brief : 关闭使用
//
// param : none
//
// return : none
void HAL_UART_Close(UART_TYPE type);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
