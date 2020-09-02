/**
 * @addtogroup  defines
 * @{
 * @addtogroup  module
 * @{
 * @file        module.hpp
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 * @p           https://www.github.com/Raesangur
 *
 * @brief       Module class definition.
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include <string>

namespace cep
{
/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */


/**
 * @class   Module
 * @brief   Base class to inherit from when building modules.                                    \n
 *          For more details on how to create a module from this class, check the README file in
 *          the cep20/tornatest repository.
 */
class Module
{
public:
    virtual ~Module() = default;

    virtual void Run() = 0;
    virtual const std::string& GetLabel() const = 0;
};

}    // namespace cep


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
