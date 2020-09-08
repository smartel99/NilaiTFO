/**
 ******************************************************************************
 * @addtogroup stm32f4xxInterrupts
 * @{
 * @file    stm32f4xxInterrupts
 * @author  Samuel Martel
 * @brief   Header for the stm32f4xxInterrupts module.
 *
 * @date 9/3/2020 3:15:17 PM
 *
 ******************************************************************************
 */
#ifndef _stm32f4xxInterrupts
#define _stm32f4xxInterrupts
#if 0
/*****************************************************************************/
/* Includes */
// #TODO change to a configurable path
#include "stm32f4xx_hal_conf.h"

/*****************************************************************************/
/* Exported defines */


/*****************************************************************************/
/* Exported macro */


/*****************************************************************************/
/* Exported types */
enum class Interrupts
{
#if defined(HAL_WWDG_MODULE_ENABLED)
    // Window Watchdog Interrupt.
    WWDG = 0,
#endif
    
#if defined(HAL_PWR_MODULE_ENABLED)
    // PVD through EXTI line detection interrupt.
    PVD,
#endif
    
#if defined(HAL_RTC_MODULE_ENABLED)
    // Tamper and TimeStamp interrupts through the EXTI line.
    TAMP_STAMP,
    // RTC Wakeup interrupt through the EXTI line.
    RTC_WKUP,
    // RTC Alarms (A and B) through EXTI line interrupt.
    RTC_Alarm,
#endif
    
#if defined(HAL_FLASH_MODULE_ENABLED)
    // Flash global interrupt.
    FLASH_INT,
#endif
    
#if defined(HAL_RCC_MODULE_ENABLED)
    // RCC global interrupt.
    RCC_INT,
#endif
    
#if defined(HAL_EXTI_MODULE_ENABLED)
    // EXTI Line0 interrupt.
    EXTI0,
    // EXTI Line1 interrupt.
    EXTI1,
    // EXTI Line2 interrupt.
    EXTI2,
    // EXTI Line3 interrupt.
    EXTI3,
    // EXTI Line4 interrupt.
    EXTI4,
    // EXTI Line[9:5] interrupts.
    EXTI9_5,
    // EXTI Line[15:10] interrupts.
    EXTI15_10,
#endif
    
#if defined(HAL_DMA_MODULE_ENABLED)
    // DMA1 Stream0 global interrupt.
    DMA1_Stream0_INT,
    // DMA1 Stream1 global interrupt.
    DMA1_Stream1_INT,
    // DMA1 Stream2 global interrupt.
    DMA1_Stream2_INT,
    // DMA1 Stream3 global interrupt.
    DMA1_Stream3_INT,
    // DMA1 Stream4 global interrupt.
    DMA1_Stream4_INT,
    // DMA1 Stream5 global interrupt.
    DMA1_Stream5_INT,
    // DMA1 Stream6 global interrupt.
    DMA1_Stream6_INT,
    // DMA1 Stream7 global interrupt.
    DMA1_Stream7_INT,
    // DMA2 Stream0 global interrupt.
    DMA2_Stream0_INT,
    // DMA2 Stream1 global interrupt.
    DMA2_Stream1_INT,
    // DMA2 Stream2 global interrupt.
    DMA2_Stream2_INT,
    // DMA2 Stream3 global interrupt.
    DMA2_Stream3_INT,
    // DMA2 Stream4 global interrupt.
    DMA2_Stream4_INT,
    // DMA2 Stream5 global interrupt.
    DMA2_Stream5_INT,
    // DMA2 Stream6 global interrupt.
    DMA2_Stream6_INT,
    // DMA2 Stream7 global interrupt.
    DMA2_Stream7_INT,
#endif
    
#if defined(HAL_ADC_MODULE_ENABLED)
    // ADC1, ADC2 and ADC3 global interrupts.
    ADC_INT,
#endif
    
#if defined(HAL_CAN_MODULE_ENABLED) || defined(HAL_CAN_LEGACY_MODULE_ENABLED)
    // CAN1 TX interrupts.
    CAN1_TX,
    // CAN1 RX0 interrupts.
    CAN1_RX0,
    // CAN1 RX1 interrupts.
    CAN1_RX1,
    // CAN1 SCE interrupt.
    CAN1_SCE,
    // CAN2 TX interrupts.
    CAN2_TX,
    // CAN2 RX0 interrupts.
    CAN2_RX0,
    // CAN2 RX1 interrupts.
    CAN2_RX1,
    // CAN2 SCE interrupt.
    CAN2_SCE,
#endif
    
#if defined(HAL_TIM_MODULE_ENABLED)
    // TIM1 Break interrupt and TIM9 global interrupt.
    TIM1_BRK_TIM9,
    // TIM1 Update interrupt and TIM10 global interrupt.
    TIM1_UP_TIM10,
    // TIM1 Trigger and Commutation interrupts and TIM11 global interrupt.
    TIM1_TRG_COM_TIM11,
    // TIM1 Capture Compare interrupt.
    TIM1_CC,
    // TIM2 global interrupt.
    TIM2_INT,
    // TIM3 global interrupt.
    TIM3_INT,
    // TIM4 global interrupt.
    TIM4_INT,
    // TIM5 global interrupt.
    TIM5_INT,
    // TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
    TIM6_DAC,
    // TIM7 global interrupt.
    TIM7_INT,
    // TIM8 Break interrupt and TIM12 global interrupt.
    TIM8_BRK_TIM12,
    // TIM8 Update interrupt and TIM13 global interrupt.
    TIM8_UP_TIM13,
    // TIM8 Trigger and Commutation interrupts and TIM14 global interrupt.
    TIM8_TRG_COM_TIM14,
    // TIM8 Capture Compare interrupt.
    TIM8_CC,
#endif
    
#if defined(HAL_I2C_MODULE_ENABLED)
    // I2C1 Event interrupt.
    I2C1_EV,
    // I2C1 Error interrupt.
    I2C1_ER,
    // I2C2 Event interrupt.
    I2C2_EV,
    // I2C2 Error interrupt.
    I2C2_ER,
    // I2C3 event interrupt.
    I2C3_EV,
    // I2C3 error interrupt.
    I2C3_ER,
#endif
    
#if defined(HAL_SPI_MODULE_ENABLED)
    // SPI1 global interrupt.
    SPI1_INT,
    // SPI2 global interrupt.
    SPI2_INT,
    // SPI3 global interrupt.
    SPI3_INT,
#endif
    
#if defined(HAL_USART_MODULE_ENABLED)
    // USART1 global interrupt.
    USART1_INT,
    // USART2 global interrupt.
    USART2_INT,
    // USART3 global interrupt.
    USART3_INT,
    // USART6 global interrupt.
    USART6_INT,
#endif
    
