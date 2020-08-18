/**
 * @addtogroup  defines
 * @{
 * @addtogroup  module
 * @{
 * @file        driverModule.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/08/18  -  09:31
 *
 * @brief       Base module for all drivers
 */
#pragma once
/*************************************************************************************************/
/* Includes ------------------------------------------------------------------------------------ */
#include "shared/defines/pch.hpp"
#include "shared/defines/module.hpp"


namespace cep
{


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
template<typename HandlerType, typename StatusType, typename RxPacket, typename TxPacket = RxPacket>
class DriverModule : public cep::Module
{
public:
    using Callback_t = std::function<void(const RxPacket&)>;

    /*********************************************************************************************/
    /* Protected member variables -------------------------------------------------------------- */
protected:
    HandlerType*            m_handle = nullptr;
    StatusType              m_status{};
    std::vector<RxPacket>   m_rxBuffer{};
    std::vector<TxPacket>   m_txBuffer{};
    std::vector<Callback_t> m_callbacks{};


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
    DriverModule(HandlerType* handler, const std::string_view moduleName)
    : cep::Module{moduleName}, m_handle{handler}
    {
    }
};


/*************************************************************************************************/
}; /* namespace cep */
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
