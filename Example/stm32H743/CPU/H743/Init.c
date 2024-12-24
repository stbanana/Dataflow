/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : Init.c
 * Author        : yono
 * Date          : 2024-12-21
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    CPU及外设的支持
*/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;
DMA_HandleTypeDef  hdma_uart4_tx;
DMA_HandleTypeDef  hdma_uart4_rx;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 集合的芯片、时钟、外设等配置
 * @param  
 */
void InitCpu(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* 禁止 MPU */
    HAL_MPU_Disable( );

    /* 配置AXI SRAM的MPU属性为Write back, Read allocate，Write allocate */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x24000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* 配置D2域MPU */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x30000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_128KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* 配置以太网收发描述符部分为Strongly Ordered */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x30040000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_32KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* 配置SRAM4的MPU属性为Non-cacheable */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x38000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_64KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* 配置FMC 片选3 TNT4882的支持*/
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x68000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE; // 此外设需配置为无cache，否则会重复片选和读写使能
    MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER4;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /*使能 MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

    /* Enable I-Cache */
    SCB_EnableICache( );

    /* Enable D-Cache */
    SCB_EnableDCache( );

    HAL_Init( ); // HAL库初始化

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // /* 锁 SCU，避免重新上电无法运行的情况 */
    // MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

    /** Supply configuration update enable */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /* 供应配置更新启用 */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* 配置内部主稳压器输出电压 */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    __HAL_RCC_SYSCFG_CLK_ENABLE( );
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    /* 初始化RCC振荡器 */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 2;
    RCC_OscInitStruct.PLL.PLLN       = 64;
    RCC_OscInitStruct.PLL.PLLP       = 2;
    RCC_OscInitStruct.PLL.PLLQ       = 20;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler( );
    }

    /* 初始化CPU、AHB和APB总线时钟 */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler( );
    }

    __HAL_RCC_CSI_ENABLE( ); //使能CSI clock

    __HAL_RCC_SYSCFG_CLK_ENABLE( ); //使能SYSCFG clock

    HAL_EnableCompensationCell( ); //使能I/O补偿单元

    /* 使能D2和D3域时钟 */
    __HAL_RCC_D2SRAM1_CLK_ENABLE( );
    __HAL_RCC_D2SRAM2_CLK_ENABLE( );
    __HAL_RCC_D2SRAM3_CLK_ENABLE( );

    __HAL_RCC_BKPRAM_CLKAM_ENABLE( );
    __HAL_RCC_D3SRAM1_CLKAM_ENABLE( );
}

/**
 * @brief DMA初始化
 * @param  
 */
void DmaInit(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE( );

    /* DMA interrupt init */
    /* DMA1_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

/**
 * @brief USRT初始化
 * @param  
 */
void UartInit(void)
{
    huart4.Instance                    = UART4;
    huart4.Init.BaudRate               = 115200;
    huart4.Init.WordLength             = UART_WORDLENGTH_8B;
    huart4.Init.StopBits               = UART_STOPBITS_1;
    huart4.Init.Parity                 = UART_PARITY_NONE;
    huart4.Init.Mode                   = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart4.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart4.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if(HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler( );
    }
    if(HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler( );
    }
    if(HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler( );
    }
    if(HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
    {
        Error_Handler( );
    }
}

/**
 * @brief HAL uartMsp支持
 * @param uartHandle 
 */
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
    GPIO_InitTypeDef         GPIO_InitStruct     = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if(uartHandle->Instance == UART4)
    {
        /* USER CODE BEGIN UART4_MspInit 0 */

        /* USER CODE END UART4_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_UART4;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler( );
        }

        /* UART4 clock enable */
        __HAL_RCC_UART4_CLK_ENABLE( );

        __HAL_RCC_GPIOH_CLK_ENABLE( );
        __HAL_RCC_GPIOC_CLK_ENABLE( );
        /**UART4 GPIO Configuration
    PH14     ------> UART4_RX
    PC10     ------> UART4_TX
    */
        GPIO_InitStruct.Pin       = GPIO_PIN_14;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* UART4 DMA Init */
        /* UART4_TX Init */
        hdma_uart4_tx.Instance                 = DMA1_Stream0;
        hdma_uart4_tx.Init.Request             = DMA_REQUEST_UART4_TX;
        hdma_uart4_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_uart4_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart4_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart4_tx.Init.Mode                = DMA_NORMAL;
        hdma_uart4_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart4_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if(HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK)
        {
            Error_Handler( );
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_uart4_tx);

        /* UART4_RX Init */
        hdma_uart4_rx.Instance                 = DMA1_Stream1;
        hdma_uart4_rx.Init.Request             = DMA_REQUEST_UART4_RX;
        hdma_uart4_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_uart4_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart4_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart4_rx.Init.Mode                = DMA_NORMAL;
        hdma_uart4_rx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart4_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if(HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
        {
            Error_Handler( );
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_uart4_rx);

        /* UART4 interrupt Init */
        HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(UART4_IRQn);
        /* USER CODE BEGIN UART4_MspInit 1 */

        /* USER CODE END UART4_MspInit 1 */
    }
}

/**
 * @brief HAL uartMsp支持
 * @param uartHandle 
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
    if(uartHandle->Instance == UART4)
    {
        /* USER CODE BEGIN UART4_MspDeInit 0 */

        /* USER CODE END UART4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART4_CLK_DISABLE( );

        /**UART4 GPIO Configuration
    PH14     ------> UART4_RX
    PC10     ------> UART4_TX
    */
        HAL_GPIO_DeInit(GPIOH, GPIO_PIN_14);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10);

        /* UART4 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmatx);
        HAL_DMA_DeInit(uartHandle->hdmarx);

        /* UART4 interrupt Deinit */
        HAL_NVIC_DisableIRQ(UART4_IRQn);
        /* USER CODE BEGIN UART4_MspDeInit 1 */

        /* USER CODE END UART4_MspDeInit 1 */
    }
}