/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file DFlow_utilty.h
 *     DFlow Component
 *     Utility
 *  一些代码工具, 均为库内调用
 */

#ifndef _DFLOW_UTILITY_H_
#define _DFLOW_UTILITY_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/**
 * @brief 改变状态机
 * @param df Dataflow对象指针
 * @param state 目标状态
 */
#define DFlowStateSwitch(df, state) \
    do                              \
    {                               \
        df->State = state;          \
    }                               \
    while(0);

/* 环形buffer判剩余长度 */
#define RBufferLenFree(pRbuffer) ((pRbuffer.UserTxWriteIndex >= pRbuffer.UserTxReadIndex) ? (pRbuffer.LenMAX - (pRbuffer.UserTxWriteIndex - pRbuffer.UserTxReadIndex)) : (pRbuffer.UserTxReadIndex - pRbuffer.UserTxWriteIndex));

/* 未使用参数标识 去警告 */
#define DFLOW_UNUSED_PARAM(x) ((void)(x));

/* Exported types ------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _DFLOW_UTILITY_H_ */
