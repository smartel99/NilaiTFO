/**
 ******************************************************************************
 * @file    stm32l4xx_hal_adc.h
 * @author  MCD Application Team
 * @brief   Header file of ADC HAL module.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L4xx_HAL_ADC_H
#define STM32L4xx_HAL_ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
#include "stm32l4xx_hal_def.h"
#include "stm32l4xx_hal_dma.h"

/* Include low level driver */
#include "stm32l4xx_ll_adc.h"

    /** @addtogroup STM32L4xx_HAL_Driver
     * @{
     */

    /** @addtogroup ADC
     * @{
     */

    /* Exported types ------------------------------------------------------------*/
    /** @defgroup ADC_Exported_Types ADC Exported Types
     * @{
     */

    /**
     * @brief  ADC group regular oversampling structure definition
     */
    struct ADC_OversamplingTypeDef
    {
        uint32_t Ratio; /*!< Configures the oversampling ratio.
                             This parameter can be a value of @ref ADC_HAL_EC_OVS_RATIO */

        uint32_t RightBitShift; /*!< Configures the division coefficient for the Oversampler.
                                     This parameter can be a value of @ref ADC_HAL_EC_OVS_SHIFT */

        uint32_t
          TriggeredMode; /*!< Selects the regular triggered oversampling mode.
                              This parameter can be a value of @ref ADC_HAL_EC_OVS_DISCONT_MODE */

        uint32_t
          OversamplingStopReset; /*!< Selects the regular oversampling mode.
                                      The oversampling is either temporary stopped or reset upon an
                                    injected sequence interruption. If oversampling is enabled on
                                    both regular and injected groups, this parameter is discarded
                                    and forced to setting "ADC_REGOVERSAMPLING_RESUMED_MODE" (the
                                    oversampling buffer is zeroed during injection sequence). This
                                    parameter can be a value of @ref ADC_HAL_EC_OVS_SCOPE_REG */
    };

    /**
     * @brief  Structure definition of ADC instance and ADC group regular.
     * @note   Parameters of this structure are shared within 2 scopes:
     *          - Scope entire ADC (affects ADC groups regular and injected): ClockPrescaler,
     * Resolution, DataAlign, ScanConvMode, EOCSelection, LowPowerAutoWait.
     *          - Scope ADC group regular: ContinuousConvMode, NbrOfConversion,
     * DiscontinuousConvMode, NbrOfDiscConversion, ExternalTrigConv, ExternalTrigConvEdge,
     * DMAContinuousRequests, Overrun, OversamplingMode, Oversampling.
     * @note   The setting of these parameters by function HAL_ADC_Init() is conditioned to ADC
     * state. ADC state can be either:
     *          - For all parameters: ADC disabled
     *          - For all parameters except 'LowPowerAutoWait', 'DMAContinuousRequests' and
     * 'Oversampling': ADC enabled without conversion on going on group regular.
     *          - For parameters 'LowPowerAutoWait' and 'DMAContinuousRequests': ADC enabled without
     * conversion on going on groups regular and injected. If ADC is not in the appropriate state to
     * modify some parameters, these parameters setting is bypassed without error reporting (as it
     * can be the expected behavior in case of intended action to update another parameter (which
     * fulfills the ADC state condition) on the fly).
     */
    struct ADC_InitTypeDef
    {
        uint32_t
          ClockPrescaler; /*!< Select ADC clock source (synchronous clock derived from APB clock or
                             asynchronous clock derived from system clock or PLL (Refer to reference
                             manual for list of clocks available)) and clock prescaler. This
                             parameter can be a value of @ref ADC_HAL_EC_COMMON_CLOCK_SOURCE. Note:
                             The ADC clock configuration is common to all ADC instances. Note: In
                             case of usage of channels on injected group, ADC frequency should be
                             lower than AHB clock frequency /4 for resolution 12 or 10 bits, AHB
                             clock frequency /3 for resolution 8 bits, AHB clock frequency /2 for
                             resolution 6 bits. Note: In case of synchronous clock mode based on
                             HCLK/1, the configuration must be enabled only if the system clock has
                             a 50% duty clock cycle (APB prescaler configured inside RCC must be
                             bypassed and PCLK clock must have 50% duty cycle). Refer to reference
                             manual for details. Note: In case of usage of asynchronous clock, the
                             selected clock must be preliminarily enabled at RCC top level. Note:
                             This parameter can be modified only if all ADC instances are disabled.
                           */

        uint32_t Resolution; /*!< Configure the ADC resolution.
                                  This parameter can be a value of @ref ADC_HAL_EC_RESOLUTION */

        uint32_t
          DataAlign; /*!< Specify ADC data alignment in conversion data register (right or left).
                          Refer to reference manual for alignments formats versus resolutions.
                          This parameter can be a value of @ref ADC_HAL_EC_DATA_ALIGN */

        uint32_t
          ScanConvMode; /*!< Configure the sequencer of ADC groups regular and injected.
                             This parameter can be associated to parameter 'DiscontinuousConvMode'
                           to have main sequence subdivided in successive parts. If disabled:
                           Conversion is performed in single mode (one channel converted, the one
                           defined in rank 1). Parameters 'NbrOfConversion' and
                           'InjectedNbrOfConversion' are discarded (equivalent to set to 1). If
                           enabled:  Conversions are performed in sequence mode (multiple ranks
                           defined by 'NbrOfConversion' or 'InjectedNbrOfConversion' and rank of
                           each channel in sequencer). Scan direction is upward: from rank 1 to rank
                           'n'. This parameter can be a value of @ref ADC_Scan_mode */

        uint32_t
          EOCSelection; /*!< Specify which EOC (End Of Conversion) flag is used for conversion by
                           polling and interruption: end of unitary conversion or end of sequence
                           conversions. This parameter can be a value of @ref ADC_EOCSelection. */

        FunctionalState
          LowPowerAutoWait; /*!< Select the dynamic low power Auto Delay: new conversion start only
                               when the previous conversion (for ADC group regular) or previous
                               sequence (for ADC group injected) has been retrieved by user
                               software, using function HAL_ADC_GetValue() or
                               HAL_ADCEx_InjectedGetValue(). This feature automatically adapts the
                               frequency of ADC conversions triggers to the speed of the system that
                               reads the data. Moreover, this avoids risk of overrun for low
                               frequency applications. This parameter can be set to ENABLE or
                               DISABLE. Note: It is not recommended to use with interruption or DMA
                               (HAL_ADC_Start_IT(), HAL_ADC_Start_DMA()) since these modes have to
                               clear immediately the EOC flag (by CPU to free the IRQ pending event
                               or by DMA). Auto wait will work but fort a very short time,
                               discarding its intended benefit (except specific case of high load of
                               CPU or DMA transfers which can justify usage of auto wait). Do use
                               with polling: 1. Start conversion with HAL_ADC_Start(), 2. Later on,
                               when ADC conversion data is needed: use HAL_ADC_PollForConversion()
                               to ensure that conversion is completed and HAL_ADC_GetValue() to
                               retrieve conversion result and trig another conversion start. (in
                               case of usage of ADC group injected, use the equivalent functions
                               HAL_ADCExInjected_Start(), HAL_ADCEx_InjectedGetValue(), ...). */

        FunctionalState
          ContinuousConvMode; /*!< Specify whether the conversion is performed in single mode (one
                               conversion) or continuous mode for ADC group regular, after the first
                               ADC conversion start trigger occurred (software start or external
                               trigger). This parameter can be set to ENABLE or DISABLE. */

        uint32_t
          NbrOfConversion; /*!< Specify the number of ranks that will be converted within the
                              regular group sequencer. To use the regular group sequencer and
                              convert several ranks, parameter 'ScanConvMode' must be enabled. This
                              parameter must be a number between Min_Data = 1 and Max_Data = 16.
                                Note: This parameter must be modified when no conversion is on going
                              on regular group (ADC disabled, or ADC enabled without continuous mode
                              or external trigger that could launch a conversion). */

        FunctionalState
          DiscontinuousConvMode; /*!< Specify whether the conversions sequence of ADC group regular
                               is performed in Complete-sequence/Discontinuous-sequence (main
                               sequence subdivided in successive parts). Discontinuous mode is used
                               only if sequencer is enabled (parameter 'ScanConvMode'). If sequencer
                               is disabled, this parameter is discarded. Discontinuous mode can be
                               enabled only if continuous mode is disabled. If continuous mode is
                               enabled, this parameter setting is discarded. This parameter can be
                               set to ENABLE or DISABLE. */

        uint32_t
          NbrOfDiscConversion; /*!< Specifies the number of discontinuous conversions in which the
                                  main sequence of ADC group regular (parameter NbrOfConversion)
                                  will be subdivided. If parameter 'DiscontinuousConvMode' is
                                  disabled, this parameter is discarded. This parameter must be a
                                  number between Min_Data = 1 and Max_Data = 8. */

        uint32_t
          ExternalTrigConv; /*!< Select the external event source used to trigger ADC group regular
                               conversion start. If set to ADC_SOFTWARE_START, external triggers are
                               disabled and software trigger is used instead. This parameter can be
                               a value of @ref ADC_regular_external_trigger_source. Caution:
                               external trigger source is common to all ADC instances. */

        uint32_t
          ExternalTrigConvEdge; /*!< Select the external event edge used to trigger ADC group
                                   regular conversion start. If trigger source is set to
                                   ADC_SOFTWARE_START, this parameter is discarded. This parameter
                                   can be a value of @ref ADC_regular_external_trigger_edge */

        FunctionalState DMAContinuousRequests; /*!< Specify whether the DMA requests are performed
                                             in one shot mode (DMA transfer stops when number of
                                             conversions is reached) or in continuous mode (DMA
                                             transfer unlimited, whatever number of conversions).
                                             This parameter can be set to ENABLE or DISABLE.
                                             Note: In continuous mode, DMA must be configured in
                                             circular mode. Otherwise an overrun will be triggered
                                             when DMA buffer maximum pointer is reached. */

        uint32_t
          Overrun; /*!< Select the behavior in case of overrun: data overwritten or preserved
                      (default). This parameter applies to ADC group regular only. This parameter
                      can be a value of @ref ADC_HAL_EC_REG_OVR_DATA_BEHAVIOR. Note: In case of
                      overrun set to data preserved and usage with programming model with
                      interruption (HAL_Start_IT()): ADC IRQ handler has to clear end of conversion
                      flags, this induces the release of the preserved data. If needed, this data
                      can be saved in function HAL_ADC_ConvCpltCallback(), placed in user program
                      code (called before end of conversion flags clear). Note: Error reporting with
                      respect to the conversion mode:
                              - Usage with ADC conversion by polling for event or interruption:
                      Error is reported only if overrun is set to data preserved. If overrun is set
                      to data overwritten, user can willingly not read all the converted data, this
                      is not considered as an erroneous case.
                              - Usage with ADC conversion by DMA: Error is reported whatever overrun
                      setting (DMA is expected to process all data from data register). */

        FunctionalState
          OversamplingMode; /*!< Specify whether the oversampling feature is enabled or disabled.
                                 This parameter can be set to ENABLE or DISABLE.
                                 Note: This parameter can be modified only if there is no conversion
                               is ongoing on ADC groups regular and injected */

        ADC_OversamplingTypeDef
          Oversampling; /*!< Specify the Oversampling parameters.
                             Caution: this setting overwrites the previous oversampling
                           configuration if oversampling is already enabled. */

#if defined(ADC_CFGR_DFSDMCFG) && defined(DFSDM1_Channel0)
        uint32_t
          DFSDMConfig; /*!< Specify whether ADC conversion data is sent directly to DFSDM.
                            This parameter can be a value of @ref ADC_HAL_EC_REG_DFSDM_TRANSFER.
                            Note: This parameter can be modified only if there is no conversion is
                          ongoing (both ADSTART and JADSTART cleared). */

#endif /* ADC_CFGR_DFSDMCFG */
    };

    /**
     * @brief  Structure definition of ADC channel for regular group
     * @note   The setting of these parameters by function HAL_ADC_ConfigChannel() is conditioned to
     * ADC state. ADC state can be either:
     *          - For all parameters: ADC disabled (this is the only possible ADC state to modify
     * parameter 'SingleDiff')
     *          - For all except parameters 'SamplingTime', 'Offset', 'OffsetNumber': ADC enabled
     * without conversion on going on regular group.
     *          - For parameters 'SamplingTime', 'Offset', 'OffsetNumber': ADC enabled without
     * conversion on going on regular and injected groups. If ADC is not in the appropriate state to
     * modify some parameters, these parameters setting is bypassed without error reporting (as it
     * can be the expected behavior in case of intended action to update another parameter (which
     * fulfills the ADC state condition) on the fly).
     */
    struct ADC_ChannelConfTypeDef
    {
        uint32_t Channel; /*!< Specify the channel to configure into ADC regular group.
                               This parameter can be a value of @ref ADC_HAL_EC_CHANNEL
                               Note: Depending on devices and ADC instances, some channels may not
                             be available on device package pins. Refer to device datasheet for
                             channels availability. */

        uint32_t Rank; /*!< Specify the rank in the regular group sequencer.
                            This parameter can be a value of @ref ADC_HAL_EC_REG_SEQ_RANKS
                            Note: to disable a channel or change order of conversion sequencer, rank
                          containing a previous channel setting can be overwritten by the new
                          channel setting (or parameter number of conversions adjusted) */

        uint32_t
          SamplingTime; /*!< Sampling time value to be set for the selected channel.
                             Unit: ADC clock cycles
                             Conversion time is the addition of sampling time and processing time
                             (12.5 ADC clock cycles at ADC resolution 12 bits, 10.5 cycles at 10
                           bits, 8.5 cycles at 8 bits, 6.5 cycles at 6 bits). This parameter can be
                           a value of @ref ADC_HAL_EC_CHANNEL_SAMPLINGTIME Caution: This parameter
                           applies to a channel that can be used into regular and/or injected group.
                                      It overwrites the last setting.
                             Note: In case of usage of internal measurement channels
                           (VrefInt/Vbat/TempSensor), sampling time constraints must be respected
                           (sampling time can be adjusted in function of ADC clock frequency and
                           sampling time setting) Refer to device datasheet for timings values. */

        uint32_t
          SingleDiff; /*!< Select single-ended or differential input.
                           In differential mode: Differential measurement is carried out between the
                         selected channel 'i' (positive input) and channel 'i+1' (negative input).
                                                 Only channel 'i' has to be configured, channel
                         'i+1' is configured automatically. This parameter must be a value of @ref
                         ADC_HAL_EC_CHANNEL_SINGLE_DIFF_ENDING Caution: This parameter applies to a
                         channel that can be used in a regular and/or injected group. It overwrites
                         the last setting. Note: Refer to Reference Manual to ensure the selected
                         channel is available in differential mode. Note: When configuring a channel
                         'i' in differential mode, the channel 'i+1' is not usable separately. Note:
                         This parameter must be modified when ADC is disabled (before ADC start
                         conversion or after ADC stop conversion). If ADC is enabled, this parameter
                         setting is bypassed without error reporting (as it can be the expected
                         behavior in case of another parameter update on the fly) */

        uint32_t OffsetNumber; /*!< Select the offset number
                                    This parameter can be a value of @ref ADC_HAL_EC_OFFSET_NB
                                    Caution: Only one offset is allowed per channel. This parameter
                                  overwrites the last setting. */

        uint32_t Offset; /*!< Define the offset to be subtracted from the raw converted data.
                              Offset value must be a positive number.
                              Depending of ADC resolution selected (12, 10, 8 or 6 bits), this
                            parameter must be a number between Min_Data = 0x000 and Max_Data =
                            0xFFF, 0x3FF, 0xFF or 0x3F respectively. Note: This parameter must be
                            modified when no conversion is on going on both regular and injected
                            groups (ADC disabled, or ADC enabled without continuous mode or external
                            trigger that could launch a conversion). */
    };

    /**
     * @brief  Structure definition of ADC analog watchdog
     * @note   The setting of these parameters by function HAL_ADC_AnalogWDGConfig() is conditioned
     * to ADC state. ADC state can be either:
     *          - For all parameters: ADC disabled or ADC enabled without conversion on going on ADC
     * groups regular and injected.
     */
    struct ADC_AnalogWDGConfTypeDef
    {
        uint32_t
          WatchdogNumber; /*!< Select which ADC analog watchdog is monitoring the selected channel.
                               For Analog Watchdog 1: Only 1 channel can be monitored (or overall
                             group of channels by setting parameter 'WatchdogMode') For Analog
                             Watchdog 2 and 3: Several channels can be monitored (by successive
                             calls of 'HAL_ADC_AnalogWDGConfig()' for each channel) This parameter
                             can be a value of @ref ADC_HAL_EC_AWD_NUMBER. */

        uint32_t WatchdogMode; /*!< Configure the ADC analog watchdog mode: single/all/none
                                  channels. For Analog Watchdog 1: Configure the ADC analog watchdog
                                  mode: single channel or all channels, ADC groups regular and-or
                                  injected. For Analog Watchdog 2 and 3: Several channels can be
                                  monitored by applying successively the AWD init structure.
                                  Channels on ADC group regular and injected are not differentiated:
                                  Set value 'ADC_ANALOGWATCHDOG_SINGLE_xxx' to monitor 1 channel,
                                  value 'ADC_ANALOGWATCHDOG_ALL_xxx' to monitor all channels,
                                  'ADC_ANALOGWATCHDOG_NONE' to monitor no channel. This parameter
                                  can be a value of @ref ADC_analog_watchdog_mode. */

        uint32_t
          Channel; /*!< Select which ADC channel to monitor by analog watchdog.
                        For Analog Watchdog 1: this parameter has an effect only if parameter
                      'WatchdogMode' is configured on single channel (only 1 channel can be
                      monitored). For Analog Watchdog 2 and 3: Several channels can be monitored. To
                      use this feature, call successively the function HAL_ADC_AnalogWDGConfig() for
                      each channel to be added (or removed with value 'ADC_ANALOGWATCHDOG_NONE').
                        This parameter can be a value of @ref ADC_HAL_EC_CHANNEL. */

        FunctionalState
          ITMode; /*!< Specify whether the analog watchdog is configured in interrupt or polling
                     mode. This parameter can be set to ENABLE or DISABLE */

        uint32_t HighThreshold; /*!< Configure the ADC analog watchdog High threshold value.
                                     Depending of ADC resolution selected (12, 10, 8 or 6 bits),
                                   this parameter must be a number between Min_Data = 0x000 and
                                   Max_Data = 0xFFF, 0x3FF, 0xFF or 0x3F respectively. Note: Analog
                                   watchdog 2 and 3 are limited to a resolution of 8 bits: if ADC
                                   resolution is 12 bits the 4 LSB are ignored, if ADC resolution is
                                   10 bits the 2 LSB are ignored. Note: If ADC oversampling is
                                   enabled, ADC analog watchdog thresholds are impacted: the
                                   comparison of analog watchdog thresholds is done on oversampling
                                   final computation (after ratio and shift application): ADC data
                                   register bitfield [15:4] (12 most significant bits). */

        uint32_t
          LowThreshold; /*!< Configures the ADC analog watchdog Low threshold value.
                             Depending of ADC resolution selected (12, 10, 8 or 6 bits), this
                           parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF,
                           0x3FF, 0xFF or 0x3F respectively. Note: Analog watchdog 2 and 3 are
                           limited to a resolution of 8 bits: if ADC resolution is 12 bits the 4 LSB
                           are ignored, if ADC resolution is 10 bits the 2 LSB are ignored. Note: If
                           ADC oversampling is enabled, ADC analog watchdog thresholds are impacted:
                           the comparison of analog watchdog thresholds is done on oversampling
                           final computation (after ratio and shift application): ADC data register
                           bitfield [15:4] (12 most significant bits). */
    };

    /**
     * @brief  ADC group injected contexts queue configuration
     * @note   Structure intended to be used only through structure "ADC_HandleTypeDef"
     */
    struct ADC_InjectionConfigTypeDef
    {
        uint32_t ContextQueue; /*!< Injected channel configuration context: build-up over each
                                    HAL_ADCEx_InjectedConfigChannel() call to finally initialize
                                    JSQR register at HAL_ADCEx_InjectedConfigChannel() last call */

        uint32_t ChannelCount; /*!< Number of channels in the injected sequence */
    };
    

    /**
     * @brief  ADC handle Structure definition
     */

    struct ADC_HandleTypeDef
    {
        ADC_TypeDef*    Instance; /*!< Register base address */
        ADC_InitTypeDef Init; /*!< ADC initialization parameters and regular conversions setting */
        DMA_HandleTypeDef* DMA_Handle; /*!< Pointer DMA Handler */
        HAL_LockTypeDef    Lock;       /*!< ADC locking object */
        NILAI_IO uint32_t  State;      /*!< ADC communication state (bitmap of ADC states) */
        NILAI_IO uint32_t  ErrorCode;  /*!< ADC Error code */
        ADC_InjectionConfigTypeDef
          InjectionConfig; /*!< ADC injected channel configuration build-up structure */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
        void (*ConvCpltCallback)(ADC_HandleTypeDef* hadc); /*!< ADC conversion complete callback */
        void (*ConvHalfCpltCallback)(
          ADC_HandleTypeDef* hadc); /*!< ADC conversion DMA half-transfer callback */
        void (*LevelOutOfWindowCallback)(
          ADC_HandleTypeDef* hadc);                     /*!< ADC analog watchdog 1 callback */
        void (*ErrorCallback)(ADC_HandleTypeDef* hadc); /*!< ADC error callback */
        void (*InjectedConvCpltCallback)(
          ADC_HandleTypeDef* hadc); /*!< ADC group injected conversion complete callback */
        void (*InjectedQueueOverflowCallback)(
          ADC_HandleTypeDef* hadc); /*!< ADC group injected context queue overflow callback */
        void (*LevelOutOfWindow2Callback)(
          ADC_HandleTypeDef* hadc); /*!< ADC analog watchdog 2 callback */
        void (*LevelOutOfWindow3Callback)(
          ADC_HandleTypeDef* hadc); /*!< ADC analog watchdog 3 callback */
        void (*EndOfSamplingCallback)(ADC_HandleTypeDef* hadc); /*!< ADC end of sampling callback */
        void (*MspInitCallback)(ADC_HandleTypeDef* hadc);       /*!< ADC Msp Init callback */
        void (*MspDeInitCallback)(ADC_HandleTypeDef* hadc);     /*!< ADC Msp DeInit callback */
#endif                                                          /* USE_HAL_ADC_REGISTER_CALLBACKS */
    };

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /**
     * @brief  HAL ADC Callback ID enumeration definition
     */
    enum HAL_ADC_CallbackIDTypeDef
    {
        HAL_ADC_CONVERSION_COMPLETE_CB_ID = 0x00U, /*!< ADC conversion complete callback ID */
        HAL_ADC_CONVERSION_HALF_CB_ID = 0x01U, /*!< ADC conversion DMA half-transfer callback ID */
        HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID = 0x02U, /*!< ADC analog watchdog 1 callback ID */
        HAL_ADC_ERROR_CB_ID                 = 0x03U, /*!< ADC error callback ID */
        HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID =
          0x04U, /*!< ADC group injected conversion complete callback ID */
        HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID =
          0x05U, /*!< ADC group injected context queue overflow callback ID */
        HAL_ADC_LEVEL_OUT_OF_WINDOW_2_CB_ID = 0x06U, /*!< ADC analog watchdog 2 callback ID */
        HAL_ADC_LEVEL_OUT_OF_WINDOW_3_CB_ID = 0x07U, /*!< ADC analog watchdog 3 callback ID */
        HAL_ADC_END_OF_SAMPLING_CB_ID       = 0x08U, /*!< ADC end of sampling callback ID */
        HAL_ADC_MSPINIT_CB_ID               = 0x09U, /*!< ADC Msp Init callback ID          */
        HAL_ADC_MSPDEINIT_CB_ID             = 0x0AU  /*!< ADC Msp DeInit callback ID        */
    };

    /**
     * @brief  HAL ADC Callback pointer definition
     */
    typedef void (*pADC_CallbackTypeDef)(
      ADC_HandleTypeDef* hadc); /*!< pointer to a ADC callback function */

