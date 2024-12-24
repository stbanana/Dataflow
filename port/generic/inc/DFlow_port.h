/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file DFLOW_port.h
 *     DFLOW Component
 *     Port Specific
 * Generic
 */

#ifndef _DFLOW_PORT_H_
#define _DFLOW_PORT_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <assert.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* port函数允许返回的操作状态值 */
#define DFLOW_PORT_RETURN_DEFAULT         0x00 // 默认的无错误
#define DFLOW_PORT_RETURNT_ERR_INDEFINITE 0x01 // 默认的未定义错误(供临时使用或极简函数返回错误)
#define DFLOW_PORT_RETURN_BUFFER_FULL     0x20 // BUFFER满
#define DFLOW_PORT_RETURN_BUFFER_EMPTY    0x21 // BUFFER空
#define DFLOW_PORT_RETURN_CABLE_BUSY      0x41 // 线路繁忙
#define DFLOW_PORT_RETURN_CABLE_LOST      0x41 // 线路掉线

/* Exported types ------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _DFLOW_PORT_H_ */
