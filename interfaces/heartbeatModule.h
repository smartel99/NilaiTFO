﻿/**
 ******************************************************************************
 * @addtogroup heartbeatModule
 * @{
 * @file    heartbeatModule
 * @author  Samuel Martel
 * @brief   Header for the heartbeatModule module.
 *
 * @date 9/23/2020 1:45:10 PM
 *
 ******************************************************************************
 */
#ifndef _heartbeatModule
#define _heartbeatModule

#if defined(NILAI_USE_HEARTBEAT)
/*****************************************************************************/
/* Includes */
#include "defines/ledPattern.h"
#include "defines/module.hpp"
#include "defines/pin.h"

#include <string>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */

class HeartbeatModule : public cep::Module
{
public:
    HeartbeatModule(const Pin& pin, const std::string& label);
    virtual ~HeartbeatModule() = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

private:
    std::string m_label = "";
    Pin         m_led;

    cep::LedPattern m_defaultPattern{500, 500, -1};
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _heartbeatModule */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
