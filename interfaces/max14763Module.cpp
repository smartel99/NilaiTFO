/**
 ******************************************************************************
 * @addtogroup max14763Module
 * @{
 * @file    max14763Module
 * @author  Lou-Gabriel Gaucher
 * @brief   Sources for the max14763Module module.
 *
 * @date 9/23/2020 3:03:33 PM
 *
 ******************************************************************************
 */
#include "max14763Module.h"
#if defined(NILAI_USE_MAX14763)
#include "stm32f4xx_hal.h"

#include <algorithm>
 /*
// Set pin to LOW.
MAX1763::Config u30Config;
 U30 : u30Config.setSELFunc = &IOEXT3_MODULE->WritePin(PCA9505::Ports::p0, PCA9505::Pins::p4, false);
 U30 : u30Config.getSELFunc = &IOEXT3_MODULE->ReadPin(PCA9505::Ports::p0, PCA9505::Pins::p4, false);

 U31 : u31Config.setSELFunc = &IOEXT3_MODULE->WritePin(PCA9505::Ports::p0, PCA9505::Pins::p3, false);
 U31 : u31Config.getSELFunc = &IOEXT3_MODULE->ReadPin(PCA9505::Ports::p0, PCA9505::Pins::p3, false);

 U33 : u33Config.setSELFunc = &IOEXT3_MODULE->WritePin(PCA9505::Ports::p0, PCA9505::Pins::p2, false);
 U33 : u33Config.getSELFunc = &IOEXT3_MODULE->ReadPin(PCA9505::Ports::p0, PCA9505::Pins::p2, false);

 U34 : u34Config.setSELFunc = &IOEXT3_MODULE->WritePin(PCA9505::Ports::p0, PCA9505::Pins::p1, false);
 U34 : u34Config.getSELFunc = &IOEXT3_MODULE->ReadPin(PCA9505::Ports::p0, PCA9505::Pins::p1, false);

 U36 : u36Config.setSELFunc = &IOEXT3_MODULE->WritePin(PCA9505::Ports::p0, PCA9505::Pins::p0, false);
 U36 : u36Config.getSELFunc = &IOEXT3_MODULE->ReadPin(PCA9505::Ports::p0, PCA9505::Pins::p0, false);

 */
Max14763Module::Max14763Module(const MAX14763::Config& config, const std::string& label)
    : m_config(config), m_label(label)
{
}

// what to put in "RUN"
void Max14763Module::Run( )
{
    SetSEL(true);

    HAL_Delay(100);
}

void Max14763Module::SetSEL(bool state)
{
    if (m_config.setSELFunc)
    {
        m_config.setSELFunc(state);
    }
}

bool Max14763Module::GetSEL()
{
    if (m_config.getSELFunc)
    {
        return m_config.getSELFunc( );
    }
    else
    {
        return false;
    }
}
#endif
