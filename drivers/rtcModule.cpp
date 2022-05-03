/**
 * @addtogroup rtcModule.cpp
 * @{
 *******************************************************************************
 * @file	rtcModule.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#include "rtcModule.h"

#if defined(NILAI_USE_RTC) && defined(HAL_RTC_MODULE_ENABLED)
#    include "services/logger.hpp"

#    define RTC_INFO(msg, ...)  LOG_INFO("[RTC]: " msg, ##__VA_ARGS__)
#    define RTC_ERROR(msg, ...) LOG_ERROR("[RTC]: " msg, ##__VA_ARGS__)

RtcModule* RtcModule::s_instance = nullptr;

RtcModule::RtcModule(RTC_HandleTypeDef* handle) : m_handle(handle)
{
    CEP_ASSERT(handle != nullptr, "RTC Handle is NULL!");
    CEP_ASSERT(s_instance == nullptr, "Only one RTC instance is allowed!");

    s_instance = this;

    RTC_INFO("Initialized");
}

/**
 * For the POST of the RTC to pass, it must validate that the RTC peripheral keeps track of time
 * properly.
 * This is done by first reading the time stored by the RTC. We then wait 2 seconds, then
 * re-read the time stored by the RTC.
 * Since it is really unlikely that the two readings are frame perfect, the second reading should be
 * 1 to 3 seconds after the first reading.
 * @brief   POST function of the RTC module.
 * @return
 */
bool RtcModule::DoPost()
{
    cep::Rtc::Time firstTime = GetTime();
    HAL_Delay(2000);
    cep::Rtc::Time secondTime = GetTime();

    size_t ftSec = firstTime.ToSeconds();
    size_t stSec = secondTime.ToSeconds();
    if ((stSec >= ftSec + 1) && (stSec <= ftSec + 3))
    {
        RTC_INFO("POST OK");
        return true;
    }
    else
    {
        RTC_ERROR("POST ERROR! (first: %i, second: %i)", ftSec, stSec);
        return false;
    }
}

void RtcModule::Run()
{
}

#    if defined(NILAI_USE_EVENTS)
bool RtcModule::OnEvent(cep::Events::Event* event)
{
#        if defined(NILAI_USE_RTC_EVENTS)
    return Module::OnEvent(event);
#        else
    return false;
#        endif
}
#    endif

void RtcModule::SetTime(const cep::Rtc::Time& time)
{
    RTC_TimeTypeDef newTime = time.ToHal();

    if (HAL_RTC_SetTime(m_handle, &newTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        RTC_ERROR("Unable to set the time!");
    }
}

cep::Rtc::Time RtcModule::GetTime()
{
    RTC_TimeTypeDef halTime = {};

    if (HAL_RTC_GetTime(m_handle, &halTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        RTC_ERROR("Unable to get the time!");
    }

    // We must always also read the date after reading the time.
    RTC_DateTypeDef dummyDate = {};
    HAL_RTC_GetDate(m_handle, &dummyDate, RTC_FORMAT_BIN);

    return {halTime};
}

void RtcModule::SetDate(const cep::Rtc::Date& date)
{
    RTC_DateTypeDef newDate = date.ToHal();

    if (HAL_RTC_SetDate(m_handle, &newDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        RTC_ERROR("Unable to set the date!");
    }
}

cep::Rtc::Date RtcModule::GetDate()
{
    RTC_TimeTypeDef dummyTime = {};

    // We must always read the time before reading the date.
    HAL_RTC_GetTime(m_handle, &dummyTime, RTC_FORMAT_BIN);

    RTC_DateTypeDef halDate = {};
    if (HAL_RTC_GetDate(m_handle, &halDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        RTC_ERROR("Unable to get the date!");
    }

    return {halDate};
}

cep::Rtc::Timestamp RtcModule::GetTimestamp()
{
    return {GetDate(), GetTime()};
}

#    if defined(NILAI_RTC_USE_STL)
size_t RtcModule::GetEpoch()
{
    return GetEpoch(GetDate(), GetTime());
}

size_t RtcModule::GetEpoch(const cep::Rtc::Date& date, const cep::Rtc::Time& time)
{
    time_t    epoch = {0};
    struct tm tim   = {};

    tim.tm_year = (date.year - 1900);
    tim.tm_mon  = date.month - 1;
    tim.tm_mday = date.day;
    tim.tm_hour = time.hours;
    tim.tm_min  = time.minutes;
    tim.tm_sec  = time.seconds;

    epoch = mktime(&tim);
    return (size_t)epoch;
}
#    endif


#endif
