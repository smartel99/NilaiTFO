/**
 ******************************************************************************
 * @file    stm32l4xx_hal_adc_ex.h
 * @author  MCD Application Team
 * @brief   Header file of ADC HAL extended module.
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
#ifndef STM32L4xx_HAL_ADC_EX_H
#define STM32L4xx_HAL_ADC_EX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal_def.h"

    /** @addtogroup STM32L4xx_HAL_Driver
     * @{
     */

    /** @addtogroup ADCEx
     * @{
     */

    /* Exported types ------------------------------------------------------------*/
    /** @defgroup ADCEx_Exported_Types ADC Extended Exported Types
     * @{
     */

    /**
     * @brief  ADC Injected Conversion Oversampling structure definition
     */
    struct ADC_InjOversamplingTypeDef
    {
        uint32_t Ratio; /*!< Configures the oversampling ratio.
                             This parameter can be a value of @ref ADC_HAL_EC_OVS_RATIO */

        uint32_t RightBitShift; /*!< Configures the division coefficient for the Oversampler.
                                     This parameter can be a value of @ref ADC_HAL_EC_OVS_SHIFT */
    };

    /**
     * @brief  Structure definition of ADC group injected and ADC channel affected to ADC group
     * injected
     * @note   Parameters of this structure are shared within 2 scopes:
     *          - Scope channel: InjectedChannel, InjectedRank, InjectedSamplingTime ,
     * InjectedSingleDiff, InjectedOffsetNumber, InjectedOffset
     *          - Scope ADC group injected (affects all channels of injected group):
     * InjectedNbrOfConversion, InjectedDiscontinuousConvMode, AutoInjectedConv,
     * QueueInjectedContext, ExternalTrigInjecConv, ExternalTrigInjecConvEdge,
     * InjecOversamplingMode, InjecOversampling.
     * @note   The setting of these parameters by function HAL_ADCEx_InjectedConfigChannel() is
     * conditioned to ADC state. ADC state can be either:
     *          - For all parameters: ADC disabled (this is the only possible ADC state to modify
     * parameter 'InjectedSingleDiff')
     *          - For parameters 'InjectedDiscontinuousConvMode', 'QueueInjectedContext',
     * 'InjecOversampling': ADC enabled without conversion on going on injected group.
     *          - For parameters 'InjectedSamplingTime', 'InjectedOffset', 'InjectedOffsetNumber',
     * 'AutoInjectedConv': ADC enabled without conversion on going on regular and injected groups.
     *          - For parameters 'InjectedChannel', 'InjectedRank', 'InjectedNbrOfConversion',
     * 'ExternalTrigInjecConv', 'ExternalTrigInjecConvEdge': ADC enabled and while conversion on
     * going on ADC groups regular and injected. If ADC is not in the appropriate state to modify
     * some parameters, these parameters setting is bypassed without error reporting (as it can be
     * the expected behavior in case of intended action to update another parameter (which fulfills
     * the ADC state condition) on the fly).
     */
    struct ADC_InjectionConfTypeDef
    {
        uint32_t InjectedChannel; /*!< Specifies the channel to configure into ADC group injected.
                                       This parameter can be a value of @ref ADC_HAL_EC_CHANNEL
                                       Note: Depending on devices and ADC instances, some channels
                                     may not be available on device package pins. Refer to device
                                     datasheet for channels availability. */

        uint32_t
          InjectedRank; /*!< Specifies the rank in the ADC group injected sequencer.
                             This parameter must be a value of @ref ADC_INJ_SEQ_RANKS.
                             Note: to disable a channel or change order of conversion sequencer,
                           rank containing a previous channel setting can be overwritten by the new
                           channel setting (or parameter number of conversions adjusted) */

        uint32_t
          InjectedSamplingTime; /*!< Sampling time value to be set for the selected channel.
                                     Unit: ADC clock cycles.
                                     Conversion time is the addition of sampling time and processing
                                   time (12.5 ADC clock cycles at ADC resolution 12 bits, 10.5
                                   cycles at 10 bits, 8.5 cycles at 8 bits, 6.5 cycles at 6 bits).
                                     This parameter can be a value of @ref
                                   ADC_HAL_EC_CHANNEL_SAMPLINGTIME. Caution: This parameter applies
                                   to a channel that can be used in a regular and/or injected group.
                                              It overwrites the last setting.
                                     Note: In case of usage of internal measurement channels
                                   (VrefInt/Vbat/TempSensor), sampling time constraints must be
                                   respected (sampling time can be adjusted in function of ADC clock
                                   frequency and sampling time setting) Refer to device datasheet
                                   for timings values. */

        uint32_t
          InjectedSingleDiff; /*!< Selection of single-ended or differential input.
                                   In differential mode: Differential measurement is between the
                                 selected channel 'i' (positive input) and channel 'i+1' (negative
                                 input). Only channel 'i' has to be configured, channel 'i+1' is
                                 configured automatically. This parameter must be a value of @ref
                                 ADC_HAL_EC_CHANNEL_SINGLE_DIFF_ENDING. Caution: This parameter
                                 applies to a channel that can be used in a regular and/or injected
                                 group. It overwrites the last setting. Note: Refer to Reference
                                 Manual to ensure the selected channel is available in differential
                                 mode. Note: When configuring a channel 'i' in differential mode,
                                 the channel 'i+1' is not usable separately. Note: This parameter
                                 must be modified when ADC is disabled (before ADC start conversion
                                 or after ADC stop conversion). If ADC is enabled, this parameter
                                 setting is bypassed without error reporting (as it can be the
                                 expected behavior in case of another parameter update on the fly)
                               */

        uint32_t
          InjectedOffsetNumber; /*!< Selects the offset number.
                                     This parameter can be a value of @ref ADC_HAL_EC_OFFSET_NB.
                                     Caution: Only one offset is allowed per channel. This parameter
                                   overwrites the last setting. */

        uint32_t
          InjectedOffset; /*!< Defines the offset to be subtracted from the raw converted data.
                               Offset value must be a positive number.
                               Depending of ADC resolution selected (12, 10, 8 or 6 bits), this
                             parameter must be a number between Min_Data = 0x000 and Max_Data =
                             0xFFF,  0x3FF, 0xFF or 0x3F respectively. Note: This parameter must be
                             modified when no conversion is on going on both regular and injected
                             groups (ADC disabled, or ADC enabled without continuous mode or
                             external trigger that could launch a conversion). */

        uint32_t
          InjectedNbrOfConversion; /*!< Specifies the number of ranks that will be converted within
                                      the ADC group injected sequencer. To use the injected group
                                      sequencer and convert several ranks, parameter 'ScanConvMode'
                                      must be enabled. This parameter must be a number between
                                      Min_Data = 1 and Max_Data = 4. Caution: this setting impacts
                                      the entire injected group. Therefore, call of
                                      HAL_ADCEx_InjectedConfigChannel() to configure a channel on
                                      injected group can impact the configuration of other channels
                                      previously set. */

        FunctionalState
          InjectedDiscontinuousConvMode; /*!< Specifies whether the conversions sequence of ADC
                                       group injected is performed in
                                       Complete-sequence/Discontinuous-sequence (main sequence
                                       subdivided in successive parts). Discontinuous mode is used
                                       only if sequencer is enabled (parameter 'ScanConvMode'). If
                                       sequencer is disabled, this parameter is discarded.
                                       Discontinuous mode can be enabled only if continuous mode is
                                       disabled. This parameter can be set to ENABLE or DISABLE.
                                       Note: This parameter must be modified when ADC is disabled
                                       (before ADC start conversion or after ADC stop conversion).
                                       Note: For injected group, discontinuous mode converts the
                                       sequence channel by channel (discontinuous length fixed to 1
                                       rank). Caution: this setting impacts the entire injected
                                       group. Therefore, call of HAL_ADCEx_InjectedConfigChannel()
                                       to configure a channel on injected group can impact the
                                       configuration of other channels previously set. */

        FunctionalState
          AutoInjectedConv; /*!< Enables or disables the selected ADC group injected automatic
                               conversion after regular one This parameter can be set to ENABLE or
                               DISABLE. Note: To use Automatic injected conversion, discontinuous
                               mode must be disabled ('DiscontinuousConvMode' and
                               'InjectedDiscontinuousConvMode' set to DISABLE) Note: To use
                               Automatic injected conversion, injected group external triggers must
                               be disabled ('ExternalTrigInjecConv' set to
                               ADC_INJECTED_SOFTWARE_START) Note: In case of DMA used with regular
                               group: if DMA configured in normal mode (single shot) JAUTO will be
                               stopped upon DMA transfer complete. To maintain JAUTO always enabled,
                               DMA must be configured in circular mode. Caution: this setting
                               impacts the entire injected group. Therefore, call of
                               HAL_ADCEx_InjectedConfigChannel() to configure a channel on injected
                               group can impact the configuration of other channels previously set.
                             */

        FunctionalState
          QueueInjectedContext; /*!< Specifies whether the context queue feature is enabled.
                                     This parameter can be set to ENABLE or DISABLE.
                                     If context queue is enabled, injected sequencer&channels
                                   configurations are queued on up to 2 contexts. If a new injected
                                   context is set when queue is full, error is triggered by
                                   interruption and through function
                                     'HAL_ADCEx_InjectedQueueOverflowCallback'.
                                     Caution: This feature request that the sequence is fully
                                   configured before injected conversion start. Therefore, configure
                                   channels with as many calls to HAL_ADCEx_InjectedConfigChannel()
                                   as the 'InjectedNbrOfConversion' parameter. Caution: this setting
                                   impacts the entire injected group. Therefore, call of
                                   HAL_ADCEx_InjectedConfigChannel() to configure a channel on
                                   injected group can impact the configuration of other channels
                                   previously set. Note: This parameter must be modified when ADC is
                                   disabled (before ADC start conversion or after ADC stop
                                   conversion). */

        uint32_t
          ExternalTrigInjecConv; /*!< Selects the external event used to trigger the conversion
                                    start of injected group. If set to ADC_INJECTED_SOFTWARE_START,
                                    external triggers are disabled and software trigger is used
                                    instead. This parameter can be a value of @ref
                                    ADC_injected_external_trigger_source. Caution: this setting
                                    impacts the entire injected group. Therefore, call of
                                    HAL_ADCEx_InjectedConfigChannel() to configure a channel on
                                    injected group can impact the configuration of other channels
                                    previously set. */

        uint32_t
          ExternalTrigInjecConvEdge; /*!< Selects the external trigger edge of injected group.
                                          This parameter can be a value of @ref
                                        ADC_injected_external_trigger_edge. If trigger source is set
                                        to ADC_INJECTED_SOFTWARE_START, this parameter is discarded.
                                          Caution: this setting impacts the entire injected group.
                                        Therefore, call of HAL_ADCEx_InjectedConfigChannel() to
                                                   configure a channel on injected group can impact
                                        the configuration of other channels previously set. */

        FunctionalState InjecOversamplingMode; /*!< Specifies whether the oversampling feature is
                                                  enabled or disabled. This parameter can be set to
                                                  ENABLE or DISABLE. Note: This parameter can be
                                                  modified only if there is no conversion is ongoing
                                                  (both ADSTART and JADSTART cleared). */

        ADC_InjOversamplingTypeDef
          InjecOversampling; /*!< Specifies the Oversampling parameters.
                                  Caution: this setting overwrites the previous oversampling
                                configuration if oversampling already enabled. Note: This parameter
                                can be modified only if there is no conversion is ongoing (both
                                ADSTART and JADSTART cleared). */
    };

