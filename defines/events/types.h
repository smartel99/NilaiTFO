/**
 * @file    types.h
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
 * not, see <a href=https://www.gnu.org/licenses/>https://www.gnu.org/licenses/</a>.
 */
#ifndef NILAI_EVENT_TYPES_H
#define NILAI_EVENT_TYPES_H


#if defined(NILAI_USE_EVENTS)
namespace Nilai::Events
{
/**
 * @addtogroup Nilai
 * @{
 */

/**
 * @addtogroup Events
 * @{
 */

/**
 * @enum EventTypes
 * @brief List of the possible events.
 */
enum class EventTypes
{
    Exti_Generic = 0,    //!< Generic EXTI event.
    Exti0,               //!< External Event on pin 0 AND/OR Software-triggered event.
    Exti1,               //!< External Event on pin 1 AND/OR Software-triggered event.
    Exti2,               //!< External Event on pin 2 AND/OR Software-triggered event.
    Exti3,               //!< External Event on pin 3 AND/OR Software-triggered event.
    Exti4,               //!< External Event on pin 4 AND/OR Software-triggered event.
    Exti5,               //!< External Event on pin 5 AND/OR Software-triggered event.
    Exti6,               //!< External Event on pin 6 AND/OR Software-triggered event.
    Exti7,               //!< External Event on pin 7 AND/OR Software-triggered event.
    Exti8,               //!< External Event on pin 8 AND/OR Software-triggered event.
    Exti9,               //!< External Event on pin 9 AND/OR Software-triggered event.
    Exti10,              //!< External Event on pin 10 AND/OR Software-triggered event.
    Exti11,              //!< External Event on pin 11 AND/OR Software-triggered event.
    Exti12,              //!< External Event on pin 12 AND/OR Software-triggered event.
    Exti13,              //!< External Event on pin 13 AND/OR Software-triggered event.
    Exti14,              //!< External Event on pin 14 AND/OR Software-triggered event.
    Exti15,              //!< External Event on pin 15 AND/OR Software-triggered event.

    DataEvent,    //!< A software event carrying data.

