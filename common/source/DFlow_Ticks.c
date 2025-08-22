/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlow_Ticks.c
 * Author        : yono
 * Date          : 2024-12-23
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    Dataflow对象的周期驱动
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
 * @brief 需要1ms运行一次
 * @param df Dataflow对象指针
 */
void DFlow_Ticks(_DFlow *df)
{
    /* 依据发送ABbuffer，注册变量 */
    volatile uint8_t *pSBuffer    = df->TxExist.BufferA;
    uint16_t         *pSBufferLen = &df->TxExist.LenA;
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

    if(df->Func->SendBefor != NULL && df->Func->SendOver != NULL)
    { /* 具有切换回调，触发切换状态机 */
        switch(df->State)
        {
        case 0:
            if(*pSBufferLen > 0 &&                                         // 待发送有数据
               df->Func->TransmitGetState( ) == DFLOW_PORT_RETURN_DEFAULT) // 通道可用
            {
                df->Func->SendBefor( );
                DFlowStateSwitch(df, 2);
            }
            return;
        case 1:
            df->Func->SendOver( );
            DFlowStateSwitch(df, 0);
            return;
        case 2:
            break;
        case 3:
            df->Func->Receive((volatile void *)df->RxExist.Buffer, df->RxExist.LenMAX);
            DFlowStateSwitch(df, 0);
            return;
        default:
            break;
        }
        /* 保险机制，避免总线抢占导致卡在发送态 */
        if(*pSBufferLen == 0 // 无发送数据且空闲
           && df->Func->TransmitGetState( ) == DFLOW_PORT_RETURN_DEFAULT)
        {
            if(df->IdleTicks++ > 20)
            {
                df->Func->SendOver( );
                df->IdleTicks = 0;
            }
        }
        else
        {
            df->IdleTicks = 0;
        }
    }

    if(*pSBufferLen > 0 &&                                         // 待发送有数据
       df->Func->TransmitGetState( ) == DFLOW_PORT_RETURN_DEFAULT) // 通道可用
    {
        if(df->Func->Transmit(pSBuffer, *pSBufferLen) == DFLOW_PORT_RETURN_DEFAULT)
        {
            df->SendAB ^= 0x03; // 切换AB指示器
            *pSBufferLen = 0;
            DFlowStateSwitch(df, 0);
        }
    }
}