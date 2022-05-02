/**
 * @addtogroup  drivers
 * @{
 * @addtogroup  RTC
 * @{
 * @file        rtcModule
 * @author      Samuel Martel
 * @date        2021/04/09
 *
 * @brief       Real-Time Clock Module
 */
#ifndef NILAI_RTC_MODULE_HPP_
#    define NILAI_RTC_MODULE_HPP_
/*************************************************************************************************/
/* Includes
 * ------------------------------------------------------------------------------------
 */
#    if defined(NILAI_USE_RTC)
#        include "defines/internalConfig.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_RTC_MODULE_ENABLED)
#            include "defines/macros.hpp"
#            include "defines/misc.hpp"
#            include "defines/module.hpp"

#            include "RTC/Enums.h"
#            include "RTC/Structs.h"

#            if defined(NILAI_USE_EVENTS)
#                include "../defines/Events/Events.h"
#            endif

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
class RtcModule : public cep::Module
{
public:
    RtcModule(RTC_HandleTypeDef* handle);

    virtual ~RtcModule() override = default;

    virtual bool DoPost() override;
    virtual void Run() override;

    bool OnEvent(cep::Events::Event* event) override;

    void           SetTime(const cep::Rtc::Time& time);
    cep::Rtc::Time GetTime();

    void           SetDate(const cep::Rtc::Date& date);
    cep::Rtc::Date GetDate();

    cep::Rtc::Timestamp GetTimestamp();

    size_t        GetEpoch();
    static size_t GetEpoch(const cep::Rtc::Date& date, const cep::Rtc::Time& time);

    static RtcModule* Get() { return s_instance; }

private:
    static RtcModule* s_instance;

private:
    RTC_HandleTypeDef* m_handle;
};


#        else
#            if WARN_MISSING_STM_DRIVERS
#                warning NilaiTFO RTC module enabled, but HAL_RTC_USE_MODULE is not defined!
#            endif
#        endif
#    endif
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
