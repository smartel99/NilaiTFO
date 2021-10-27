/**
 * @addtogroup  defines
 * @{
 * @addtogroup  module
 * @{
 * @file        module.hpp
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 *
 * @brief       Module class definition.
 */
#ifndef MODULE_HPP_
#define MODULE_HPP_
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
 * @brief   Base class to inherit from when building modules.
 */
class Module
{
public:
    virtual ~Module() = default;

    virtual bool                             DoPost()         = 0;
    virtual void                             Run()            = 0;
    [[nodiscard]] virtual const std::string& GetLabel() const = 0;
};

}    // namespace cep

/*************************************************************************************************/
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
