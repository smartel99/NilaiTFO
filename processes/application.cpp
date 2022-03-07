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

namespace cep
{
Application* Application::s_instance = nullptr;

Application::Application()
{
    std::signal(SIGABRT, &AbortionHandler);

    s_instance = this;
#if defined(NILAI_USE_EVENTS)
    for (auto& event : m_callbacks)
    {
        for (auto& cb : event)
        {
            cb = &DefaultEventCallback;
        }
    }
#endif
}

#if defined(NILAI_USE_EVENTS)
size_t Application::RegisterEventCallback(Events::EventTypes               event,
                                          const Application::CallbackFunc& cb)
{
    CEP_ASSERT(cb, "Callback is not valid!");

    return InsertCallback(m_callbacks[(size_t)event], cb);
}

void Application::UnregisterEventCallback(Events::EventTypes event, size_t id)
{
    CEP_ASSERT(id < s_maxEventCbCount, "Invalid callback ID!");

    auto& events = m_callbacks[(size_t)event];

    events[id] = DefaultEventCallback;
}

void Application::TriggerSoftEvent(Events::SoftwareEvents ch)
{
    Events::ExtEvent e;
    e.IsFromSoft = true;
    e.SrcCh      = (uint8_t)ch;

    DispatchEvent((Events::EventTypes)ch, &e);
}

size_t Application::InsertCallback(Application::EventCallbacks&     events,
                                   const Application::CallbackFunc& cb)
{
    // Find the first free slot in the array.
    EventCallbacks::iterator it = std::find_if(events.begin(),
                                               events.end(),
                                               [](const CallbackFunc& f)
                                               {
                                                   auto fptr = f.target<bool (*)(Events::Event*)>();
                                                   return fptr && *fptr == &DefaultEventCallback;
                                               });

    // If a slot is found:
    if (it != events.end())
    {
        // Place the callback there.
        *it = cb;
        // Return the ID of the callback.
        return std::distance(events.begin(), it);
    }

    // No slot found.
    return -1;
}

void Application::DispatchEvent(Events::EventTypes e, Events::Event* data)
{
    const auto& events = m_callbacks[(size_t)e];
    for (const auto& cb : events)
    {
        if (cb(data))
        {
            return;
        }
    }
}

#endif

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
