/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Write.c
 * Author        : yono
 * Date          : 2024-12-24
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <DFlow.h>
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 发送API
 * @param df Dataflow对象指针
 * @param pcBuf 期望发送的数据buffer头
 * @param ui32Len 期望发送的数据长度
 * @return 标准返回
 */
uint32_t DFlow_Write(_DFlow *df, uint8_t *pcBuf, uint32_t ui32Len)
{
    /* 依据发送ABbuffer，注册变量 */
    uint8_t  *pSBuffer    = (uint8_t *)df->TxExist.BufferA;
    uint16_t *pSBufferLen = &df->TxExist.LenA;
    if(df->SendAB & 0x1)
    {
        pSBuffer    = (uint8_t *)df->TxExist.BufferA;
        pSBufferLen = &df->TxExist.LenA;
    }
    else if(df->SendAB & 0x2)
    {
        pSBuffer    = (uint8_t *)df->TxExist.BufferB;
        pSBufferLen = &df->TxExist.LenB;
    }

    /* 审查buffer剩余 */
    if(df->TxExist.LenMAX < (*pSBufferLen + ui32Len))
        return DFLOW_API_RETURN_BUFFER_FULL;

    for(uint32_t i = 0; i < ui32Len; i++)
    {
        pSBuffer[*pSBufferLen + i] = pcBuf[i];
    }
    *pSBufferLen += ui32Len;

    return DFLOW_API_RETURN_DEFAULT;
}