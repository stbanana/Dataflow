/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Get_Peek.c
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
 * @brief 查询接收buffer是否为空
 * @param df Dataflow对象指针
 * @return 标准返回
 */
uint32_t DFlow_Get_Peek(_DFlow *df)
{
    /* 审查buffer是否为空 */
    if(df->RxExist.UserRxReadIndex == df->RxExist.UserRxWriteIndex)
        return DFLOW_API_RETURN_BUFFER_EMPTY;

    return DFLOW_API_RETURN_DEFAULT;
}