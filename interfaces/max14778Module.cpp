/**
 ******************************************************************************
 * @addtogroup max14778Module
 * @{
 * @file    max14778Module
 * @author  Samuel Martel
 * @brief   Sources for the max14778Module module.
 *
 * @date 9/15/2020 3:03:33 PM
 *
 ******************************************************************************
 */
#include "max14778Module.h"
#if defined(NILAI_USE_MAX14778)

#    include <utility>


Max14778Module::Max14778Module(MAX14778::Config config) : m_config(std::move(config))
{
}

void Max14778Module::SetEnA(bool state) const
{
    m_config.setEnAFunc(state);
}

void Max14778Module::SetEnB(bool state) const
{
    m_config.setEnBFunc(state);
}

void Max14778Module::SelectA0() const
{
    m_config.setSA0Func(false);
    m_config.setSA1Func(false);
}

void Max14778Module::SelectA1() const
{
    m_config.setSA0Func(true);
    m_config.setSA1Func(false);
}

void Max14778Module::SelectA2() const
{
    m_config.setSA0Func(false);
    m_config.setSA1Func(true);
}

void Max14778Module::SelectA3() const
{
    m_config.setSA0Func(true);
    m_config.setSA1Func(true);
}

void Max14778Module::SelectB0() const
{
    m_config.setSB0Func(false);
    m_config.setSB1Func(false);
}

void Max14778Module::SelectB1() const
{
    m_config.setSB0Func(true);
    m_config.setSB1Func(false);
}

void Max14778Module::SelectB2() const
{
    m_config.setSB0Func(false);
    m_config.setSB1Func(true);
}

void Max14778Module::SelectB3() const
{
    m_config.setSB0Func(true);
    m_config.setSB1Func(true);
}

void Max14778Module::SetACom(bool state) const
{
    m_config.setAComFunc(state);
}

void Max14778Module::SetBCom(bool state) const
{
    m_config.setBComFunc(state);
}

bool Max14778Module::GetACom() const
{
    return m_config.getAComFunc();
}

bool Max14778Module::GetBCom() const
{
    return m_config.getBComFunc();
}
#endif
