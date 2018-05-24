//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : _dma_cfg.c
//  Date     : 2017-12-18 20:43
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include <string.h>
#include "_dma_cfg.h"

#define HI_UINT16(DATA)       ((DATA)>>8)
#define LO_UINT16(DATA)       ((DATA) & 0xFF)
#define BUILD_UINT16(hi,lo)   (uint16_t)(((hi) & 0x00FF) << 8 + ((lo) & 0x00FF))

#define DMA_NEW_RX_BYTE(TYPE,IDX)  (0x00 == HI_UINT16(dmaCfg[TYPE].buf[(IDX)]))
#define DMA_GET_RX_BYTE(TYPE,IDX)  (LO_UINT16(dmaCfg[TYPE].buf[(IDX)]))
#define DMA_CLR_RX_BYTE(TYPE,IDX)  (dmaCfg[TYPE].buf[(IDX)] = 0xFFFF)

#define DMA_NEW_RX_BYTE_M(IDX)  (0x00 == HI_UINT16(dmaCfg_M.buf[(IDX)]))
#define DMA_GET_RX_BYTE_M(IDX)  (LO_UINT16(dmaCfg_M.buf[(IDX)]))
#define DMA_CLR_RX_BYTE_M(IDX)  (dmaCfg_M.buf[(IDX)] = 0xFFFF)

#define HAL_UART_DMA_IDLE          (5)   //1ms


#define HAL_UART_DMA_FULL          (RECE_BUF_MAX_LEN - 16)

#define TRUE                        1
#define FALSE                       0

static dmaCfg_t               dmaCfg[2];
static sendData_cb            dmaSendCb[2];
static USART_TypeDef*         hDmaUart[2] = {0,0};

static dmaCfg_t     dmaCfg_M;
static sendData_cb_M  dmaSendCb_M;
static USART_TypeDef* hDmaUart_M = 0;
//*****************************************************************************
// fn :    findTail
//
// brief :  找到当前DMA接收缓存区正在操作的位置
//
// param : None.
//
// return : Index of tail of rxBuf.
//*****************************************************************************/
static uint16_t findTail(uint8_t type)
{
  uint16_t idx = dmaCfg[type].rxHead;

  do
  {
    if (!DMA_NEW_RX_BYTE(type,idx))
    {
      break;
    }
    
    if (++idx >= RECE_BUF_MAX_LEN)
    {
      idx = 0;
    }
  } while (idx != dmaCfg[type].rxHead);

  return idx;
}

static uint16_t findTail_M(void)
{
  uint16_t idx = dmaCfg_M.rxHead;

  do
  {
    if (!DMA_NEW_RX_BYTE_M(idx))
    {
      break;
    }
    
    if (++idx >= RECE_BUF_MAX_LEN)
    {
      idx = 0;
    }
  } while (idx != dmaCfg_M.rxHead);

  return idx;
}


//******************************************************************************
// fn : UartDma_Init
//
// brief : 初始化dmaReceCfg的结构
//
// param : none
//
// return : none
uint8_t* UartDma_Init(uint8_t type,sendData_cb sendCb ,USART_TypeDef* hUart)
{
  memset(dmaCfg[type].buf,0xff,RECE_BUF_MAX_LEN<<1);
  dmaCfg[type].rxHead = 0;
  dmaCfg[type].rxTail = 0;
  dmaCfg[type].rxTick = 0;
  dmaCfg[type].rxShdw = 0;
  dmaCfg[type].txSel  = 0;
  
  dmaCfg[type].txIdx[0] = 0;
  dmaCfg[type].txIdx[1] = 0;
  
  dmaCfg[type].rxTick = 0;   //delay 1ms
  
  dmaCfg[type].txDMAPending = FALSE;
  dmaCfg[type].txShdwValid = FALSE;
  dmaSendCb[type] = sendCb;
  hDmaUart[type] = hUart;
  return (uint8_t*)dmaCfg[type].buf;
}

