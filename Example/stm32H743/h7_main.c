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
/* Private macros ------------------------------------------------------------*/
// 建议4字节对齐，否则DMA可能存在问题
#define T_LEN_MAX 256
#define R_LEN_MAX 256
/* Private variables ---------------------------------------------------------*/

/* hal外设对象 */
extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef  hdma_uart4_tx;
extern DMA_HandleTypeDef  hdma_uart4_rx;

/* 预分配内存区 */
uint8_t MEM[T_LEN_MAX * 2 + R_LEN_MAX * 2] __ALIGNED(32);
/* Private Constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* 先期芯片配置支持 */
extern void InitCpu(void);
extern void DmaInit(void);
extern void UartInit(void);
extern void GpioInit(void);

/* 库使用示例 */
void DataflowTest(void);

/* 库port示例 */
uint32_t Transmit485(volatile void *Data, size_t Len);
uint32_t Receive485(volatile void *Data, size_t Len);
uint32_t TransmitGetState485(void);
void     SendBefor485(void);
void     SendOver485(void);

/* 中断函数编写示例 */
/* Private functions ---------------------------------------------------------*/

/* Dataflow对象*/
_DFlow                 DFlow;
_DFLOW_COMMON_FUNCTION DFlowFunc = {.Transmit         = Transmit485, //
                                    .Receive          = Receive485,
                                    .TransmitGetState = TransmitGetState485,
                                    .SendBefor        = SendBefor485,
                                    .SendOver         = SendOver485};

/**/
int main(int argc, char const *argv[])
{
    /* 先期芯片配置支持 */
    InitCpu( );
    DmaInit( );
    UartInit( );
    GpioInit( );

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
    uint16_t FN = 0; // 分频计数
    uint8_t  data;   // 取数据

    DFlow_Init(&DFlow, MEM, T_LEN_MAX, R_LEN_MAX, &DFlowFunc);

    while(1)
    {
        DFlow_Ticks(&DFlow);
        switch(++FN)
        {
        case 1000:
            DFlow_Write(&DFlow, "Hello1\n", sizeof("Hello1\n"));
            break;
        case 2000:
            DFlow_Write(&DFlow, "Hello2\n", sizeof("Hello2\n"));
            break;
        case 3000:
            DFlow_Write(&DFlow, "Hello3\n", sizeof("Hello3\n"));
            break;
        case 4000:
            FN = 0;
            DFlow_Write(&DFlow, "Hello1\n", sizeof("Hello1\n"));
            DFlow_Write(&DFlow, "Hello2\n", sizeof("Hello2\n"));
            DFlow_Write(&DFlow, "Hello3\n", sizeof("Hello3\n"));
            break;
        default:
            break;
        }
        while(DFlow_Getc(&DFlow, &data) == DFLOW_API_RETURN_DEFAULT)
        {
            DFlow_Writec(&DFlow, data);
        }
        HAL_Delay(0);
    }
}

/**** 一些port函数的编写示例 ****/

uint32_t Transmit485(volatile void *Data, size_t Len)
{
    SCB_CleanDCache_by_Addr((uint32_t *)Data, Len);                          // DMA发送前解决发送内存区 缓存一致性
    if(HAL_UART_Transmit_DMA(&huart4, (const uint8_t *)Data, Len) != HAL_OK) // 尝试开启传输DMA通道
        return DFLOW_PORT_RETURNT_ERR_INDEFINITE;
    return DFLOW_PORT_RETURN_DEFAULT;
}

uint32_t Receive485(volatile void *Data, size_t Len)
{
    if(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t *)Data, Len) != HAL_OK) // 尝试开启接收DMA通道
        return DFLOW_PORT_RETURNT_ERR_INDEFINITE;
    return DFLOW_PORT_RETURN_DEFAULT;
}

uint32_t TransmitGetState485(void)
{
    if(HAL_DMA_GetState(&hdma_uart4_tx) != HAL_DMA_STATE_READY) // 检查DMA通道是否已经完成事务
        return DFLOW_PORT_RETURN_CABLE_BUSY;
    return DFLOW_PORT_RETURN_DEFAULT;
}

void SendBefor485(void)
{
    /* 切换RS485收发芯片为发送模式 */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
}

void SendOver485(void)
{
    /* 切换RS485收发芯片为接收模式 */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
}

/**** 中断调用的示例 ****/

/* 供uart4的UART4中断 */
void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart4);
}

void DMA1_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_uart4_tx);
}

void DMA1_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_uart4_rx);
}

/**
 *@brief HAL UART的事件回调
 *
 * @param huart hal_Uart通道结构体
 * @param Size 可用的读数据字节数
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == &huart4)
    {
        switch(huart->RxEventType)
        {
        case HAL_UART_RXEVENT_IDLE:
            DFlow_Interrupt_IDLE_RC_FTF(&DFlow, Size);
            break;
        case HAL_UART_RXEVENT_TC: // 接收通道结束，程序运行不应当进入此分支，表明接收buffer已经不够大了
            DFlow_Interrupt_IDLE_RC_FTF(&DFlow, Size);
            break;

        default:
            break;
        }
    }
}

/**
  * @brief 覆盖hal库的 weak 传输结束
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart4)
    {
        DFlow_Interrupt_TC(&DFlow);
    }
}