    // #TODO Find define for USB
    // USB On-The-Go FS Wakeup through EXTI line interrupt.
    OTH_FS_WKUP,
    // USB On-The-Go FS global interrupt.
    OTG_FS,
    // USB On-The-Go HS End Point 1 Out global interrupt.
    OTG_HS_EP1_OUT,
    // USB On-The-Go HS End Point 1 In global interrupt.
    OTG_HS_EP1_IN,
    // USB On-The-Go HS Wakeup through EXTI interrupt.
    OTG_HS_WKUP,
    
#if defined(HAL_NAND_MODULE_ENABLED) \
    || defined(HAL_NOR_MODULE_ENABLED) \
        || defined(HAL_SRAM_MODULE_ENABLED)\
        || defined(HAL_SDRAM_MODULE_ENABLED)
    // FSMC global interrupt.
    FSMC,
#endif
    
#if defined(HAL_SD_MODULE_ENABLED) || defined(HAL_MMC_MODULE_ENABLED)
    // SDIO global interrupt.
    SDIO_INT,
#endif
    
#if defined(HAL_UART_MODULE_ENABLED)
    // UART4 global interrupt.
    UART4_INT,
    // UART5 global interrupt.
    UART5_INT,
#endif    
    
#if defined(HAL_ETH_MODULE_ENABLED)
    // Ethernet global interrupt.
    ETH,
    // Ethernet Wakeup through EXTI line interrupt.
    ETH_WKUP,
#endif    
    
#if defined(HAL_DCMI_MODULE_ENABLED)
    // DCMI global interrupt.
    DCMI,
#endif
    
#if defined(HAL_CRYP_MODULE_ENABLED)
    // CRYP crypto global interrupt.
    CRYP,
#endif
    
#if defined(HAL_RNG_MODULE_ENABLED) || defined(HAL_HASH_MODULE_ENABLED)
    // Hash and RNG global interrup.
    HASH_RNG,
#endif
    
