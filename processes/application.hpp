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

#    if defined(NILAI_USE_EVENTS)
#        include "../defines/Events/Events.h"
#        include <array>
#        include <functional>
#    endif

namespace cep
{
/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
[[noreturn]] void AbortionHandler(int signal);

class Application
{
#    if defined(NILAI_USE_EVENTS)
public:
    /**
     * @brief A callback function should return true if the event should not be propagated further,
     * i.e if the following callbacks in the list should not be called after this one.
     */
    using CallbackFunc = std::function<bool(Events::Event*)>;
#    endif
public:
    Application();
    virtual ~Application() = default;

    virtual void Init()   = 0;
    virtual bool DoPost() = 0;
    virtual void Run()    = 0;

#    if defined(NILAI_USE_EVENTS)
    /**
     * @brief Registers a callback to an event.
     * @param event The event to bind the callback to.
     * @param cb The callback function. This function should return true if the event should not be
     * propagated further.
     * @return The ID of the callback. This ID is used to unregister the callback.
     */
    size_t RegisterEventCallback(Events::EventTypes event, const CallbackFunc& cb);

    /**
     * @brief Unregisters a callback that has previously been registered to an event.
     * @param event The event to remove the callback from.
     * @param id The ID of the callback to be removed.
     */
    void UnregisterEventCallback(Events::EventTypes event, size_t id);

    /**
     * @brief Dispatches a software event to the callbacks bound to the specified channel.
     * @param ch The event channel to dispatch the event on.
     */
    void TriggerSoftEvent(Events::SoftwareEvents ch);

    template<typename T>
    void TriggerDataEvent(const T& data)
    {
        Events::DataEvent e(data);
        DispatchEvent(Events::EventTypes::DataEvent, &e);
    }
#    endif

#    if defined(NILAI_USE_EVENTS)
private:
    void DispatchEvent(Events::EventTypes e, Events::Event* data);

    static constexpr size_t s_maxEventCbCount = NILAI_EVENTS_MAX_CALLBACKS;
    static constexpr size_t s_numOfEvents     = (size_t)Events::EventTypes::Count;

    using EventCallbacks = std::array<CallbackFunc, s_maxEventCbCount>;
    std::array<EventCallbacks, s_numOfEvents> m_callbacks;

private:
    static bool   DefaultEventCallback(Events::Event*) { return false; }
    static size_t InsertCallback(EventCallbacks& events, const CallbackFunc& cb);
#    endif
};

}    // namespace cep

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