#if defined(ADC_MULTIMODE_SUPPORT)
    /**
     * @brief  Structure definition of ADC multimode
     * @note   The setting of these parameters by function HAL_ADCEx_MultiModeConfigChannel() is
     * conditioned by ADCs state (both Master and Slave ADCs). Both Master and Slave ADCs must be
     * disabled.
     */
    struct ADC_MultiModeTypeDef
    {
        uint32_t Mode; /*!< Configures the ADC to operate in independent or multimode.
                            This parameter can be a value of @ref ADC_HAL_EC_MULTI_MODE. */

        uint32_t DMAAccessMode; /*!< Configures the DMA mode for multimode ADC:
                                     selection whether 2 DMA channels (each ADC uses its own DMA
                                   channel) or 1 DMA channel (one DMA channel for both ADC, DMA of
                                   ADC master) This parameter can be a value of @ref
                                   ADC_HAL_EC_MULTI_DMA_TRANSFER_RESOLUTION. */

        uint32_t TwoSamplingDelay; /*!< Configures the Delay between 2 sampling phases.
                                        This parameter can be a value of @ref
                                      ADC_HAL_EC_MULTI_TWOSMP_DELAY. Delay range depends on selected
                                      resolution: from 1 to 12 clock cycles for 12 bits, from 1 to
                                      10 clock cycles for 10 bits, from 1 to 8 clock cycles for 8
                                      bits, from 1 to 6 clock cycles for 6 bits.     */
    };
