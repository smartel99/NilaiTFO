/**
 * @file    adc.cpp
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
#include "adc.h"
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* adc, uint32_t* data, uint32_t size)
{
    return HAL_BUSY;
}
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* adc)
{
    return HAL_BUSY;
}
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* adc)
{
    return 0;
}
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef* adc)
{
    return 0;
}
void NILAI_ADC_Init(ADC_HandleTypeDef* adc, size_t chCount)
{
}
