/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Getc.c
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
 * @brief 接收API,单字节接收，性能较差
 * @param df Dataflow对象指针
 * @param data_rx 接收的字节数据
 * @return 标准返回
 */
uint32_t DFlow_Getc(_DFlow *df, uint8_t *data_rx)
{
    /* 审查buffer是否为空 */
    if(df->RxExist.UserRxReadIndex == df->RxExist.UserRxWriteIndex)
        return DFLOW_API_RETURN_BUFFER_EMPTY;

    *data_rx                    = df->RxExist.UserRxBuffer[df->RxExist.UserRxReadIndex];

    df->RxExist.UserRxReadIndex = (df->RxExist.UserRxReadIndex + 1) % df->RxExist.LenMAX;

    return DFLOW_API_RETURN_DEFAULT;
}