    UserEvent1,     //!< The 1st user defined event.
    UserEvent2,     //!< The 2nd user defined event.
    UserEvent3,     //!< The 3rd user defined event.
    UserEvent4,     //!< The 4th user defined event.
    UserEvent5,     //!< The 5th user defined event.
    UserEvent6,     //!< The 6th user defined event.
    UserEvent7,     //!< The 7th user defined event.
    UserEvent8,     //!< The 8th user defined event.
    UserEvent9,     //!< The 9th user defined event.
    UserEvent10,    //!< The 10th user defined event.

#    if defined(NILAI_USE_TIMER_EVENTS)
    //----------------------------------------------------------------------------------------------
    // Timer Events.
    /**
     * @brief Generic Timer Event.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_Generic,
    /**
     * @brief Period of the timer is now completed.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_PeriodElapsed,
    /**
     * @brief Output compare callback in non blocking mode.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_OC_DelayElapsed,
    /**
     * @brief Input capture in non blocking mode.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_IC_Capture,
    /**
     * @brief PWM pulse finished callback in non blocking mode.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_PWM_PulseFinished,
    /**
     * @brief Hall trigger detection callback in non blocking mode.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_Trigger,
    /**
     * @brief Timer error callback in non blocking mode.
     * @note Only present if @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Tim_Error,
#    endif

#    if defined(NILAI_USE_I2C_EVENTS)
    /**
     * @brief Generic I2C Event.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_Generic,
    /**
     * @brief Transmission complete in master mode.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_MasterTxCplt,
    /**
     * @brief Reception complete in master mode.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_MasterRxCplt,
    /**
     * @brief Transmission complete in slave mode.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_SlaveTxCplt,
    /**
     * @brief Reception complete in slave mode.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_SlaveRxCplt,
    /**
     * @brief Received data request.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_Addr,
    /**
     * @brief TODO
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_ListenCplt,
    /**
     * @brief Write to register complete.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_MemTxCplt,
    /**
     * @brief Read from register complete.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_MemRxCplt,
    /**
     * @brief Error detected.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_Error,
    /**
     * @brief Abort request complete.
     * @note Only present if @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C_AbortCplt,
#    endif

#    if defined(NILAI_USE_I2S_EVENTS)
    /**
     * @brief Generic I2S Event.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    I2S_Generic,
    /**
     * @brief Half of the data has been sent.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    I2S_TxHalfCplt,
    /**
     * @brief All of the data has been sent.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    I2S_TxCplt,
    /**
     * @brief Half of the data has been received.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    I2S_RxHalfCplt,
    /**
     * @brief All of the data has been received.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    I2S_RxCplt,
    /**
     * @brief An error occurred.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    I2S_Error,
#    endif

#    if defined(NILAI_USE_RTC_EVENTS)
    /**
     * @brief Generic RTC Event.
     * @note Only present if @ref NILAI_USE_RTC_EVENTS is defined.
     */
    RTC_Generic,
    /**
     * @brief Alarm A event.
     * @note Only present if @ref NILAI_USE_RTC_EVENTS is defined.
     */
    RTC_AlarmAEvent,
// TODO Might have more events depending on the MCU.
#    endif

#    if defined(NILAI_USE_SPI_EVENTS)
    /**
     * @brief Generic SPI Event.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_Generic,
    /**
     * @brief Data transfer complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_TxCplt,
    /**
     * @brief Data reception complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_RxCplt,
    /**
     * @brief Data transaction complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_TxRxCplt,
    /**
     * @brief Data transfer half-complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_TxHalfCplt,
    /**
     * @brief Data reception half-complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_RxHalfCplt,
    /**
     * @brief Data transaction half-complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_TxRxHalfCplt,
    /**
     * @brief An error occurred.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_Error,
    /**
     * @brief Abortion request complete.
     * @note Only present if @ref NILAI_USE_SPI_EVENTS is defined.
     */
    SPI_AbortCplt,
#    endif

#    if defined(NILAI_USE_UART_EVENTS)
    /**
     * @brief Generic UART event.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_Generic,
    /**
     * @brief Data transfer half complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_TxHalfCplt,
    /**
     * @brief Data transfer complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_TxCplt,
    /**
     * @brief Data reception half complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_RxHalfCplt,
    /**
     * @brief Data reception complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_RxCplt,
    /**
     * @brief An error occurred.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_Error,
    /**
     * @brief Abortion request complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_AbortCplt,
    /**
     * @brief Transmission abort request complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_AbortTxCplt,
    /**
     * @brief Reception abort request complete.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_AbortRxCplt,
    /**
     * @brief Reception event.
     * @note Only present if @ref NILAI_USE_UART_EVENTS is defined.
     */
    UART_RxEvent,
#    endif

#    if defined(NILAI_USE_ADC_EVENTS)
    /**
     * @brief Generic ADC event.
     * @note Only present if @ref NILAI_USE_ADC_EVENTS is defined.
     */
    ADC_Generic,
    /**
     * @brief Conversion sequence complete.
     * @note Only present if @ref NILAI_USE_ADC_EVENTS is defined.
     */
    ADC_ConvCplt,
    /**
     * @brief Conversion sequence half complete.
     * @note Only present if @ref NILAI_USE_ADC_EVENTS is defined.
     */
    ADC_ConvHalfCplt,
    /**
     * @brief Voltage read is outside the tolerances.
     * @note Only present if @ref NILAI_USE_ADC_EVENTS is defined.
     */
    ADC_LevelOutOfWindow,
    /**
     * @brief An error occurred.
     * @note Only present if @ref NILAI_USE_ADC_EVENTS is defined.
     */
    ADC_Error,
#    endif

#    if defined(NILAI_USE_CAN_EVENTS)
    /**
     * @brief Generic CAN event.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_Generic,
    /**
     * @brief Transmit operation in mailbox 0 complete.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_TxMailbox0Cplt,
    /**
     * @brief Transmit operation in mailbox 1 complete.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_TxMailbox1Cplt,
    /**
     * @brief Transmit operation in mailbox 2 complete.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_TxMailbox2Cplt,
    /**
     * @brief Abortion request complete in mailbox 0.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_TxMailbox0AbortCplt,
    /**
     * @brief Abortion request complete in mailbox 1.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_TxMailbox1AbortCplt,
    /**
     * @brief Abortion request complete in mailbox 2.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_TxMailbox2AbortCplt,
    /**
     * @brief A CAN packet has been received in FIFO 0.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_RxFifo0MsgPending,
    /**
     * @brief No more packets can be received in FIFO 0.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_RxFifo0Full,
    /**
     * @brief A CAN packet has been received in FIFO 1.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_RxFifo1MsgPending,
    /**
     * @brief No more packets can be received in FIFO 1.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_RxFifo1Full,
    /**
     * @brief The CAN peripheral successfully went to sleep.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_Sleep,
    /**
     * @brief The CAN peripheral has been awaken by the reception of a packet.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_WakeUpFromRx,
    /**
     * @brief An error occurred.
     * @note Only present if @ref NILAI_USE_CAN_EVENTS is defined.
     */
    CAN_Error,
#    endif

#    if defined(NILAI_USE_SAI_EVENTS)
    /**
     * @brief Generic SAI event.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    SAI_Generic,
    /**
     * @brief Transmission half complete.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    SAI_TxHalfCplt,
    /**
     * @brief Transmission complete.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    SAI_TxCplt,
    /**
     * @brief Reception half complete.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    SAI_RxHalfCplt,
    /**
     * @brief Reception complete.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    SAI_RxCplt,
    /**
     * @brief An error occurred.
     * @note Only present if @ref NILAI_USE_SAI_EVENTS is defined.
     */
    SAI_Error,
#    endif

