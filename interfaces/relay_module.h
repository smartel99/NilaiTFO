/**
 ******************************************************************************
 * @addtogroup RelayModule
 * @{
 * @file    RelayModule
 * @author  Lou-Gabriel Gaucher
 * @brief   Header for the RelayModule module.
 *
 * @date 9/24/2020 3:03:33 PM
 *
 ******************************************************************************
 */

#ifndef _relayModule
#    define _relayModule

#    if defined(NILAI_USE_RELAY)
/*****************************************************************************/
/* Includes */
#        include <functional>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
namespace Nilai
{
class Relay
{
public:
    Relay() = default;
    Relay(const std::function<void(bool)>& set, const std::function<bool()>& get)
    : m_set(set), m_get(get)
    {
    }

    void SetSetFunc(const std::function<void(bool)>& func) { m_set = func; }
    void SetGetFunc(const std::function<bool()>& func) { m_get = func; }

    void Set(bool state)
    {
        if (m_set)
        {
            m_set(state);
        }
    }

    bool Get()
    {
        if (m_get)
        {
            return m_get();
        }
    }

private:
    std::function<void(bool)> m_set = {};
    std::function<bool()>     m_get = {};
};
}    // namespace Nilai

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#    endif /* _relayModule */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
#pragma once