uint8_t* Uart3Dma_Init_M(sendData_cb_M sendCb ,USART_TypeDef* hUart)
{
  memset(dmaCfg_M.buf,0xff,RECE_BUF_MAX_LEN<<1);
  dmaCfg_M.rxHead = 0;
  dmaCfg_M.rxTail = 0;
  dmaCfg_M.rxTick = 0;
  dmaCfg_M.rxShdw = 0;
  dmaCfg_M.txSel  = 0;
  
  dmaCfg_M.txIdx[0] = 0;
  dmaCfg_M.txIdx[1] = 0;
  
  dmaCfg_M.rxTick = 0;   //delay 1ms
  
  dmaCfg_M.txDMAPending = FALSE;
  dmaCfg_M.txShdwValid = FALSE;
  dmaSendCb_M = sendCb;
  hDmaUart_M = hUart;
  return (uint8_t*)dmaCfg_M.buf;
}
//******************************************************************************
// fn :     UartDma_Read
//
// brief :  从接收缓存里读取指定长度串口数据，并释放占用的空间
//
// param :  buf  - 有效长度的目标缓存
//          len  - 要读取的长度
//
// return : length of buffer that was read
//******************************************************************************
uint16_t UartDma_Read(uint8_t type,uint8_t *buf, uint16_t len)
{
  uint16_t cnt;

  for (cnt = 0; cnt < len; cnt++)
  {
    if (!DMA_NEW_RX_BYTE(type,dmaCfg[type].rxHead))
    {
      break;
    }
    *buf++ = DMA_GET_RX_BYTE(type,dmaCfg[type].rxHead);
    
    //释放占用空间
    DMA_CLR_RX_BYTE(type,dmaCfg[type].rxHead);

    if (++(dmaCfg[type].rxHead) >= RECE_BUF_MAX_LEN)
    {
      dmaCfg[type].rxHead = 0;
    }
  }

  return cnt;
}

uint16_t Uart3Dma_Read_M(uint8_t *buf, uint16_t len)
{
  uint16_t cnt;

  for (cnt = 0; cnt < len; cnt++)
  {
    if (!DMA_NEW_RX_BYTE_M(dmaCfg_M.rxHead))
    {
      break;
    }
    *buf++ = DMA_GET_RX_BYTE_M(dmaCfg_M.rxHead);
    
    //释放占用空间
    DMA_CLR_RX_BYTE_M(dmaCfg_M.rxHead);

    if (++(dmaCfg_M.rxHead) >= RECE_BUF_MAX_LEN)
    {
      dmaCfg_M.rxHead = 0;
    }
  }

  return cnt;
}
//******************************************************************************
// fn :     UartDma_Write
//
// brief :  将要发送的数据，拷贝到发送缓存中
//
// param :  buf  - 有效长度的目标缓存
//          len  - 要读取的长度
//
// return : length of buffer that was write
//******************************************************************************
uint16_t UartDma_Write(uint8_t type,uint8_t *buf, uint16_t len)
{
  uint16_t cnt;
  uint8_t txSel;
  uint8_t txIdx;

  // Enforce all or none.
  if ((len + dmaCfg[type].txIdx[dmaCfg[type].txSel]) > SENT_BUF_MAX_LEN)
  {
    return 0;
  }

  txSel = dmaCfg[type].txSel;
  txIdx = dmaCfg[type].txIdx[txSel];

  for (cnt = 0; cnt < len; cnt++)
  {
    dmaCfg[type].txBuf[txSel][txIdx++] = buf[cnt];
  }
  
  if (txSel != dmaCfg[type].txSel)
  {
    txSel = dmaCfg[type].txSel;
    txIdx = dmaCfg[type].txIdx[txSel];

    for (cnt = 0; cnt < len; cnt++)
    {
      dmaCfg[type].txBuf[txSel][txIdx++] = buf[cnt];
    }
  }

  dmaCfg[type].txIdx[txSel] = txIdx;

  if (dmaCfg[type].txIdx[(txSel ^ 1)] == 0)
  {
    // TX DMA is expected to be fired
    dmaCfg[type].txDMAPending = TRUE;
  }

  return cnt;
}

