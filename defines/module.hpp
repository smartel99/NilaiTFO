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

#    if defined(NILAI_USE_EVENTS)
#        include "Events/Events.h"
#    endif

namespace cep
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

#    if defined(NILAI_USE_EVENTS)
    /**
     * @brief Called whenever an event happens.
     * @param type The type of event
     * @param event The event data object.
     * @returns True if the event was handled and shouldn't be propagated further.
     * @returns False if the event was not handled and/or should be propagated to the other modules.
     */
    virtual bool OnEvent([[maybe_unused]] Events::Event* event) { return false; }
#    endif

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

}    // namespace cep

/*************************************************************************************************/
#endif
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
