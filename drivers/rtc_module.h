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
#        include "defines/internal_config.h"
#        include NILAI_HAL_HEADER
#        if defined(HAL_RTC_MODULE_ENABLED)
#            include "defines/macros.h"
#            include "defines/misc.h"
#            include "defines/module.h"

#            include "RTC/enums.h"
#            include "RTC/structs.h"

#            if defined(NILAI_USE_EVENTS)
#                include "../defines/Events/events.h"
#            endif

/*************************************************************************************************/
/* Defines
 * -------------------------------------------------------------------------------------
 */

/*************************************************************************************************/
/* Classes
 * -------------------------------------------------------------------------------------
 */
namespace Nilai::Drivers
{
class RtcModule : public Module
{
public:
    RtcModule(RTC_HandleTypeDef* handle);

    virtual ~RtcModule() override = default;

    virtual bool DoPost() override;
    virtual void Run() override;

    void      SetTime(const Rtc::Time& time);
    Rtc::Time GetTime();

    void      SetDate(const Rtc::Date& date);
    Rtc::Date GetDate();

    Rtc::Timestamp GetTimestamp();

#            if defined(NILAI_RTC_USE_STL)
    size_t        GetEpoch();
    static size_t GetEpoch(const Nilai::Rtc::Date& date, const Nilai::Rtc::Time& time);
#            endif

    static RtcModule* Get() { return s_instance; }

private:
    static RtcModule* s_instance;

private:
    RTC_HandleTypeDef* m_handle;
};
}    // namespace Nilai::Drivers


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