#endif /* ADC_MULTIMODE_SUPPORT */

    /**
     * @}
     */

    /* Exported constants --------------------------------------------------------*/


    /* Exported functions --------------------------------------------------------*/
    /** @addtogroup ADCEx_Exported_Functions
     * @{
     */

    /** @addtogroup ADCEx_Exported_Functions_Group1
     * @{
     */
    /* IO operation functions *****************************************************/

    /* ADC calibration */
    HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc, uint32_t SingleDiff);
    uint32_t          HAL_ADCEx_Calibration_GetValue(ADC_HandleTypeDef* hadc, uint32_t SingleDiff);
    HAL_StatusTypeDef HAL_ADCEx_Calibration_SetValue(ADC_HandleTypeDef* hadc,
                                                     uint32_t           SingleDiff,
                                                     uint32_t           CalibrationFactor);

    /* Blocking mode: Polling */
    HAL_StatusTypeDef HAL_ADCEx_InjectedStart(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_InjectedStop(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_InjectedPollForConversion(ADC_HandleTypeDef* hadc,
                                                          uint32_t           Timeout);

    /* Non-blocking mode: Interruption */
    HAL_StatusTypeDef HAL_ADCEx_InjectedStart_IT(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_InjectedStop_IT(ADC_HandleTypeDef* hadc);

#if defined(ADC_MULTIMODE_SUPPORT)
    /* ADC multimode */
    HAL_StatusTypeDef HAL_ADCEx_MultiModeStart_DMA(ADC_HandleTypeDef* hadc,
                                                   uint32_t*          pData,
                                                   uint32_t           Length);
    HAL_StatusTypeDef HAL_ADCEx_MultiModeStop_DMA(ADC_HandleTypeDef* hadc);
    uint32_t          HAL_ADCEx_MultiModeGetValue(ADC_HandleTypeDef* hadc);
#endif /* ADC_MULTIMODE_SUPPORT */

    /* ADC retrieve conversion value intended to be used with polling or interruption */
    uint32_t HAL_ADCEx_InjectedGetValue(ADC_HandleTypeDef* hadc, uint32_t InjectedRank);

    /* ADC IRQHandler and Callbacks used in non-blocking modes (Interruption) */
    void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc);
    void HAL_ADCEx_InjectedQueueOverflowCallback(ADC_HandleTypeDef* hadc);
    void HAL_ADCEx_LevelOutOfWindow2Callback(ADC_HandleTypeDef* hadc);
    void HAL_ADCEx_LevelOutOfWindow3Callback(ADC_HandleTypeDef* hadc);
    void HAL_ADCEx_EndOfSamplingCallback(ADC_HandleTypeDef* hadc);

    /* ADC group regular conversions stop */
    HAL_StatusTypeDef HAL_ADCEx_RegularStop(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_RegularStop_IT(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_RegularStop_DMA(ADC_HandleTypeDef* hadc);
#if defined(ADC_MULTIMODE_SUPPORT)
    HAL_StatusTypeDef HAL_ADCEx_RegularMultiModeStop_DMA(ADC_HandleTypeDef* hadc);
#endif /* ADC_MULTIMODE_SUPPORT */

    /**
     * @}
     */

    /** @addtogroup ADCEx_Exported_Functions_Group2
     * @{
     */
    /* Peripheral Control functions ***********************************************/
    HAL_StatusTypeDef HAL_ADCEx_InjectedConfigChannel(ADC_HandleTypeDef*        hadc,
                                                      ADC_InjectionConfTypeDef* sConfigInjected);
#if defined(ADC_MULTIMODE_SUPPORT)
    HAL_StatusTypeDef HAL_ADCEx_MultiModeConfigChannel(ADC_HandleTypeDef*    hadc,
                                                       ADC_MultiModeTypeDef* multimode);
#endif /* ADC_MULTIMODE_SUPPORT */
    HAL_StatusTypeDef HAL_ADCEx_EnableInjectedQueue(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_DisableInjectedQueue(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_DisableVoltageRegulator(ADC_HandleTypeDef* hadc);
    HAL_StatusTypeDef HAL_ADCEx_EnterADCDeepPowerDownMode(ADC_HandleTypeDef* hadc);

    /**
     * @}
     */

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

#endif /* STM32L4xx_HAL_ADC_EX_H */
