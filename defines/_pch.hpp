/**
 * @addtogroup  defines
 * @{
 * @addtogroup  pch
 * @{
 * @file        _pch.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 *
 * @brief       Attempt at precompile header, but it is not precompiled.                         \n
 *              This is just a general include file. (for now)
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */

/*------------------------------------*/
/* Standard library files */
#include <algorithm>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>

#include <array>
#include <map>
#include <string>
#include <string_view>
#include <vector>

/*------------------------------------*/
/* STM32 HAL */
#include "Core/Inc/main.h"
#include "Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"

/*------------------------------------*/
/* spdlog */
#include "vendor/spdlog/include/spdlog/spdlog.h"
#include "vendor/spdlog/include/spdlog/sinks/stdout_sinks.h"


/*------------------------------------*/
/* CEP files */
#include "defines/compilerDefines.h"
#include "defines/macros.hpp"
#include "defines/typeTemplates.hpp"
#include "services/logger.hpp"


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
