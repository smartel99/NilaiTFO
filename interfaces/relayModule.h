/**
 ******************************************************************************
 * @addtogroup relayModule
 * @{
 * @file    relayModule
 * @author  Lou-Gabriel Gaucher
 * @brief   Header for the relayModule module.
 *
 * @date 9/24/2020 3:03:33 PM
 *
 ******************************************************************************
 */

#ifndef _relayModule
#    define _relayModule

/*****************************************************************************/
/* Includes */
#    include "defines/module.hpp"
#    include <functional>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
namespace RELAY
{
    /**
     * @struct  Config
     * @brief
     */
    struct Config
    {
        std::function<void(bool)> setENFunc = {};
        std::function<bool()> getENFunc = {};

        std::function<void(bool)> setContactor1Func = {};
        std::function<void(bool)> setContactor2Func = {};
        std::function<bool()> getContactor1Func = {};
        std::function<bool()> getContactor2Func = {};
    };
}    // namespace RELAY

class RelayModule: public cep::Module
{
public:
    RelayModule() = delete;
    RelayModule(const RELAY::Config &config, const std::string &label);
    virtual ~RelayModule() override = default;

    virtual void Run() override;
    virtual const std::string& GetLabel() const override
    {
        return m_label;
    }

    void SetEN(bool state);
    bool GetEN();

    void SetContactor1(bool state);
    void SetContactor2(bool state);
    bool GetContactor1();
    bool GetContactor2();

private:
    RELAY::Config m_config;
    std::string m_label;
}
;

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _relayModule */
/**
 * @}
 */
/****** END OF FILE ******/
#pragma once