#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
 * @}
 */

/* Include ADC HAL Extended module */
#include "stm32l4xx_hal_adc_ex.h"

    /* Exported functions --------------------------------------------------------*/
    /** @addtogroup ADC_Exported_Functions
     * @{
     */

    /** @addtogroup ADC_Exported_Functions_Group1
     * @brief    Initialization and Configuration functions
     * @{
     */
    /* Initialization and de-initialization functions  ****************************/
    HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc);
    void              HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
    void              HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /* Callbacks Register/UnRegister functions  ***********************************/
    HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef*        hadc,
                                               HAL_ADC_CallbackIDTypeDef CallbackID,
                                               pADC_CallbackTypeDef      pCallback);
    HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef*        hadc,
                                                 HAL_ADC_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    /**
     * @}
     */

    /** @addtogroup ADC_Exported_Functions_Group2
     * @brief    IO operation functions
     * @{
     */
    /* IO operation functions  *****************************************************/

    /* Blocking mode: Polling */
    HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
    HAL_StatusTypeDef HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc,
                                           uint32_t           EventType,
                                           uint32_t           Timeout);

    /* Non-blocking mode: Interruption */
    HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc);

    /* Non-blocking mode: DMA */
    HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
    HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc);

    /* ADC retrieve conversion value intended to be used with polling or interruption */
    uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);

    /* ADC IRQHandler and Callbacks used in non-blocking modes (Interruption and DMA) */
    void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
    void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
    void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc);
    void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc);
    /**
     * @}
     */

    /** @addtogroup ADC_Exported_Functions_Group3 Peripheral Control functions
     *  @brief    Peripheral Control functions
     * @{
     */
    /* Peripheral Control functions ***********************************************/
    HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef*      hadc,
                                            ADC_ChannelConfTypeDef* sConfig);
    HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef*        hadc,
                                              ADC_AnalogWDGConfTypeDef* AnalogWDGConfig);

    /**
     * @}
     */

    /* Peripheral State functions *************************************************/
    /** @addtogroup ADC_Exported_Functions_Group4
     * @{
     */
    uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc);
    uint32_t HAL_ADC_GetError(ADC_HandleTypeDef* hadc);

    /**
     * @}
     */

    /**
     * @}
     */

    /* Private functions -----------------------------------------------------------*/
    /** @addtogroup ADC_Private_Functions ADC Private Functions
     * @{
     */
    HAL_StatusTypeDef ADC_ConversionStop(ADC_HandleTypeDef* hadc, uint32_t ConversionGroup);
    HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef ADC_Disable(ADC_HandleTypeDef* hadc);
    void              ADC_DMAConvCplt(DMA_HandleTypeDef* hdma);
    void              ADC_DMAHalfConvCplt(DMA_HandleTypeDef* hdma);
    void              ADC_DMAError(DMA_HandleTypeDef* hdma);

    /**
     * @}
     */

    /**
     * @}
     */

    /**
     * @}
     */

#ifdef __cplusplus
}
#endif


#endif /* STM32L4xx_HAL_ADC_H */
