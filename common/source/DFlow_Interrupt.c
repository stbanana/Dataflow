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
#include <DFlow_api.h>
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 在不同的中断分支调用此函数
 * @param df Dataflow对象指针
 * @param interrupt 调用此函数的分支
 */
void DFlow_Interrupt(_DFlow *df, _DFLOW_INT interrupt)
{
    switch(interrupt)
    {
    case DFLOW_IDLE: // 空闲，搬运Rxbuffer数据，重开接收通道
        uint16_t LenReg = df->RxExist.Len;
        while(df->RxExist.Len > 0)
        {
            df->RxExist.UserRxBuffer[df->RxExist.UserRxReadIndex] = df->RxExist.Buffer[LenReg - df->RxExist.Len];
            df->RxExist.UserRxReadIndex                           = (df->RxExist.UserRxReadIndex + 1) % df->RxExist.LenMAX;
            df->RxExist.Len--;
        }
        if(df->Func->Receive(df->RxExist.Buffer, df->RxExist.LenMAX) == DFLOW_PORT_RETURN_DEFAULT)
        {
            DFlowStateSwitch(df, 3);
        }
        break;
    case DFLOW_TC: // 发送完成，触发回调，流转发送结束
        if(df->Func->SendOver != NULL)
        { /* 延迟触发结束回调 */
            df->State = 1;
        }
        break;
    default:
        break;
    }
}