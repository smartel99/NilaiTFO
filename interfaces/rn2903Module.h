/**
 * @addtogroup rn2903Module.h
 * @{
 *******************************************************************************
 * @file	rn2903Module.h
 * @author	Samuel Martel
 * @brief
 * Created on: Apr 9, 2021
 *******************************************************************************
 */

#ifndef RN2903MODULE_H_
#define RN2903MODULE_H_

#if defined(NILAI_USE_RN2903)
#if !defined(NILAI_USE_UART)
#error The UART module must be enabled!
#endif
/***********************************************/
/* Includes */
#include "shared/defines/module.hpp"

#include "shared/drivers/uartModule.hpp"


/***********************************************/
/* Defines */
class Rn2903Module : public cep::Module
{
public:
    Rn2903Module(const std::string& label, UartModule* uart);
    virtual ~Rn2903Module() override = default;

    virtual bool               DoPost() override;
    virtual void               Run() override;
    virtual const std::string& GetLabel() const override { return m_label; }

    void Transmit(const std::string& pkt);

private:
    std::string m_label = "";
    UartModule* m_uart  = nullptr;
};

/***********************************************/
/* Function declarations */


#endif
/**
 * @}
 */
/* END OF FILE */
#endif /* RN2903MODULE_H_ */
