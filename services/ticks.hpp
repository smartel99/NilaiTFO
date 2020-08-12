/**
 * @addtogroup  services
 * @{
 * @addtogroup  ticks
 * @{
 * @file        ticks.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2019/11/20  -  16:49
 *
 * @brief       Header for the Tick module.
 *              This module gives access to multiple event based ticks.
 *
 * @version     2020/08/11 - Switch to C++17
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"
#include "shared/services/tickUnits.hpp"


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define SYSTICK  SysTicker::GetInstance()
#define FASTTICK FastTicker::GetInstance()


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */

#pragma region Callback
template<typename TickType>
class TickCallback_t
{
    /*********************************************************************************************/
    /* Public member variables ----------------------------------------------------------------- */
public:
    std::function<void(TickType)> m_function;
    TickType                      m_value;

    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
    TickCallback_t(std::function<void(TickType)> func, TickType val = TickType::npos)
    : m_function{func}, m_value{val}
    {
    }

    /*********************************************************************************************/
    /* Operators ------------------------------------------------------------------------------- */
    [[nodiscard]] constexpr bool
    operator==(const std::function<void(TickType)>& func) const noexcept
    {
        return m_function.template target<std::function<void(TickType)>>() ==
               func.template target<std::function<void(TickType)>>();
    }
    [[nodiscard]] constexpr bool
    operator!=(const std::function<void(TickType)>& func) const noexcept
    {
        return !(operator==(func));
    }

    constexpr void operator()(TickType val) const { m_function(val); }
};
#pragma endregion


#pragma region TickerBase
template<typename TickType>
class TickerBase
{
    /*********************************************************************************************/
    /* Private & protected member variables ---------------------------------------------------- */
private:
    std::vector<TickCallback_t<TickType>> m_callbacks;

protected:
    bool     m_enabled;
    TickType m_currentTick;


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    TickerBase()
    {
        static_assert((std::is_same<TickType, FastTick_t>::value ||
                       std::is_same<TickType, SysTick_t>::value),
                      "This tick type is not supported (must be FastTick_t or SysTick_t)");
    }


    /*********************************************************************************************/
    /* Public member functions ----------------------------------------------------------------- */
    [[nodiscard]] TickType Get() const noexcept { return m_currentTick; }
    [[nodiscard]] bool     CheckHasInc(TickType& timeToCheck = TickType::npos) const noexcept;

    [[nodiscard]] constexpr TickType FindTimeoutTime(TickType timeout) const noexcept;
    [[nodiscard]] constexpr bool     ShouldTimeout(TickType timeoutTime) const noexcept;

    virtual TickType Start()
    {
        m_enabled = true;
        return m_currentTick;
    }
    virtual void Stop() { m_enabled = false; }
    virtual void Reset() { m_currentTick = 0; }

    constexpr void Increment() noexcept
    {
        if (m_enabled == true)
        {
            m_currentTick++;

            for (const TickCallback_t<TickType>& callback : m_callbacks)
            {
                if (callback.m_value == TickType::npos || callback.m_value == m_currentTick)
                {
                    callback(m_currentTick);
                }
            }
        }
    }
    constexpr void Decrement() noexcept
    {
        if (m_enabled == true)
        {
            m_currentTick--;
        }
    }

    void AddCallback(std::function<void(TickType)>& callbackFunc,
                     TickType                       value = TickType::npos) noexcept;
    void RemoveCallback(std::function<void(TickType)>& callbackFunc);
};
#pragma endregion


class SysTicker : public TickerBase<SysTick_t>
{
private:
    static SysTicker s_instance;

public:
    ALWAYS_INLINE static SysTicker& GetInstance() { return s_instance; }
};

class FastTicker : public TickerBase<FastTick_t>
{
private:
    std::size_t       s_accessCounter = 0;
    static FastTicker s_instance;

public:
    ALWAYS_INLINE static FastTicker& GetInstance() { return s_instance; }

    FastTick_t Start() override
    {
        m_enabled = true;
        s_accessCounter++;
        return m_currentTick;
    }
    void Stop() override
    {
        if (--s_accessCounter == 0)
        {
            m_enabled = false;
        }
    }
};


/*************************************************************************************************/
/* Public member functions definitions --------------------------------------------------------- */
#pragma region Public member function definitions

/**
 * @brief   Check if the tick counter has incremented since the last call of this function.
 *          Another tick counter variable can be provided to make the comparison.
 *
 * @param   None
 * @retval  true  If FastTick has incremented
 * @retval  false If FastTick has not incremented
 */
template<typename TickType>
[[nodiscard]] bool TickerBase<TickType>::CheckHasInc(TickType& timeToCheck) const noexcept
{
    static TickType oldTick = -1;

    if (timeToCheck != -1)
    {
        if (timeToCheck != m_currentTick)
        {
            timeToCheck = m_currentTick;
            return true;
        }
    }
    else
    {
        if (oldTick != m_currentTick)
        {
            oldTick = m_currentTick;
            return true;
        }
    }

    return false;
}

/**
 * @brief   Calculates the time at which a timeout should happen from the current value of the tick
 *          counter and the desired number of ticks.
 * @param   timeout The number of ticks that should be awaited for before timing out
 * @retval  TickType: The value of the tick counter at which point the timeout should happen
 */
template<typename TickType>
[[nodiscard]] constexpr TickType
TickerBase<TickType>::FindTimeoutTime(TickType timeout) const noexcept
{
    return m_currentTick + timeout;
}

/**
 * @brief   Compare the current value of the tick counter with the passed value.
 *          If the counter is greater than the value, it means that the scheduled timeout time has
 *          been reached.
 *
 * @param   timeoutTime The number of ticks that must have done before timing out
 * @retval  true  If a timeout should happen
 * @retval  false If no timeout should happen
 */
template<typename TickType>
[[nodiscard]] constexpr bool
TickerBase<TickType>::ShouldTimeout(TickType timeoutTime) const noexcept
{
    return m_currentTick > timeoutTime;
}

/**
 * @brief   Add a new callback function to the callback function stack.
 *          The callback function will be called when the tick counter reaches the indicated value.
 *          If the indicated value is set to TickType::npos, the callback function will be called
 *          every time the counter increases.
 */
template<typename TickType>
void TickerBase<TickType>::AddCallback(std::function<void(TickType)>& callbackFunc,
                                       TickType                       value) noexcept
{
    m_callbacks.emplace_back(callbackFunc, value);
}

/**
 * @brief   Remove a specific callback function from the callback function stack.
 */
template<typename TickType>
void TickerBase<TickType>::RemoveCallback(std::function<void(TickType)>& callbackFunc)
{
    m_callbacks.erase(std::remove(m_callbacks.begin(), m_callbacks.end(), callbackFunc),
                      m_callbacks.end());
}

#pragma endregion


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
