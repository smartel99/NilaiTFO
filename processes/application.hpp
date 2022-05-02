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

#    include "../defines/Assertion.h"
#    include "../defines/module.hpp"

#    include <csignal>

#    if defined(NILAI_USE_EVENTS)
#        include "../defines/Core.h"
#        include "../defines/Events/Events.h"

#        include <array>
#        include <functional>
#    endif

namespace cep
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

    virtual void Init() {}
    virtual bool DoPost() { return false; }
    virtual void Run();

    template<IsModule T, typename... Args>
    Ref<T> AddModule(Args&&... args)
    {
        if (m_modules.size() >= NILAI_MAX_MODULE_AMOUNT)
        {
            // Too many modules!
            return nullptr;
        }
        ModuleInfo m;
        Ref<T> module = CreateRef<T>(std::forward<Args>(args)...);
        m.Mod       = module;
        m.Mod->m_id = m_lastId;
        m.Id        = m_lastId;

        m_modules.push_back(m);

        ++m_lastId;

        return module;
    }

    void        RemoveModule(size_t id);
    Ref<Module> GetModule(size_t id);

#    if defined(NILAI_USE_EVENTS)
    /**
     * @brief Dispatches a software event to the callbacks bound to the specified channel.
     * @param ch The event channel to dispatch the event on.
     */
    void TriggerSoftEvent(Events::SoftwareEvents ch);

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

    static Application* s_instance;

protected:
    //! Pointers to the modules of the application.
    std::vector<ModuleInfo> m_modules = {};
};

}    // namespace cep

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
