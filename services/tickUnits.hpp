/**
 * @addtogroup  services
 * @{
 * @addtogroup  ticks
 * @{
 *
 * @file        tickUnits.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/12  -  12:14
 *
 * @brief       Base SysTick_t and FastTick_t units used to count ticks with the timers.
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
/* Forward declarations */
class FastTick_t;
class SysTick_t;

#pragma region Tick Units
class TickUnit_Base
{
public:
    using TickCounterType = uint64_t;

    constexpr static TickCounterType npos = -1;

    /*********************************************************************************************/
    /* Public member variables ----------------------------------------------------------------- */
    TickCounterType value = 0;


    /*********************************************************************************************/
    /* Constructors ---------------------------------------------------------------------------- */
    constexpr TickUnit_Base() = default;
    constexpr TickUnit_Base(TickCounterType val) : value{val} {}


    /*********************************************************************************************/
    /* Operators ------------------------------------------------------------------------------- */
    constexpr operator TickCounterType() const noexcept { return value; }

    /* Postfix operators */
    constexpr TickCounterType operator++(int) noexcept { return value++; }
    constexpr TickCounterType operator--(int) noexcept { return value--; }
};


/*------------------------------------*/
class FastTick_t : public TickUnit_Base
{
    /*********************************************************************************************/
    /* Constructors ---------------------------------------------------------------------------- */
public:
    constexpr FastTick_t() = default;
    constexpr FastTick_t(TickCounterType val) : TickUnit_Base{val} {}
    FastTick_t(const SysTick_t& systick); /* Definition in ticks.cpp */
    constexpr FastTick_t(const FastTick_t&) = default;


    /*********************************************************************************************/
    /* Operators ------------------------------------------------------------------------------- */

    constexpr FastTick_t operator=(TickCounterType val) noexcept { return value = val; }
    constexpr            operator TickCounterType() const noexcept { return value; }
};
constexpr FastTick_t operator"" _FastTicks(FastTick_t::TickCounterType val) noexcept
{
    return FastTick_t{val};
}


/*------------------------------------*/
class SysTick_t : public TickUnit_Base
{
    /*********************************************************************************************/
    /* Constructors ---------------------------------------------------------------------------- */
public:
    constexpr SysTick_t() = default;
    constexpr SysTick_t(TickCounterType val) : TickUnit_Base{val} {}
    SysTick_t(const FastTick_t& fasttick); /* Definition in ticks.cpp */
    constexpr SysTick_t(const SysTick_t&) = default;


    /*********************************************************************************************/
    /* Operators ------------------------------------------------------------------------------- */
    constexpr SysTick_t operator=(TickCounterType val) noexcept { return value = val; }
    constexpr           operator TickCounterType() const noexcept { return value; }
};
constexpr SysTick_t operator"" _SysTicks(SysTick_t::TickCounterType val) noexcept
{
    return SysTick_t{val};
}

#pragma endregion


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