uint16_t Uart3Dma_Write_M(uint8_t *buf, uint16_t len)
{
  uint16_t cnt;
  uint8_t txSel;
  uint8_t txIdx;

  // Enforce all or none.
  if ((len + dmaCfg_M.txIdx[dmaCfg_M.txSel]) > SENT_BUF_MAX_LEN)
  {
    return 0;
  }

  txSel = dmaCfg_M.txSel;
  txIdx = dmaCfg_M.txIdx[txSel];

  for (cnt = 0; cnt < len; cnt++)
  {
    dmaCfg_M.txBuf[txSel][txIdx++] = buf[cnt];
  }
  
  if (txSel != dmaCfg_M.txSel)
  {
    txSel = dmaCfg_M.txSel;
    txIdx = dmaCfg_M.txIdx[txSel];

    for (cnt = 0; cnt < len; cnt++)
    {
      dmaCfg_M.txBuf[txSel][txIdx++] = buf[cnt];
    }
  }

  dmaCfg_M.txIdx[txSel] = txIdx;

  if (dmaCfg_M.txIdx[(txSel ^ 1)] == 0)
  {
    // TX DMA is expected to be fired
    dmaCfg_M.txDMAPending = TRUE;
  }

  return cnt;
}

//**************************************************************************************************
// fn  :  UartDma_Avail
//
// brief :  计算缓存区中数据个数 - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
extern uint16_t UartDma_Avail(uint8_t type)
{
  uint16_t cnt = 0;

  if (DMA_NEW_RX_BYTE(type,dmaCfg[type].rxHead))
  {
    uint16_t idx;

    for (idx = 0; idx < RECE_BUF_MAX_LEN; idx++)
    {
      if (DMA_NEW_RX_BYTE(type,idx))
      {
        cnt++;
      }
    }
  }

  return cnt;
}

extern uint16_t Uart3Dma_Avail_M(void)
{
  uint16_t cnt = 0;

  if (DMA_NEW_RX_BYTE_M(dmaCfg_M.rxHead))
  {
    uint16_t idx;

    for (idx = 0; idx < RECE_BUF_MAX_LEN; idx++)
    {
      if (DMA_NEW_RX_BYTE_M(idx))
      {
        cnt++;
      }
    }
  }

  return cnt;
}
//******************************************************************************
// fn : UartDma_Poll
//
// brief : 轮询接收缓存数据长度
//
// param : none
//
// return : none
//****************************************************************************/
uint8_t UartDma_Poll(void)
{
  uint16_t cnt = 0;
  uint8_t evt = 0;
  for(uint8_t i = 0 ; i < 2 ; i++)
  {
    if(DMA_NEW_RX_BYTE(i,dmaCfg[i].rxHead))
    {
      uint16_t tail = findTail(i);
      
      // If the DMA has transferred in more Rx bytes, reset the Rx idle timer.
      if (dmaCfg[i].rxTail != tail)
      {
        dmaCfg[i].rxTail = tail;

        if (dmaCfg[i].rxTick == 0)
        {
          dmaCfg[i].rxShdw = HAL_GetTick();
        }

        dmaCfg[i].rxTick = HAL_UART_DMA_IDLE;
      }
      else if (dmaCfg[i].rxTick)
      {
        uint32_t Tick = HAL_GetTick();
        uint32_t delta = Tick >= dmaCfg[i].rxShdw ?
                                 (Tick - dmaCfg[i].rxShdw ): 
                                 (Tick + (UINT32_MAX - dmaCfg[i].rxShdw));
        
        if (dmaCfg[i].rxTick > delta)
        {
          dmaCfg[i].rxTick -= delta;
          dmaCfg[i].rxShdw = Tick;
        }
        else
        {
          dmaCfg[i].rxTick = 0;
        }
      }
      cnt = UartDma_Avail(i);
    }
    else
    {
      dmaCfg[i].rxTick = 0;
    }

    if (cnt >= HAL_UART_DMA_FULL)
    {
      evt = HAL_UART_RX_FULL<<(i<<1);
    }
    else if (cnt && !dmaCfg[i].rxTick)
    {
      evt = HAL_UART_RX_TIMEOUT<<(i<<1);
    }
    
    if (dmaCfg[i].txShdwValid)
    {
      uint32_t decr = HAL_GetTick() - dmaCfg[i].txShdw;
    
      if (decr > dmaCfg[i].txTick)
      {
        // No protection for txShdwValid is required
        // because while the shadow was valid, DMA ISR cannot be triggered
        // to cause concurrent access to this variable.
        dmaCfg[i].txShdwValid = FALSE;
      }
    }
    
    if (dmaCfg[i].txDMAPending && !dmaCfg[i].txShdwValid)
    {
      // Clear the DMA pending flag
      dmaCfg[i].txDMAPending = FALSE;
      //Send  data
      if(dmaSendCb[i])
      {
        (dmaSendCb[i])(i,dmaCfg[i].txBuf[dmaCfg[i].txSel],dmaCfg[i].txIdx[dmaCfg[i].txSel]);
      }
      dmaCfg[i].txSel ^= 1;
    } 
  }

  return evt;
}

