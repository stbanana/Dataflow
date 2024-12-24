/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : main.c
 * Author        : yono
 * Date          : 2024-12-21
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    采用 UART4 外设的 DMA方式 作为示例
    引脚分配见 stm32H743/CPU/H743/Init.c 的 HAL_UART_MspInit 函数
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "DFlow_api.h"
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* hal外设对象 */
extern UART_HandleTypeDef huart4;

/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* 先期芯片配置支持 */
extern void InitCpu(void);
extern void DmaInit(void);
extern void UartInit(void);

/* 库使用示例 */
void DataflowTest(void);

/* 库port示例 */
uint32_t Transmit485(void *Data, size_t Len);

/* 中断函数编写示例 */
/* Private functions ---------------------------------------------------------*/

/* Dataflow对象*/
_DFlow                 DFlow;
_DFLOW_COMMON_FUNCTION DFlowFunc = {.Transmit = Transmit485};

/**/
int main(int argc, char const *argv[])
{
    /* 先期芯片配置支持 */
    InitCpu( );
    DmaInit( );
    UartInit( );

    /* 库使用示例 */
    DataflowTest( );

    return 0;
}

/**
 * @brief 一个简单的示例，展示了在Dataflow框架下收发数据
 * @param  
 */
void DataflowTest(void)
{
}

/**** 一些port函数的编写示例 ****/

uint32_t Transmit485(void *Data, size_t Len)
{
}

uint32_t Receive485(void *Data, size_t Len)
{
}

uint32_t TransmitGetState485(void)
{
}

void SendBefor485(void)
{
}

void SendOver485(void)
{
}