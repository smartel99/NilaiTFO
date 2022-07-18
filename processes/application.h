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
#ifndef NILAI_APPLICATION_HPP
#    define NILAI_APPLICATION_HPP
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */

#    include "../defines/assertion.h"
#    include "../defines/module.h"

#    include "../defines/smart_pointers.h"

#    include <csignal>
#    include <type_traits>
#    include <vector>

#    if defined(NILAI_USE_EVENTS)
#        include "../defines/events/events.h"

#        include <functional>
#    endif

namespace Nilai
{
[[noreturn]] void AbortionHandler(int signal);
/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
template<typename T>
concept IsModule = std::is_base_of_v<Module, T>;


class Application
{
    struct ModuleInfo
    {
        Ref<Module> Mod = nullptr;
        size_t      Id  = 0;

        operator bool() const { return Mod != nullptr; }
    };

public:
    Application();
    virtual ~Application() = default;

    virtual void              OnInit() {}
    virtual bool              OnPost() { return false; }
    [[noreturn]] virtual void Run();

    virtual void OnRun();

    template<IsModule T, typename... Args>
    Ref<T> AddModule(Args&&... args)
    {
        if (m_modules.size() >= NILAI_MAX_MODULE_AMOUNT)
        {
            // Too many modules!
            return nullptr;
        }
        ModuleInfo m;
        Ref<T>     module = CreateRef<T>(std::forward<Args>(args)...);
        m.Mod             = module;
        m.Mod->m_id       = m_lastId;
        m.Id              = m_lastId;

        m_modules.push_back(m);

        ++m_lastId;

        return module;
    }

    void        RemoveModule(size_t id);
    Ref<Module> GetModule(size_t id);

#    if defined(NILAI_USE_EVENTS)
    /**
     * @brief Registers a callback to an event.
     * @param event The event to bind the callback to.
     * @param cb The callback function. This function should return true if the event should not be
     * propagated further.
     * @return The ID of the callback. This ID is used to unregister the callback.
     */
    size_t RegisterEventCallback(Events::EventTypes event, const Events::EventFunction& cb);

    /**
     * @brief Unregisters a callback that has previously been registered to an event.
     * @param event The event to remove the callback from.
     * @param id The ID of the callback to be removed.
     */
    void UnregisterEventCallback(Events::EventTypes event, size_t id);

    template<typename T>
    void TriggerDataEvent(const T& data)
    {
        Events::DataEvent e(data);
        DispatchEvent(&e);
    }
    /**
     * @brief Dispatch an event to the other modules.
     * @param e The event type.
     * @param data The event data.
     */
    void DispatchEvent(Events::Event* data);
#    endif

    static Application* Get() { return s_instance; }

private:
    size_t m_lastId = 0;
    //! List of the modules to remove from the application.
    std::vector<size_t> m_deletionQueue = {};

    bool m_modulesPendingDeletion = false;

#    if defined(NILAI_USE_EVENTS)
protected:
    struct CallbackSlot
    {
        Events::EventFunction F    = [](Events::Event*) { return false; };
        bool                  Used = false;
    };
#        if NILAI_EVENTS_MAX_CALLBACKS == 1
    using EventCallbacks = CallbackSlot;
#        else
    static constexpr size_t s_maxEventCbCount = NILAI_EVENTS_MAX_CALLBACKS;
    using EventCallbacks                      = std::array<CallbackSlot, s_maxEventCbCount>;
#        endif

    static constexpr size_t s_numOfEvents = static_cast<size_t>(Events::EventTypes::Count);
    std::array<EventCallbacks, s_numOfEvents> m_callbacks = {};

private:
    static size_t InsertCallback(EventCallbacks& events, const Events::EventFunction& cb);
#    endif

    static Application* s_instance;

protected:
    //! Pointers to the modules of the application.
    std::vector<ModuleInfo> m_modules = {};
};

}    // namespace Nilai

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
