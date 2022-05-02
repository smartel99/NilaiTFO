/**
 * @addtogroup application.cpp
 * @{
 *******************************************************************************
 * @file	application.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: May 19, 2021
 *******************************************************************************
 */
#include "application.hpp"

#if defined(NILAI_USE_EVENTS)
#    include "../defines/macros.hpp"
#endif

static void AtExitForwarder();


namespace cep
{
Application* Application::s_instance = nullptr;

Application::Application()
{
    std::set_terminate(&AtExitForwarder);
    std::set_unexpected(&AtExitForwarder);
    std::signal(SIGABRT, &AbortionHandler);
    std::atexit(&AtExitForwarder);

    m_modules.reserve(NILAI_MAX_MODULE_AMOUNT);
    m_deletionQueue.reserve(NILAI_MAX_MODULE_AMOUNT);

    s_instance = this;
}

#if defined(NILAI_USE_EVENTS)
void Application::TriggerSoftEvent(Events::SoftwareEvents ch)
{
    Events::ExtEvent e(true, (uint8_t)ch, Events::EventTypes(ch));

    DispatchEvent(&e);
}

void Application::DispatchEvent(Events::Event* data)
{
    for (auto& module : m_modules)
    {
        if (module.Mod->OnEvent(data))
        {
            // Event was handled, stop propagating it.
            break;
        }
    }
}
#endif

void Application::Run()
{
    if (!m_modulesPendingDeletion)
    {
        return;
    }

    m_modulesPendingDeletion = false;
    for (size_t id : m_deletionQueue)
    {
        Ref<Module> module = GetModule(id);
        module->OnDetach();
    }

    // Remove the modules to be removed from the list.
    m_modules.erase(std::remove_if(m_modules.begin(),
                                   m_modules.end(),
                                   [&, this](const auto& m)
                                   {
                                       return std::find(m_deletionQueue.begin(),
                                                        m_deletionQueue.end(),
                                                        m.Id) != m_deletionQueue.end();
                                   }),
                    m_modules.end());
    m_deletionQueue.clear();
}

void Application::RemoveModule(size_t id)
{
    m_deletionQueue.push_back(id);
    m_modulesPendingDeletion = true;
}

cep::Ref<cep::Module> Application::GetModule(size_t id)
{
    for (auto& module : m_modules)
    {
        if (module.Id == id)
        {
            return module.Mod;
        }
    }

    // Module not found.
    AssertFailed((const uint8_t*)__FILE__, __LINE__, 1);
}

[[noreturn]] void AbortionHandler(int signal)
{
    (void)signal;
    // This function will in most cases be called by the STL, in place of the standard _exit().
    AssertFailed((const uint8_t*)__FILE__, __LINE__, 1);
    while (true)
    {
    }
}
}    // namespace cep

void AtExitForwarder()
{
    cep::AbortionHandler(0);
}
