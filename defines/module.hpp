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
#ifndef NILAI_MODULE_HPP_
#    define NILAI_MODULE_HPP_
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#    include <string>

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

    virtual bool                             DoPost() { return false; }
    virtual void                             Run() {}
    [[nodiscard]] virtual const std::string& GetLabel() const
    {
        // This is UB, yes. However, this function should always be overridden.
        // It must not be pure virtual to avoid adding excessive bloat to the binary file.
        DISABLE_WARNING("-Wreturn-local-addr")
        return {};
        DISABLE_WARNING_POP
    }
};

}    // namespace cep

/*************************************************************************************************/
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
