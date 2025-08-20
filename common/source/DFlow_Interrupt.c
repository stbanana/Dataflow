/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Interrupt.c
 * Author        : yono
 * Date          : 2024-12-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    自行插入中断处理的函数
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
 * @brief 在 IDLE(空闲中断) 和 RC(接收通道结束) 的中断分支调用此函数
 * @param df Dataflow对象指针
 * @param Size 可用的读数据字节数
 */
void DFlow_Interrupt_IDLE_RC_FTF(_DFlow *df, uint16_t Size)
{
    df->RxExist.Len = Size;
    while(df->RxExist.Len > 0)
    {
        df->RxExist.UserRxBuffer[df->RxExist.UserRxWriteIndex] = df->RxExist.Buffer[Size - df->RxExist.Len];
        df->RxExist.UserRxWriteIndex                           = (df->RxExist.UserRxWriteIndex + 1) % df->RxExist.LenMAX;
        df->RxExist.Len--;
    }
    if(df->Func->Receive((volatile void *)df->RxExist.Buffer, df->RxExist.LenMAX) == DFLOW_PORT_RETURN_DEFAULT)
    {
        DFlowStateSwitch(df, 3);
    }
}

/**
 * @brief 在 TC(传输完成) 的中断分支调用此函数
 * @param df Dataflow对象指针
 * @param interrupt 调用此函数的分支
 */
void DFlow_Interrupt_TC(_DFlow *df)
{
    if(df->Func->SendOver != NULL)
    { /* 延迟触发结束回调 */
        df->State = 1;
    }
}