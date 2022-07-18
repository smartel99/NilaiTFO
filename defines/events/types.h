/**
 * @file    Types.h
 * @author  Samuel Martel
 * @date    2022-05-02
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
#ifndef NILAI_EVENT_TYPES_H
#define NILAI_EVENT_TYPES_H


#if defined(NILAI_USE_EVENTS)
namespace Nilai::Events
{
/**
 * @brief List of the possible events.
 */
enum class EventTypes
{
    Exti_Generic = 0,    //! Generic EXTI event.
    Exti0,               //! External Event on pin 0 AND/OR Software-triggered event.
    Exti1,               //! External Event on pin 1 AND/OR Software-triggered event.
    Exti2,               //! External Event on pin 2 AND/OR Software-triggered event.
    Exti3,               //! External Event on pin 3 AND/OR Software-triggered event.
    Exti4,               //! External Event on pin 4 AND/OR Software-triggered event.
    Exti5,               //! External Event on pin 5 AND/OR Software-triggered event.
    Exti6,               //! External Event on pin 6 AND/OR Software-triggered event.
    Exti7,               //! External Event on pin 7 AND/OR Software-triggered event.
    Exti8,               //! External Event on pin 8 AND/OR Software-triggered event.
    Exti9,               //! External Event on pin 9 AND/OR Software-triggered event.
    Exti10,              //! External Event on pin 10 AND/OR Software-triggered event.
    Exti11,              //! External Event on pin 11 AND/OR Software-triggered event.
    Exti12,              //! External Event on pin 12 AND/OR Software-triggered event.
    Exti13,              //! External Event on pin 13 AND/OR Software-triggered event.
    Exti14,              //! External Event on pin 14 AND/OR Software-triggered event.
    Exti15,              //! External Event on pin 15 AND/OR Software-triggered event.

    DataEvent,    //! A software event carrying data.

    UserEvent1,
    UserEvent2,
    UserEvent3,
    UserEvent4,
    UserEvent5,
    UserEvent6,
    UserEvent7,
    UserEvent8,
    UserEvent9,
    UserEvent10,

#    if defined(NILAI_USE_TIMER_EVENTS)
    //----------------------------------------------------------------------------------------------
    // Timer Events.
    Tim_Generic,              //! Generic Timer Event.
    Tim_PeriodElapsed,        //! Period of the timer is now completed.
    Tim_OC_DelayElapsed,      //! Output compare callback in non blocking mode.
    Tim_IC_Capture,           //! Input capture in non blocking mode.
    Tim_PWM_PulseFinished,    //! PWM pulse finished callback in non blocking mode.
    Tim_Trigger,              //! Hall trigger detection callback in non blocking mode.
    Tim_Error,                //! Timer error callback in non blocking mode.
#    endif

#    if defined(NILAI_USE_I2C_EVENTS)
    I2C_Generic,         //! Generic I2C Event.
    I2C_MasterTxCplt,    //! Transmission complete in master mode.
    I2C_MasterRxCplt,    //! Reception complete in master mode.
    I2C_SlaveTxCplt,     //! Transmission complete in slave mode.
    I2C_SlaveRxCplt,     //! Reception complete in slave mode.
    I2C_Addr,            //! Received data request.
    I2C_ListenCplt,      //! TODO
    I2C_MemTxCplt,       //! Write to register complete.
    I2C_MemRxCplt,       //! Read from register complete.
    I2C_Error,           //! Error detected.
    I2C_AbortCplt,       //! Abort request complete.
#    endif

#    if defined(NILAI_USE_I2S_EVENTS)
    I2S_Generic,       //! Generic I2S Event.
    I2S_TxHalfCplt,    //! Half of the data has been sent.
    I2S_TxCplt,        //! All of the data has been sent.
    I2S_RxHalfCplt,    //! Half of the data has been received.
    I2S_RxCplt,        //! All of the data has been received.
    I2S_Error,         //! An error occurred.
#    endif

#    if defined(NILAI_USE_RTC_EVENTS)
    RTC_Generic,        //! Generic RTC Event.
    RTC_AlarmAEvent,    //! Alarm A event.
// TODO Might have more events depending on the MCU.
#    endif

#    if defined(NILAI_USE_SPI_EVENTS)
    SPI_Generic,         //! Generic SPI Event.
    SPI_TxCplt,          //! Data transfer complete.
    SPI_RxCplt,          //! Data reception complete.
    SPI_TxRxCplt,        //! Data transaction complete.
    SPI_TxHalfCplt,      //! Data transfer half-complete.
    SPI_RxHalfCplt,      //! Data reception half-complete.
    SPI_TxRxHalfCplt,    //! Data transaction half-complete.
    SPI_Error,           //! An error occurred.
    SPI_AbortCplt,       //! Abortion request complete.
#    endif

#    if defined(NILAI_USE_UART_EVENTS)
    UART_Generic,        //! Generic UART event
    UART_TxHalfCplt,     //! Data transfer half complete.
    UART_TxCplt,         //! Data transfer complete.
    UART_RxHalfCplt,     //! Data reception half complete.
    UART_RxCplt,         //! Data reception complete.
    UART_Error,          //! An error occurred.
    UART_AbortCplt,      //! Abortion request complete.
    UART_AbortTxCplt,    //! Transmission abort request complete.
    UART_AbortRxCplt,    //! Reception abort request complete.
    UART_RxEvent,        //! Reception event.
#    endif

#    if defined(NILAI_USE_ADC_EVENTS)
    ADC_Generic,             //! Generic ADC event.
    ADC_ConvCplt,            //! Conversion sequence complete.
    ADC_ConvHalfCplt,        //! Conversion sequence half complete.
    ADC_LevelOutOfWindow,    //! Voltage read is outside the tolerances.
    ADC_Error,               //! An error occurred.
#    endif

#    if defined(NILAI_USE_CAN_EVENTS)
    CAN_Generic,                //! Generic CAN event.
    CAN_TxMailbox0Cplt,         //! Transmit operation in mailbox 0 complete.
    CAN_TxMailbox1Cplt,         //! Transmit operation in mailbox 1 complete.
    CAN_TxMailbox2Cplt,         //! Transmit operation in mailbox 2 complete.
    CAN_TxMailbox0AbortCplt,    //! Abortion request complete in mailbox 0.
    CAN_TxMailbox1AbortCplt,    //! Abortion request complete in mailbox 1.
    CAN_TxMailbox2AbortCplt,    //! Abortion request complete in mailbox 2.
    CAN_RxFifo0MsgPending,      //! A CAN packet has been received in FIFO 0.
    CAN_RxFifo0Full,            //! No more packets can be received in FIFO 0.
    CAN_RxFifo1MsgPending,      //! A CAN packet has been received in FIFO 1.
    CAN_RxFifo1Full,            //! No more packets can be received in FIFO 1.
    CAN_Sleep,                  //! The CAN peripheral successfully went to sleep.
    CAN_WakeUpFromRx,           //! The CAN peripheral has been awaken by the reception of a packet.
    CAN_Error,                  //! An error occurred.
#    endif

#    if defined(NILAI_USE_SAI_EVENTS)
    SAI_Generic,       //! Generic SAI event.
    SAI_TxHalfCplt,    //! Transmission half complete.
    SAI_TxCplt,        //! Transmission complete.
    SAI_RxHalfCplt,    //! Reception half complete.
    SAI_RxCplt,        //! Reception complete.
    SAI_Error,         //! An error occurred.
#    endif

