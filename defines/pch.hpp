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

#include <cstddef>
#include <limits>
#include <type_traits>


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


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
