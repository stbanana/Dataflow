/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Writec.c
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
 * @brief 发送API,单字节发送，性能较差
 * @param df Dataflow对象指针
 * @param data 期望发送的字节数据
 * @return 标准返回
 */
uint32_t DFlow_Writec(_DFlow *df, uint8_t data)
{
    /* 依据发送ABbuffer，注册变量 */
    volatile uint8_t  *pSBuffer;
    volatile uint16_t *pSBufferLen;
    if(df->SendAB & 0x1)
    {
        pSBuffer    = df->TxExist.BufferA;
        pSBufferLen = &df->TxExist.LenA;
    }
    else if(df->SendAB & 0x2)
    {
        pSBuffer    = df->TxExist.BufferB;
        pSBufferLen = &df->TxExist.LenB;
    }

    /* 审查buffer剩余 */
    if(df->TxExist.LenMAX < (*pSBufferLen + 1))
        return DFLOW_API_RETURN_BUFFER_FULL;

    pSBuffer[*pSBufferLen] = data;

    *pSBufferLen += 1;

    return DFLOW_API_RETURN_DEFAULT;
}