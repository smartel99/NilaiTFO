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

#include "../../../defines/internal_config.h"
#include NILAI_HAL_HEADER

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

extern "C"
{
    struct ADC_TypeDef
    {
        uint32_t data;
    };

    extern ADC_TypeDef ADC1;

    struct ADC_HandleTypeDef
    {
        ADC_TypeDef* Instance = nullptr;
    };

    HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* adc, uint32_t* data, uint32_t size);
    HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* adc);

    uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* adc);
    uint32_t HAL_ADC_GetError(ADC_HandleTypeDef* adc);
}

#endif    // GUARD_NILAI_TEST_MOCK_DRIVER_ADC_H
