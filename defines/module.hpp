/**
 * @addtogroup  defines
 * @{
 * @addtogroup  module
 * @{
 * @file        module.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 *
 * @brief       Module class definition.
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "shared/defines/pch.hpp"
//#include "shared/processes/application.hpp"


class Application;
namespace cep
{


/*************************************************************************************************/
/* Macros -------------------------------------------------------------------------------------- */
/**
 * @brief   Checks if the current module is enabled before entering a module's handler.          \n
 */
#define CHECK_TASK_ENABLED()                                                                       \
    do                                                                                             \
    {                                                                                              \
        if (IsEnabled() == false)                                                                  \
        {                                                                                          \
            return;                                                                                \
        }                                                                                          \
    } while (false)


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
    friend void Application::AddModule(Module* newModule);
    friend class Application;
    /*********************************************************************************************/
    /* Private & protected member variables ---------------------------------------------------- */
private:
    bool m_isEnabled = false;
    bool m_firstRun  = true;

protected:
    std::string m_moduleName;
    size_t      m_moduleIndex = 0;


    /*********************************************************************************************/
    /* Constructors ---------------------------------------------------------------------------- */
public:
    Module() = delete;
    Module(std::string_view moduleName) : m_moduleName{moduleName} {}
    ~Module() { DeInit(); }


    /*********************************************************************************************/
    /* Public member functions ----------------------------------------------------------------- */
    virtual void Init(size_t index)
    {
        SetInstance(index);
        Enable();
    };

    virtual void DeInit()
    {
        Disable();
        RemoveInstance(m_moduleIndex);
    }

    ALWAYS_INLINE void Enable() { m_isEnabled = true; }
    ALWAYS_INLINE void Disable() { m_isEnabled = true; }
    ALWAYS_INLINE bool IsEnabled() { return m_isEnabled; }

    /**
     * @brief   Check whether it is the first time this function is called.
     *
     * @retval  `True`: If this is the first time this function is called.
     *          `False`: If this is not the first time this function is called.
     */
    ALWAYS_INLINE bool FirstRun()
    {
        if (m_firstRun == true)
        {
            m_firstRun = false;
            return true;
        }
        else
        {
            return false;
        }
    }

    ALWAYS_INLINE void Run()
    {
        CHECK_TASK_ENABLED();
        TaskHandler();
    }

    /*********************************************************************************************/
    /* Virtual functions ----------------------------------------------------------------------- */
private:
    virtual void   SetInstance(size_t instanceIndex)  = 0;
    virtual size_t RemoveInstance(size_t moduleIndex) = 0;
    virtual void   TaskHandler()                      = 0;
};

}    // namespace cep


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
