/**
 ******************************************************************************
 * @addtogroup max14763Module
 * @{
 * @file    max14763Module
 * @author  Lou-Gabriel Gaucher
 * @brief   Header for the max14763Module module.
 *
 * @date 9/23/2020 3:03:33 PM
 *
 ******************************************************************************
 */

#ifndef _max14763Module
#    define _max14763Module

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
namespace MAX14763
{
/**
 * @struct  Config
 * @brief
 */
struct Config
{

    std::function<void(bool)> setSELFunc = { };

    std::function<bool( )> getSELFunc = { };
};
}    // namespace MAX14763

class Max14763Module : public cep::Module
{
public:
    Max14763Module( ) = delete;
    Max14763Module(const MAX14763::Config& config, const std::string& label);
    virtual ~Max14763Module( ) override = default;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    void SetSEL(bool state);

    bool GetSEL( );

private:
    MAX14763::Config m_config;
    std::string      m_label;
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _max14763Module */
/**
 * @}
 */
/****** END OF FILE ******/