    //! Number of events, should always be last.
    Count
};

enum class EventCategories
{
    External = 0,    //! External Event.
    UserEvent,
    Data,    //! Data Event.
#    if defined(NILAI_USE_TIMER_EVENTS)
    Timer,    //! Timer Event.
#    endif
#    if defined(NILAI_USE_I2C_EVENTS)
    I2C,    //! I2C Event.
#    endif
#    if defined(NILAI_USE_I2S_EVENTS)
    I2S,    //! I2S Event.
#    endif
#    if defined(NILAI_USE_RTC_EVENTS)
    Rtc,    //! RTC Event.
#    endif
#    if defined(NILAI_USE_SPI_EVENTS)
    Spi,    //! SPI Event.
#    endif
#    if defined(NILAI_USE_UART_EVENTS)
    Uart,    //! UART Event.
#    endif
#    if defined(NILAI_USE_ADC_EVENTS)
    Adc,    //! ADC Event.
#    endif
#    if defined(NILAI_USE_CAN_EVENTS)
    Can,    //! CAN Event.
#    endif
#    if defined(NILAI_USE_SAI_EVENTS)
    Sai,    //! SAI Event.
#    endif

    //! Number of Categories, should always be last.
    Count
};

inline constexpr bool IsInCategory(EventTypes t, EventCategories c)
{
    EventCategories typeCategory = EventCategories::Count;
    switch (t)
    {
        case EventTypes::Exti_Generic:
        case EventTypes::Exti0:
        case EventTypes::Exti1:
        case EventTypes::Exti2:
        case EventTypes::Exti3:
        case EventTypes::Exti4:
        case EventTypes::Exti5:
        case EventTypes::Exti6:
        case EventTypes::Exti7:
        case EventTypes::Exti8:
        case EventTypes::Exti9:
        case EventTypes::Exti10:
        case EventTypes::Exti11:
        case EventTypes::Exti12:
        case EventTypes::Exti13:
        case EventTypes::Exti14:
        case EventTypes::Exti15: typeCategory = EventCategories::External; break;

        case EventTypes::DataEvent: typeCategory = EventCategories::Data; break;

        case EventTypes::UserEvent1:
        case EventTypes::UserEvent2:
        case EventTypes::UserEvent3:
        case EventTypes::UserEvent4:
        case EventTypes::UserEvent5:
        case EventTypes::UserEvent6:
        case EventTypes::UserEvent7:
        case EventTypes::UserEvent8:
        case EventTypes::UserEvent9:
        case EventTypes::UserEvent10: typeCategory = EventCategories::UserEvent; break;

#    if defined(NILAI_USE_TIMER_EVENTS)
        case EventTypes::Tim_Generic:
        case EventTypes::Tim_PeriodElapsed:
        case EventTypes::Tim_OC_DelayElapsed:
        case EventTypes::Tim_IC_Capture:
        case EventTypes::Tim_PWM_PulseFinished:
        case EventTypes::Tim_Trigger:
        case EventTypes::Tim_Error: typeCategory = EventCategories::Timer; break;
#    endif

#    if defined(NILAI_USE_I2C_EVENTS)
        case EventTypes::I2C_Generic:
        case EventTypes::I2C_MasterTxCplt:
        case EventTypes::I2C_MasterRxCplt:
        case EventTypes::I2C_SlaveTxCplt:
        case EventTypes::I2C_SlaveRxCplt:
        case EventTypes::I2C_Addr:
        case EventTypes::I2C_ListenCplt:
        case EventTypes::I2C_MemTxCplt:
        case EventTypes::I2C_MemRxCplt:
        case EventTypes::I2C_Error:
        case EventTypes::I2C_AbortCplt: typeCategory = EventCategories::I2C; break;
#    endif

#    if defined(NILAI_USE_I2S_EVENTS)
        case EventTypes::I2S_Generic:
        case EventTypes::I2S_TxHalfCplt:
        case EventTypes::I2S_TxCplt:
        case EventTypes::I2S_RxHalfCplt:
        case EventTypes::I2S_RxCplt:
        case EventTypes::I2S_Error: typeCategory = EventCategories::I2S; break;
#    endif

#    if defined(NILAI_USE_RTC_EVENTS)
        case EventTypes::RTC_Generic:
        case EventTypes::RTC_AlarmAEvent: typeCategory = EventCategories::Rtc; break;
#    endif

#    if defined(NILAI_USE_SPI_EVENTS)
        case EventTypes::SPI_Generic:
        case EventTypes::SPI_TxCplt:
        case EventTypes::SPI_RxCplt:
        case EventTypes::SPI_TxRxCplt:
        case EventTypes::SPI_TxHalfCplt:
        case EventTypes::SPI_RxHalfCplt:
        case EventTypes::SPI_TxRxHalfCplt:
        case EventTypes::SPI_Error:
        case EventTypes::SPI_AbortCplt: typeCategory = EventCategories::Spi; break;
#    endif

#    if defined(NILAI_USE_UART_EVENTS)
        case EventTypes::UART_Generic:
        case EventTypes::UART_TxHalfCplt:
        case EventTypes::UART_TxCplt:
        case EventTypes::UART_RxHalfCplt:
        case EventTypes::UART_RxCplt:
        case EventTypes::UART_Error:
        case EventTypes::UART_AbortCplt:
        case EventTypes::UART_AbortTxCplt:
        case EventTypes::UART_AbortRxCplt:
        case EventTypes::UART_RxEvent: typeCategory = EventCategories::Uart; break;
#    endif

#    if defined(NILAI_USE_ADC_EVENTS)
        case EventTypes::ADC_Generic:
        case EventTypes::ADC_ConvCplt:
        case EventTypes::ADC_ConvHalfCplt:
        case EventTypes::ADC_LevelOutOfWindow:
        case EventTypes::ADC_Error: typeCategory = EventCategories::Adc; break;
#    endif

#    if defined(NILAI_USE_CAN_EVENTS)
        case EventTypes::CAN_Generic:
        case EventTypes::CAN_TxMailbox0Cplt:
        case EventTypes::CAN_TxMailbox1Cplt:
        case EventTypes::CAN_TxMailbox2Cplt:
        case EventTypes::CAN_TxMailbox0AbortCplt:
        case EventTypes::CAN_TxMailbox1AbortCplt:
        case EventTypes::CAN_TxMailbox2AbortCplt:
        case EventTypes::CAN_RxFifo0MsgPending:
        case EventTypes::CAN_RxFifo0Full:
        case EventTypes::CAN_RxFifo1MsgPending:
        case EventTypes::CAN_RxFifo1Full:
        case EventTypes::CAN_Sleep:
        case EventTypes::CAN_WakeUpFromRx:
        case EventTypes::CAN_Error: typeCategory = EventCategories::Can; break;
#    endif

#    if defined(NILAI_USE_SAI_EVENTS)
        case EventTypes::SAI_Generic:
        case EventTypes::SAI_TxHalfCplt:
        case EventTypes::SAI_TxCplt:
        case EventTypes::SAI_RxHalfCplt:
        case EventTypes::SAI_RxCplt:
        case EventTypes::SAI_Error: typeCategory = EventCategories::Sai; break;
#    endif

        case EventTypes::Count:
        default: typeCategory = EventCategories::Count; break;
    }

    return c == typeCategory;
}
}    // namespace Nilai::Events
#endif

#endif    // NILAI_EVENT_TYPES_H
