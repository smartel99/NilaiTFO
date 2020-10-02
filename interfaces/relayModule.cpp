/**
 ******************************************************************************
 * @addtogroup relayModule
 * @{
 * @file    relayModule
 * @author  Lou-Gabriel Gaucher
 * @brief   Sources for the relayModule module.
 *
 * @date 9/24/2020 3:03:33 PM
 *
 ******************************************************************************
 */
#include "relayModule.h"

#include "stm32f4xx_hal.h"

#include <algorithm>

RelayModule::RelayModule(const RELAY::Config& config, const std::string& label)
	: m_config(config)
	, m_label(label)
{
}

// what to put in "RUN"
void RelayModule::Run()
{
	SetEN(true);

	HAL_Delay(100);
}

void RelayModule::SetEN(bool state)
{
	if (m_config.setENFunc)
	{
		m_config.setENFunc(state);
	}
}

bool RelayModule::GetEN()
{
	if (m_config.getENFunc)
	{
		return m_config.getENFunc();
	}
	else
	{
		return false;
	}
}

void RelayModule::SetContactor1(bool state)
{
	if (m_config.setENFunc)
	{
		m_config.setENFunc(state);
	}
}

bool RelayModule::GetContactor1()
{
	if (m_config.getENFunc)
	{
		return m_config.getENFunc();
	}
	else
	{
		return false;
	}
}

void RelayModule::SetContactor2(bool state)
{
	if (m_config.setENFunc)
	{
		m_config.setENFunc(state);
	}
}

bool RelayModule::GetContactor2()
{
	if (m_config.getENFunc)
	{
		return m_config.getENFunc();
	}
	else
	{
		return false;
	}
}