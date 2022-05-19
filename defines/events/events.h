/**
 * @file    Events.h
 * @author  Samuel Martel
 * @date    2022-03-03
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/<a/>.
 */
#ifndef NILAI_EVENTS_H
#define NILAI_EVENTS_H

#if defined(NILAI_USE_EVENTS)

#    include "../internal_config.h"
#    include "data_event.h"
#    include "ext_event.h"
#    include "generic_event.h"
#    include "types.h"

#    if defined(NILAI_USE_ADC_EVENTS)
#        include "adc_event.h"
#    endif

#    if defined(NILAI_USE_TIMER_EVENTS)
#        include "tim_event.h"
#    endif

#    if defined(NILAI_USE_I2C_EVENTS)
#        include "i2c_event.h"
#    endif

#    if defined(NILAI_USE_I2S_EVENTS)
#        include "i2s_event.h"
#    endif

#    if defined(NILAI_USE_RTC_EVENTS)
#        include "rtc_event.h"
#    endif

#    if defined(NILAI_USE_SPI_EVENTS)
#        include "spi_event.h"
#    endif

#    if defined(NILAI_USE_UART_EVENTS)
#        include "uart_event.h"
#    endif

#    if defined(NILAI_USE_CAN_EVENTS)
#        include "can_event.h"
#    endif

#    if defined(NILAI_USE_SAI_EVENTS)
#        include "sai_event.h"
#    endif

#    include "../pin.h"


namespace Nilai::Events
{
EventTypes PinToEvent(const Nilai::Pin& p);

}    // namespace Nilai::Events
#endif

#endif    // NILAI_EVENTS_H
