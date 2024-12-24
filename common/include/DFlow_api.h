/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file DFlow_api.h
 *     Dataflow Component
 *     Application Interface (API)
 */

#ifndef _DATAFLOW_API_H_
#define _DATAFLOW_API_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <DFlow_port.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* API返回集 */
/* 通用返回 */
#define DFLOW_API_RETURN_DEFAULT        0x00 //默认的无错误
#define DFLOW_API_RETURN_ERR_INDEFINITE 0x01 //默认的未指定错误
#define DFLOW_API_RETURN_ERR_PARAM      0x02 //传递参数错误
/* BUFFER相关返回 */
#define DFLOW_API_RETURN_BUFFER_FULL  0x20 //BUFFER满
#define DFLOW_API_RETURN_BUFFER_EMPTY 0x21 //BUFFER空

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    DFLOW_IDLE, // 在空闲中断 支持DMA接收
    DFLOW_TC,   // 在发送完成中断 支持DMA发送
} _DFLOW_INT;

/**
 * @brief modbus通信发送接收函数的配置
 * 主机、从机的定义共用
 */
typedef struct
{
    uint32_t (*Transmit)(void *Data, size_t Len); // 数据开启发送通道函数绑定
    uint32_t (*Receive)(void *Data, size_t Len);  // 数据开启接收通道函数绑定
    uint32_t (*TransmitGetState)(void);           // 获得发送通道状态函数绑定
    /* 额外的回调函数处理，通常用于485切换模式，或者IC的en口控制
        这部分绑定为 NULL 将裁剪回调部分处理 */
    void (*SendBefor)(void); // 开始发送之前的回调函数指针（主要用于RS485切换到发送模式）
    void (*SendOver)(void);  // 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式）
} _DFLOW_COMMON_FUNCTION;

/**
 * @brief 发送buffer的定义
 * 线性 AB buffer 采用指针便于动态申请内存
 */
typedef struct
{
    volatile uint8_t *BufferA; // 数据buffer绑定
    uint16_t          LenA;    // 数据长度变量
    volatile uint8_t *BufferB; // 数据buffer绑定
    uint16_t          LenB;    // 数据长度变量
    uint16_t          LenMAX;  // 数据buffer的最大长度
} _DFLOW_TX_EXIST;

/**
 * @brief 接收buffer的定义
 * 线性 AB buffer 采用指针便于动态申请内存
 */
typedef struct
{
    //  接收用户 BUFFER
    volatile uint8_t *UserRxBuffer;
    volatile uint16_t UserRxWriteIndex; //入buffer指针
    volatile uint16_t UserRxReadIndex;  //出buffer指针
    //  中断 BUFFER
    volatile uint8_t *Buffer; // 数据buffer绑定
    volatile uint16_t Len;    // 数据长度变量
    uint16_t          LenMAX; // 数据buffer的最大长度
} _DFLOW_RX_EXIST;

/**
 * @brief 定义Dataflow应用对象 
 */
typedef struct _DFlow
{
    /* 初始化时需赋固定值的部分 */
    uint8_t State;  // 0:空闲态.触发回调，流转发送 1:发送结束，触发回调流转空闲 2:发送，发数据，中断流转发送结束 3:接收，开启接收通道失败会在此重开
    uint8_t SendAB; // 0x1:使用Abuffer发送 0x2:使用Bbuffer发送
    /* 需传入初始化函数进行配置的部分 */
    _DFLOW_TX_EXIST         TxExist; // 供发送的buffer空间
    _DFLOW_RX_EXIST         RxExist; // 供接收的buffer空间
    _DFLOW_COMMON_FUNCTION *Func;    // 函数绑定
} _DFlow;
/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/***** 用户实用API *****/
extern void DFlow_Ticks(_DFlow *df);
extern void DFlow_Init(_DFlow *df, void *AllBuffer, uint32_t TBufferLen, uint32_t RBufferLen, _DFLOW_COMMON_FUNCTION *Func);

/**/
extern uint32_t DFlow_Write(_DFlow *df, uint8_t *pcBuf, uint32_t ui32Len);
extern uint32_t DFlow_Writec(_DFlow *df, uint8_t data);
extern uint32_t DFlow_Getc(_DFlow *df, uint8_t *data_rx);

/***** 自行插入中断处理 *****/
extern void DFlow_Interrupt(_DFlow *df, _DFLOW_INT interrupt);

/* Include DFlow utility and system file.  */
#include "DFlow_utility.h"

#ifdef __cplusplus
}
#endif
#endif /* _DATAFLOW_API_H_ */