uint8_t Uart3Dma_Poll_M(void)
{
  uint16_t cnt = 0;
  uint8_t evt = 0;

  if(DMA_NEW_RX_BYTE_M(dmaCfg_M.rxHead))
  {
    uint16_t tail = findTail_M();
    
    // If the DMA has transferred in more Rx bytes, reset the Rx idle timer.
    if (dmaCfg_M.rxTail != tail)
    {
      dmaCfg_M.rxTail = tail;

      if (dmaCfg_M.rxTick == 0)
      {
        dmaCfg_M.rxShdw = HAL_GetTick();
      }
      
      dmaCfg_M.rxTick = HAL_UART_DMA_IDLE;
    }
    else if (dmaCfg_M.rxTick)
    {
      uint32_t Tick = HAL_GetTick();
      uint32_t delta = Tick >= dmaCfg_M.rxShdw ?
                               (Tick - dmaCfg_M.rxShdw ): 
                               (Tick + (UINT32_MAX - dmaCfg_M.rxShdw));
      
      if (dmaCfg_M.rxTick > delta)
      {
        dmaCfg_M.rxTick -= delta;
        dmaCfg_M.rxShdw = Tick;
      }
      else
      {
        dmaCfg_M.rxTick = 0;
      }
    }
    cnt = Uart3Dma_Avail_M();
  }
  else
  {
    dmaCfg_M.rxTick = 0;
  }

  if (cnt >= HAL_UART_DMA_FULL)
  {
    evt = HAL_UART_RX_FULL;
  }
  else if (cnt && !dmaCfg_M.rxTick)
  {
    evt = HAL_UART_RX_TIMEOUT;
  }
  
  if (dmaCfg_M.txShdwValid)
  {
    uint32_t decr = HAL_GetTick() - dmaCfg_M.txShdw;;
	
    if (decr > dmaCfg_M.txTick)
    {
      // No protection for txShdwValid is required
      // because while the shadow was valid, DMA ISR cannot be triggered
      // to cause concurrent access to this variable.
      dmaCfg_M.txShdwValid = FALSE;
    }
  }
  
  if (dmaCfg_M.txDMAPending && !dmaCfg_M.txShdwValid)
  {
    // Clear the DMA pending flag
    dmaCfg_M.txDMAPending = FALSE;
    //Send  data
    if(dmaSendCb_M)
    {
      dmaSendCb_M(dmaCfg_M.txBuf[dmaCfg_M.txSel],dmaCfg_M.txIdx[dmaCfg_M.txSel]);
    }
    dmaCfg_M.txSel ^= 1;
  }

  return evt;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  uint8_t index = 0xff;
  
  if(huart->Instance == hDmaUart[0])
  {
    index= 0;  //表示NB
  }
  else if(huart->Instance == hDmaUart[1])
  {
    index = 1; //表示GPS
  }
  
  if(index != 0xff)
  {
    // Indicate that the other buffer is free now.
    dmaCfg[index].txIdx[(dmaCfg[index].txSel ^ 1)] = 0;
    
    // Set TX shadow
    dmaCfg[index].txShdw = HAL_GetTick();
    dmaCfg[index].txShdwValid = TRUE;

    // If there is more Tx data ready to go, re-start the DMA immediately on it.
    if (dmaCfg[index].txIdx[dmaCfg[index].txSel])
    {
      // UART TX DMA is expected to be fired
      dmaCfg[index].txDMAPending = TRUE;
    }
  }
}

void HAL_UART3_TxCpltCallback_M(UART_HandleTypeDef *huart)
{
  if(huart->Instance == hDmaUart_M)
  {
    // Indicate that the other buffer is free now.
    dmaCfg_M.txIdx[(dmaCfg_M.txSel ^ 1)] = 0;
    
    // Set TX shadow
    dmaCfg_M.txShdw = HAL_GetTick();
    dmaCfg_M.txShdwValid = TRUE;

    // If there is more Tx data ready to go, re-start the DMA immediately on it.
    if (dmaCfg_M.txIdx[dmaCfg_M.txSel])
    {
      // UART TX DMA is expected to be fired
      dmaCfg_M.txDMAPending = TRUE;
    }
  }
}