/**
 * @file    adc.h
 * @author  Samuel Martel
 * @date    2022-10-13
 * @brief
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */

#ifndef GUARD_NILAI_TEST_MOCK_DRIVER_ADC_H
#define GUARD_NILAI_TEST_MOCK_DRIVER_ADC_H

#include "../generic_stm32.h"

#include <cstdint>

#define HAL_ADC_ERROR_NONE 0x00U /*!< No error                                              */
#define HAL_ADC_ERROR_INTERNAL                                                                     \
    0x01U                       /*!< ADC IP internal error: if problem of clocking,                \
                                     enable/disable, erroneous state                       */
#define HAL_ADC_ERROR_OVR 0x02U /*!< Overrun error                                         */
#define HAL_ADC_ERROR_DMA 0x04U /*!< DMA transfer error                                    */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
#    define HAL_ADC_ERROR_INVALID_CALLBACK (0x10U) /*!< Invalid Callback error */
#endif                                             /* USE_HAL_ADC_REGISTER_CALLBACKS */

using ADCIoBuffer = IoBuffer<uint32_t, 20>;

struct ADC_TypeDef
{
};

struct ADC_InitTypeDef
{
    uint32_t NbrOfConversion = 0;
};

struct ADC_HandleTypeDef
{
    size_t          id       = -1;
    ADC_TypeDef*    Instance = nullptr;
    ADC_InitTypeDef Init;

    ADC_HandleTypeDef() : Instance(new ADC_TypeDef) {}
    ~ADC_HandleTypeDef()
    {
        delete Instance;
        Instance = nullptr;
    }
};

HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* adc, uint32_t* data, uint32_t size);
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* adc);

uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* adc);
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef* adc);

void NILAI_ADC_Init(ADC_HandleTypeDef* adc, size_t chCount);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
static_assert(false, "Not implemented");
/**
 * @brief  HAL ADC Callback ID enumeration definition
 */
typedef enum
{
    HAL_ADC_CONVERSION_COMPLETE_CB_ID = 0x00U, /*!< ADC conversion complete callback ID */
    HAL_ADC_CONVERSION_HALF_CB_ID     = 0x01U, /*!< ADC conversion DMA half-transfer callback ID */
    HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID = 0x02U, /*!< ADC analog watchdog 1 callback ID */
    HAL_ADC_ERROR_CB_ID                 = 0x03U, /*!< ADC error callback ID */
    HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID =
      0x04U,                         /*!< ADC group injected conversion complete callback ID */
    HAL_ADC_MSPINIT_CB_ID   = 0x05U, /*!< ADC Msp Init callback ID          */
    HAL_ADC_MSPDEINIT_CB_ID = 0x06U  /*!< ADC Msp DeInit callback ID        */
} HAL_ADC_CallbackIDTypeDef;

/**
 * @brief  HAL ADC Callback pointer definition
 */
typedef void (*pADC_CallbackTypeDef)(
  ADC_HandleTypeDef* hadc); /*!< pointer to a ADC callback function */

HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef*        hadc,
                                           HAL_ADC_CallbackIDTypeDef CallbackID,
                                           pADC_CallbackTypeDef      pCallback);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

#endif    // GUARD_NILAI_TEST_MOCK_DRIVER_ADC_H
