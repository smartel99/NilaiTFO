/**
 * @addtogroup  services
 * @{
 * @addtogroup  ticks
 * @{
 * @file        timeout.hpp
 * @author      Samuel Martel
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/12  -  10:42
 *
 * @brief       RAII class starting a FastTicker until a timeout is hit.
 *              At which point, it either calls an error, or a custom function.
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define START_TIMEOUT(fastticks, ...)                                                              \
    Timeout { LOG_CURRENT_LOCATION, ##__VA_ARGS__ }


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
template<typename TickType = FastTick_t>
class Timeout
{
    /*********************************************************************************************/
    /* Private member variables ---------------------------------------------------------------- */
private:
    bool     m_wasHit = false;
    TickType m_timeoutTime;

    const std::string_view m_file;
    const std::string_view m_func;
    std::size_t            m_line;

    std::function<void(TickType)> m_timeoutCallback;
    std::function<void(void)>     m_callback;

    /*********************************************************************************************/
    /* Constructors ---------------------------------------------------------------------------- */
public:
    Timeout()               = delete;
    Timeout(const Timeout&) = default;
    Timeout& operator=(const Timeout&) = default;

    Timeout(TickType               timeoutTime,
            std::string_view file,
            std::string_view func,
            std::size_t            line)
    : m_timeoutTime{timeoutTime}, m_file{file}, m_func{func}, m_line{line}
    {
        SubscribeToTick();
    }

    Timeout(TickType                  timeoutTime,
            std::string_view    file,
            std::string_view    func,
            std::size_t               line,
            std::function<void(void)> callback)
    : m_timeoutTime{timeoutTime}, m_file{file}, m_func{func}, m_line{line}, m_callback{callback}
    {
        SubscribeToTick();
    }

    ~Timeout()
    {
        if constexpr (std::is_same_v<TickType, FastTick_t>)
        {
            FASTTICK.RemoveCallback(m_timeoutCallback);
            FASTTICK.Stop();
        }
        else if constexpr (std::is_same_v<TickType, SysTick_t>)
        {
            SYSTICK.RemoveCallback(m_timeoutCallback);
        }
    }


    /*********************************************************************************************/
    /* Public member functions ----------------------------------------------------------------- */
public:
    void TimeoutHit(FastTick_t tick)
    {
        UNUSED(tick);
        m_wasHit = true;
        if (m_callback)
        {
            m_callback();
        }
        else
        {
            LOG_WARNING_HELPER("Timeout hit", m_file, m_func, m_line);
        }
    }

    bool WasHit() { return m_wasHit; }


    /*********************************************************************************************/
    /* Private member functions --------------------------------------------------------------- */
private:
    void SubscribeToTick()
    {
        m_timeoutCallback = std::bind(&Timeout::TimeoutHit, this, std::placeholders::_1);

        if constexpr (std::is_same_v<TickType, FastTick_t>)
        {
            FASTTICK.AddCallback(m_timeoutCallback, m_timeoutTime);
            FASTTICK.Start();
        }
        else if constexpr (std::is_same_v<TickType, SysTick_t>)
        {
            SYSTICK.AddCallback(m_timeoutCallback, m_timeoutTime);
        }
        else
        {
            /* clang-format off */
            static_assert(!std::is_same_v<TickType, FastTick_t> &&
                          !std::is_same_v<TickType, SysTick_t>,
                          "Timeout tick type is invalid (must be FastTick_t or SysTick_t)");
            /* clang-format on */
        }
    }
};


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