    //! Number of events, should always be last.
    Count
};

/**
 * @enum EventCategories
 * @brief List of the existing categories of event.
 */
enum class EventCategories
{
    External = 0,    //!< External Event (external interrupt and software request).
    UserEvent,       //!< User defined software event.
    Data,            //!< User defined Data Event.
#    if defined(NILAI_USE_TIMER_EVENTS)
    /**
     * @brief Timer Event.
     * @note Only available when @ref NILAI_USE_TIMER_EVENTS is defined.
     */
    Timer,
#    endif
#    if defined(NILAI_USE_I2C_EVENTS)
    /**
     * @brief I2C Event
     * @note Only available when @ref NILAI_USE_I2C_EVENTS is defined.
     */
    I2C,
#    endif
#    if defined(NILAI_USE_I2S_EVENTS)
    /**
     * @brief I2S Event
     * @note Only available when @ref NILAI_USE_I2S_EVENTS is defined.
     */
    I2S,
#    endif
#    if defined(NILAI_USE_RTC_EVENTS)
    /**
     * @brief RTC Event.
     * @note Only available when @ref NILAI_USE_RTC_EVENTS is defined.
     */
    Rtc,
#    endif
#    if defined(NILAI_USE_SPI_EVENTS)
    /**
     * @brief SPI Event.
     * @note Only available when @ref NILAI_USE_SPI_EVENTS is defined.
     */
    Spi,
#    endif
#    if defined(NILAI_USE_UART_EVENTS)
    /**
     * @brief UART Event.
     * @note Only available when @ref NILAI_USE_UART_EVENTS is defined.
     */
    Uart,
#    endif
#    if defined(NILAI_USE_ADC_EVENTS)
    /**
     * @brief ADC Event.
     * @note Only available when @ref NILAI_USE_ADC_EVENTS is defined.
     */
    Adc,
#    endif
#    if defined(NILAI_USE_CAN_EVENTS)
    /**
     * @brief CAN Event.
     * @note Only available when @ref NILAI_USE_CAN_EVENTS is defined.
     */
    Can,
#    endif
#    if defined(NILAI_USE_SAI_EVENTS)
    /**
     * @brief SAI Event.
     * @note Only available when @ref NILAI_USE_SAI_EVENTS is defined.
     */
    Sai,
#    endif

    //! Number of Categories, should always be last.
    Count
};

/**
 * @brief Checks if the event type is in a specific event category.
 * @param t The event type to check.
 * @param c The category to match it type with.
 * @returns True if @c t is in @c c.
 * @returns False if @c t is not in @c c.
 */
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

//!@}
//!@}
}    // namespace Nilai::Events
#endif

#endif    // NILAI_EVENT_TYPES_H
