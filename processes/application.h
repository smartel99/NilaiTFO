/**
 * @file        application.h
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

#        include <array>
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
        Ptr<Module> Mod = nullptr;
        size_t      Id  = 0;

        operator bool() const { return Mod != nullptr; }

        ModuleInfo(Ptr<Module> mod, size_t id) : Mod(std::move(mod)), Id(id) {}
    };

public:
    Application();
    virtual ~Application() = default;

    virtual void              OnInit() {}
    virtual bool              OnPost() { return false; }
    [[noreturn]] virtual void Run();

    virtual void OnRun();

    template<IsModule T, typename... Args>
    T& AddModule(Args&&... args)
        requires std::constructible_from<T, Args...>
    {
        ModuleInfo& m =
          m_modules.emplace_back(CreatePtr<Module>(new T(std::forward<Args>(args)...)), m_lastId);
        m.Mod->m_id = m_lastId;

        ++m_lastId;

        return *static_cast<T*>(m.Mod.get());
    }

    void RemoveModule(size_t id);

    template<IsModule T>
    T& GetModule(size_t id)
    {
        auto it = std::find_if(
          m_modules.begin(), m_modules.end(), [id](const auto& m) { return m.Id == id; });

        if (it != m_modules.end())
        {
            return *static_cast<T*>(it->Mod.get());
        }
        else
        {
            // Module not found.
            AssertFailed(reinterpret_cast<const uint8_t*>(__FILE__), __LINE__, 1);
        }
    }

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
        requires std::constructible_from<Events::DataEvent, T>
    {
        Events::DataEvent e(data);
        DispatchEvent(&e);
    }
    /**
     * @brief Dispatch an event to the other modules.
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
#endif
/* ----- END OF FILE ----- */
