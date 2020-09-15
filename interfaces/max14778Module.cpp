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

#include "stm32f4xx_hal.h"

#include <algorithm>

Max14778Module::Max14778Module(const MAX14778::Config& config, const std::string& label)
    : m_config(config), m_label(label)
{
}

void Max14778Module::Run( )
{
    static uint8_t cnt = 0;
    switch (cnt)
    {
        case 0:
            SetEnA(true);
            SetEnB(false);
            SelectA0( );
            break;
        case 1:
            SelectA1( );
            break;
        case 2:
            SelectA2( );
            break;
        case 3:
            SelectA3( );
            break;
        case 4:
            SetEnA(false);
            SetEnB(true);
            SelectB0( );
            break;
        case 5:
            SelectB1( );
            break;
        case 6:
            SelectB2( );
            break;
        case 7:
            SelectB3( );
            break;
    }

    if (++cnt > 7)
    {
        cnt = 0;
    }
    HAL_Delay(100);
}

void Max14778Module::SetEnA(bool state)
{
    if (m_config.setEnAFunc)
    {
        m_config.setEnAFunc(state);
    }
}

void Max14778Module::SetEnB(bool state)
{
    if (m_config.setEnBFunc)
    {
        m_config.setEnBFunc(state);
    }
}

void Max14778Module::SelectA0( )
{
    if (m_config.setSA0Func)
    {
        m_config.setSA0Func(false);
    }
    if (m_config.setSA1Func)
    {
        m_config.setSA1Func(false);
    }
}

void Max14778Module::SelectA1( )
{
    if (m_config.setSA0Func)
    {
        m_config.setSA0Func(true);
    }
    if (m_config.setSA1Func)
    {
        m_config.setSA1Func(false);
    }
}

void Max14778Module::SelectA2( )
{
    if (m_config.setSA0Func)
    {
        m_config.setSA0Func(false);
    }
    if (m_config.setSA1Func)
    {
        m_config.setSA1Func(true);
    }
}

void Max14778Module::SelectA3( )
{
    if (m_config.setSA0Func)
    {
        m_config.setSA0Func(true);
    }
    if (m_config.setSA1Func)
    {
        m_config.setSA1Func(true);
    }
}

void Max14778Module::SelectB0( )
{
    if (m_config.setSB0Func)
    {
        m_config.setSB0Func(false);
    }
    if (m_config.setSB1Func)
    {
        m_config.setSB1Func(false);
    }
}

void Max14778Module::SelectB1( )
{
    if (m_config.setSB0Func)
    {
        m_config.setSB0Func(true);
    }
    if (m_config.setSB1Func)
    {
        m_config.setSB1Func(false);
    }
}

void Max14778Module::SelectB2( )
{
    if (m_config.setSB0Func)
    {
        m_config.setSB0Func(false);
    }
    if (m_config.setSB1Func)
    {
        m_config.setSB1Func(true);
    }
}

void Max14778Module::SelectB3( )
{
    if (m_config.setSB0Func)
    {
        m_config.setSB0Func(true);
    }
    if (m_config.setSB1Func)
    {
        m_config.setSB1Func(true);
    }
}

void Max14778Module::SetACom(bool state)
{
    if (m_config.setAComFunc)
    {
        m_config.setAComFunc(state);
    }
}

void Max14778Module::SetBCom(bool state)
{
    if (m_config.setBComFunc)
    {
        m_config.setBComFunc(state);
    }
}

bool Max14778Module::GetACom( )
{
    if (m_config.getAComFunc)
    {
        return m_config.getAComFunc( );
    }
    else
    {
        return false;
    }
}

bool Max14778Module::GetBCom( )
{
    if (m_config.getBComFunc)
    {
        return m_config.getBComFunc( );
    }
    else
    {
        return false;
    }
}
