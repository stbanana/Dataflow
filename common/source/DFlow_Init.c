/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Init.c
 * Author        : yono
 * Date          : 2024-12-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    初始化Dataflow对象
*/

/* Includes ------------------------------------------------------------------*/
#include <DFlow.h>
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 初始化Dataflow对象
 * @param df Dataflow对象指针
 * @param AllBuffer 所有可供分配的内存区，用于分配Tx、Rx的Buffer
 *                  注意：内存区大小必须不小于 TBufferLen * 2 + RBufferLen * 2
 * @param TBufferLen 单个TxBuffer的长度
 * @param RBufferLen 单个RxBuffer的长度
 * @param Func 回调函数集合指针
 * @return 标准返回
 */
uint32_t DFlow_Init(_DFlow *df, void *AllBuffer, uint32_t TBufferLen, uint32_t RBufferLen, _DFLOW_COMMON_FUNCTION *Func)
{
    void *dist = AllBuffer;

    if(df == NULL || AllBuffer == NULL || Func == NULL)
    {
        return DFLOW_API_RETURN_ERR_PARAM;
    }
    if(Func->Transmit == NULL || Func->Receive == NULL || Func->TransmitGetState == NULL)
    {
        return DFLOW_API_RETURN_ERR_PARAM;
    }

    df->State     = 0;
    df->SendAB    = 0x1;
    df->IdleTicks = 0;
    df->Func      = Func;

    /* 分段分配内存 */
    df->TxExist.BufferA = dist;
    df->TxExist.LenA    = 0;
    dist += TBufferLen;

    df->TxExist.BufferB = dist;
    df->TxExist.LenB    = 0;
    dist += TBufferLen;

    df->TxExist.LenMAX           = TBufferLen;

    df->RxExist.UserRxBuffer     = dist;
    df->RxExist.UserRxWriteIndex = 0;
    df->RxExist.UserRxReadIndex  = 0;
    dist += RBufferLen;

    df->RxExist.Buffer = dist;
    df->RxExist.Len    = 0;

    df->RxExist.LenMAX = RBufferLen;

    if(df->Func->Receive((volatile void *)df->RxExist.Buffer, df->RxExist.LenMAX) != DFLOW_PORT_RETURN_DEFAULT)
    {
        DFlowStateSwitch(df, 3);
    }

    return DFLOW_API_RETURN_DEFAULT;
}