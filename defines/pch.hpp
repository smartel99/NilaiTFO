/**
 * @addtogroup  defines
 * @{
 * @addtogroup  pch
 * @{
 * @file        pch.hpp
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
/* CEP files */
#include "shared/defines/compilerDefines.h"
#include "shared/defines/macros.hpp"
#include "shared/defines/typeTemplates.hpp"
#include "shared/services/logger.hpp"
#include "shared/services/ticks.hpp"


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
