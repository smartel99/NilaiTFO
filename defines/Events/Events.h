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

#    include "DataEvent.h"
#    include "ExtEvent.h"
#    include "GenericEvent.h"
#    include "Types.h"

#    if defined(NILAI_USE_ADC_EVENTS)
#        include "AdcEvent.h"
#    endif

#    if defined(NILAI_USE_TIMER_EVENTS)
#        include "TimEvent.h"
#    endif

#    if defined(NILAI_USE_I2C_EVENTS)
#        include "I2cEvent.h"
#    endif

#    if defined(NILAI_USE_I2S_EVENTS)
#        include "I2sEvents.h"
#    endif

#    if defined(NILAI_USE_RTC_EVENTS)
#        include "RtcEvent.h"
#    endif

#    if defined(NILAI_USE_SPI_EVENTS)
#        include "SpiEvent.h"
#    endif

#    if defined(NILAI_USE_UART_EVENTS)
#        include "UartEvent.h"
#    endif

#    include "../pin.h"


namespace cep::Events
{
EventTypes PinToEvent(const cep::Pin& p);

}    // namespace cep::Events
#endif

#endif    // NILAI_EVENTS_H
