/**
 * @file	application.cpp
 * @author	Samuel Martel
 * @brief
 * Created on: May 19, 2021
 *******************************************************************************
 */
#include "application.h"

#if defined(NILAI_USE_EVENTS)
#    include "../defines/macros.h"
#endif

#include "../services/profiler/profiler.h"

#include <exception>

static void AtExitForwarder();


namespace Nilai
{
Application* Application::s_instance = nullptr;


Application::Application()
{
    std::set_terminate(&AtExitForwarder);
    std::signal(SIGABRT, &AbortionHandler);
    std::atexit(&AtExitForwarder);

    m_modules.reserve(NILAI_MAX_MODULE_AMOUNT);
    m_deletionQueue.reserve(NILAI_MAX_MODULE_AMOUNT);

    s_instance = this;
}

#if defined(NILAI_USE_EVENTS)
size_t Application::RegisterEventCallback(Events::EventTypes event, const Events::EventFunction& cb)
{
    NILAI_ASSERT(cb, "Callback is not valid!");

    return InsertCallback(m_callbacks[static_cast<size_t>(event)], cb);
}

void Application::UnregisterEventCallback(Events::EventTypes event, [[maybe_unused]] size_t id)
{

    auto& events = m_callbacks[static_cast<size_t>(event)];

#    if NILAI_EVENTS_MAX_CALLBACKS == 1
    events = {};
#    else
    NILAI_ASSERT(id < s_maxEventCbCount, "Invalid callback ID!");
    events[id] = {};
#    endif
}

size_t Application::InsertCallback(Application::EventCallbacks& events,
                                   const Events::EventFunction& cb)
{
#    if NILAI_EVENTS_MAX_CALLBACKS == 1
    if (!events.Used)
    {
        events = {cb, true};
        return 0;
    }
    return -1;
#    else
    // Find the first free slot in the array.
    EventCallbacks::iterator it =
      std::find_if(events.begin(), events.end(), [](const CallbackSlot& f) { return !f.Used; });

    // If a slot is found:
    if (it != events.end())
    {
        // Place the callback there.
        *it = {cb, true};
        // Return the ID of the callback.
        return std::distance(events.begin(), it);
    }

    // No slot found.
    return -1;
#    endif
}

void Application::DispatchEvent(Events::Event* data)
{
    const auto& events = m_callbacks[static_cast<size_t>(data->Type)];
#    if NILAI_EVENTS_MAX_CALLBACKS == 1
    events.F(data);
#    else
    for (const auto& [cb, used] : events)
    {
        if (cb(data))
        {
            return;
        }
    }
#    endif
}

#endif

[[noreturn]] void Application::Run()
{
    std::for_each(
      m_modules.begin(), m_modules.end(), [](const ModuleInfo& module) { module.Mod->OnAttach(); });

    while (true)
    {
        OnRun();
    }

    std::for_each(
      m_modules.begin(), m_modules.end(), [](const ModuleInfo& module) { module.Mod->OnDetach(); });
}

void Application::OnRun()
{
    NILAI_PROFILE_FUNCTION();
    for (auto& module : m_modules)
    {
        module.Mod->Run();
    }

    if (m_modulesPendingDeletion)
    {
        m_modulesPendingDeletion = false;
        for (size_t id : m_deletionQueue)
        {
            auto& module = GetModule<Module>(id);
            module.OnDetach();
        }

        // Remove the modules to be removed from the list.
        std::erase_if(m_modules,
                      [&, this](const auto& m)
                      {
                          return std::find(m_deletionQueue.begin(),
                                           m_deletionQueue.end(),
                                           m.Id) != m_deletionQueue.end();
                      });
        m_deletionQueue.clear();
    }

#if defined(NILAI_ENABLE_PROFILING)
    Profiler::Report();
#endif
}

void Application::RemoveModule(size_t id)
{
    m_deletionQueue.push_back(id);
    m_modulesPendingDeletion = true;
}


[[noreturn]] void AbortionHandler([[maybe_unused]] int signal)
{
    // This function will in most cases be called by the STL, in place of the standard _exit().
    AssertFailed(reinterpret_cast<const uint8_t*>(__FILE__), __LINE__, 1);
    while (true)
    {
    }
}
}    // namespace Nilai

void AtExitForwarder()
{
    Nilai::AbortionHandler(0);
}
