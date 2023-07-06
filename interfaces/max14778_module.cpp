/**
 ******************************************************************************
 * @file    max14778_module.cpp
 * @author  Samuel Martel
 * @brief   Sources for the max14778Module module.
 *
 * @date 9/15/2020 3:03:33 PM
 *
 ******************************************************************************
 */
#include "max14778_module.h"
#if defined(NILAI_USE_MAX14778)
#    include "stm32f4xx_hal.h"

#    include <algorithm>

#    define CALL_SET_FN(fn, state)                                                                 \
        do                                                                                         \
        {                                                                                          \
            if (m_config.fn) m_config.fn(state);                                                   \
        } while (0)
#    define CALL_GET_FN(fn) (m_config.fn ? m_config.fn() : 0)

Max14778Module::Max14778Module(const MAX14778::Config& config) : m_config(config)
{
}

void Max14778Module::SetEnA(bool state) const
{
    CALL_SET_FN(setEnAFunc, state);
}

void Max14778Module::SetEnB(bool state) const
{
    CALL_SET_FN(setEnBFunc, state);
}

void Max14778Module::SelectA0() const
{
    CALL_SET_FN(setSA0Func, false);
    CALL_SET_FN(setSA1Func, false);
}

void Max14778Module::SelectA1() const
{
    CALL_SET_FN(setSA0Func, true);
    CALL_SET_FN(setSA1Func, false);
}

void Max14778Module::SelectA2() const
{
    CALL_SET_FN(setSA0Func, false);
    CALL_SET_FN(setSA1Func, true);
}

void Max14778Module::SelectA3() const
{
    CALL_SET_FN(setSA0Func, true);
    CALL_SET_FN(setSA1Func, true);
}

void Max14778Module::SelectB0() const
{
    CALL_SET_FN(setSB0Func, false);
    CALL_SET_FN(setSB1Func, false);
}

void Max14778Module::SelectB1() const
{
    CALL_SET_FN(setSB0Func, true);
    CALL_SET_FN(setSB1Func, false);
}

void Max14778Module::SelectB2() const
{
    CALL_SET_FN(setSB0Func, false);
    CALL_SET_FN(setSB1Func, true);
}

void Max14778Module::SelectB3() const
{
    CALL_SET_FN(setSB0Func, true);
    CALL_SET_FN(setSB1Func, true);
}

void Max14778Module::SetACom(bool state) const
{
    CALL_SET_FN(setAComFunc, state);
}

void Max14778Module::SetBCom(bool state) const
{
    CALL_SET_FN(setBComFunc, state);
}

bool Max14778Module::GetACom() const
{
    return CALL_GET_FN(getAComFunc);
}

bool Max14778Module::GetBCom() const
{
    return CALL_GET_FN(getBComFunc);
}
#endif
