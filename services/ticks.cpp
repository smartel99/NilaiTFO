/**
 * @addtogroup  services
 * @{
 * @addtogroup  ticks
 * @{
 * @file        ticks.cpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2019/11/20  -  16:49
 *
 * @brief       Source for the Tick module.
 *              This module gives access to multiple event based ticks.
 *
 * @version     2020/08/11 - Switch to C++17 - Pascal-Emmanuel Lachance
 */
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/services/ticks.hpp"


/*************************************************************************************************/
/* Defines ------------------------------------------------------------------------------------- */
#define FASTTICK_SPEED 10000
#define SYSTICK_SPEED  1000


/*************************************************************************************************/
/* Constructors -------------------------------------------------------------------------------- */
FastTick_t::FastTick_t(const SysTick_t& systick)
{
    value = systick.value * (FASTTICK_SPEED / SYSTICK_SPEED);
}

SysTick_t::SysTick_t(const FastTick_t& fasttick)
{
    value = fasttick.value * (SYSTICK_SPEED / FASTTICK_SPEED);
}


/*************************************************************************************************/
/* Instance creation --------------------------------------------------------------------------- */
SysTicker SysTicker::s_instance{};
FastTicker FastTicker::s_instance{};


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
