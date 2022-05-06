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
#    include <cstddef>

namespace Nilai
{
/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */

/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class Application;

/**
 * @class   Module
 * @brief   Base class to inherit from when building modules.
 */
class Module
{
public:
    Module()          = default;
    virtual ~Module() = default;

    /**
     * @brief Called when the module is being attached to the application.
     */
    virtual void OnAttach() {}

    /**
     * @brief Called when the module is being detached from the application.
     */
    virtual void OnDetach() {}

    /**
     * @brief Called during the application startup.
     *
     * Use this function to verify the functionalities of the module and to check for problems.
     *
     * Returning false from this function causes the POST to fail, the application blocking.
     *
     * @return True if the POST passed, false otherwise.
     */
    virtual bool DoPost() { return true; }

    /**
     * @brief Called once every frame.
     */
    virtual void Run() {}

    /**
     * @brief Gets the ID of the module.
     * @return the ID of the module
     */
    [[nodiscard]] size_t GetId() const { return m_id; }

protected:
    friend Application;

    size_t m_id = 0;
};

}    // namespace Nilai

/*************************************************************************************************/
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
