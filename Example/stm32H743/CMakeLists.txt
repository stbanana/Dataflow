cmake_minimum_required(VERSION 3.22)

set(CMAKE_PROJECT_NAME user_src) 

project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(${CMAKE_PROJECT_NAME} INTERFACE 
	USE_HAL_DRIVER 
	STM32H743xx
    $<$<CONFIG:Debug>:DEBUG>
)


target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/CPU
    ${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Inc
    ${CMAKE_CURRENT_LIST_DIR}/CMSIS/Include
    ${CMAKE_CURRENT_LIST_DIR}/CMSIS/Device/ST/STM32H7xx/Include
)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/h7_main.c
    ${HAL_SRC}
    ${2_CONTORL_SRC}
    ${3_MODULE_SRC}
    ${4_DRIVER_SRC}
    ${5_DRIVER_SRC}
    # CPU部分
    ${CMAKE_CURRENT_LIST_DIR}/CPU/GNU/startup_stm32h743xx.s
    ${CMAKE_CURRENT_LIST_DIR}/CPU/stm32h7xx_hal_timebase_tim.c
    ${CMAKE_CURRENT_LIST_DIR}/CPU/stm32h7xx_it.c
    ${CMAKE_CURRENT_LIST_DIR}/CPU/H743/Init.c
    # 库支持部分
    ${CMAKE_CURRENT_LIST_DIR}/CPU/GNU/syscalls.c
    ${CMAKE_CURRENT_LIST_DIR}/CPU/GNU/sysmem.c
    # hal部分
    ${CMAKE_CURRENT_LIST_DIR}/CMSIS/Device/ST/STM32H7xx/Source/Templates/system_stm32h7xx.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_cortex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_exti.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_flash.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_flash_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_gpio.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_i2c.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_i2c_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_pwr.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_pwr_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_rcc.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_rcc_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_tim.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_tim_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_uart.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_uart_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_sram.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_ll_fmc.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_mdma.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_hcd.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_ll_usb.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_ltdc.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma2d.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_sdram.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_qspi.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_fdcan.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_spi.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_pcd.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_pcd_ex.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_eth.c
	${CMAKE_CURRENT_LIST_DIR}/stm32h7xx-hal-driver/Src/stm32h7xx_hal_eth_ex.c
)

target_link_directories(${CMAKE_PROJECT_NAME} INTERFACE

)

target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE
    
    # ${CMAKE_CURRENT_LIST_DIR}/../../../5_PhysicalChip/GD32F3x0/CMSIS/GD/arm_cortexM4lf_math.lib
)


if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


