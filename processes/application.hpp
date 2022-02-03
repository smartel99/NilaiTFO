/**
 * @addtogroup  processes
 * @{
 * @addtogroup  application
 * @{
 *
 * @file        application.hpp
 * @author      Pascal-Emmanuel Lachance feat.
 * @author      Samuel Martel
 * @date        2020/07/06  -  09:02
 */
#ifndef GUARD_APPLICATION_HPP
#    define GUARD_APPLICATION_HPP
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */

#    include "../defines/Assertion.h"
#    include "../defines/module.hpp"

#    include <csignal>
#    include <vector>

namespace cep
{
/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
[[noreturn]] void AbortionHandler(int signal);

class Application
{
public:
    Application() { std::signal(SIGABRT, &AbortionHandler); }
    virtual ~Application() = default;

    virtual void Init()   = 0;
    virtual bool DoPost() = 0;
    virtual void Run()    = 0;
};

class ModuleStack
{
public:
    ModuleStack() { m_modules.reserve(16); }

    Module* GetModule(const std::string& label)
    {
        // I really don't like having to iterate through the vector, checking each labels...
        // A map breaks in Application::Run() though...
        for (const auto& module : m_modules)
        {
            if (module->GetLabel() == label)
            {
                return module;
            }
        }

        return nullptr;
    }

    void AddModule(Module* module) { m_modules.push_back(module); }

    std::vector<Module*>::iterator begin() { return m_modules.begin(); }
    std::vector<Module*>::iterator end() { return m_modules.end(); }

private:
    std::vector<Module*> m_modules;
};

}    // namespace cep

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