    // #TODO find the include for FPU
    // FPU global interrupt.
    FPU_INT,
};

/*****************************************************************************/
/* Exported functions */
void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();


void WWDG_IRQHandler();
void PVD_IRQHandler();
void TAMP_STAMP_IRQHandler();
void RTC_WKUP_IRQHandler();
void FLASH_IRQHandler();
void RCC_IRQHandler();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
void EXTI2_IRQHandler();
void EXTI3_IRQHandler();
void EXTI4_IRQHandler();
void DMA1_Stream0_IRQHandler();
void DMA1_Stream1_IRQHandler();
void DMA1_Stream2_IRQHandler();
void DMA1_Stream3_IRQHandler();
void DMA1_Stream4_IRQHandler();
void DMA1_Stream5_IRQHandler();
void DMA1_Stream6_IRQHandler();
void ADC_IRQHandler();
void CAN1_TX_IRQHandler();
void CAN1_RX0_IRQHandler();
void CAN1_RX1_IRQHandler();
void CAN1_SCE_IRQHandler();
void EXTI9_5_IRQHandler();
void TIM1_BRK_TIM9_IRQHandler();
void TIM1_UP_TIM10_IRQHandler();
void TIM1_TRG_COM_TIM11_IRQHandler();
void TIM1_CC_IRQHandler();
void TIM2_IRQHandler();
void TIM3_IRQHandler();
void TIM4_IRQHandler();
void I2C1_EV_IRQHandler();
void I2C1_ER_IRQHandler();
void I2C2_EV_IRQHandler();
void I2C2_ER_IRQHandler();
void SPI1_IRQHandler();
void SPI2_IRQHandler();
void USART1_IRQHandler();
void USART2_IRQHandler();
void USART3_IRQHandler();
void EXTI15_10_IRQHandler();
void RTC_Alarm_IRQHandler();
void OTG_FS_WKUP_IRQHandler();
void TIM8_BRK_TIM12_IRQHandler();
void TIM8_UP_TIM13_IRQHandler();
void TIM8_TRG_COM_TIM14_IRQHandler();
void TIM8_CC_IRQHandler();
void DMA1_Stream7_IRQHandler();
void FSMC_IRQHandler();
void SDIO_IRQHandler();
void TIM5_IRQHandler();
void SPI3_IRQHandler();
void UART4_IRQHandler();
void UART5_IRQHandler();
void TIM6_DAC_IRQHandler();
void TIM7_IRQHandler();
void DMA2_Stream0_IRQHandler();
void DMA2_Stream1_IRQHandler();
void DMA2_Stream2_IRQHandler();
void DMA2_Stream3_IRQHandler();
void DMA2_Stream4_IRQHandler();
void CAN2_TX_IRQHandler();
void CAN2_RX0_IRQHandler();
void CAN2_RX1_IRQHandler();
void CAN2_SCE_IRQHandler();
void OTG_FS_IRQHandler();
void DMA2_Stream5_IRQHandler();
void DMA2_Stream6_IRQHandler();
void DMA2_Stream7_IRQHandler();
void USART6_IRQHandler();
void I2C3_EV_IRQHandler();
void I2C3_ER_IRQHandler();
void OTG_HS_EP1_OUT_IRQHandler();
void OTG_HS_EP1_IN_IRQHandler();
void OTG_HS_WKUP_IRQHandler();
void OTG_HS_IRQHandler();
void HASH_RNG_IRQHandler();
void FPU_IRQHandler();

/* Have a wonderful day :) */
#endif /* _stm32f4xxInterrupts */
/**
 * @}
 */
#endif
/****** END OF FILE ******/
