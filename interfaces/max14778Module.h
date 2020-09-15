/**
 ******************************************************************************
 * @addtogroup max14778Module
 * @{
 * @file    max14778Module
 * @author  Samuel Martel
 * @brief   Header for the max14778Module module.
 *
 * @date 9/15/2020 3:03:33 PM
 *
 ******************************************************************************
 */
#ifndef _max14778Module
#    define _max14778Module

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
namespace MAX14778 {
struct Config
{
    std::function<void(bool)> setEnAFunc = { };
    std::function<void(bool)> setEnBFunc = { };

    std::function<void(bool)> setSA0Func = { };
    std::function<void(bool)> setSA1Func = { };

    std::function<void(bool)> setSB0Func = { };
    std::function<void(bool)> setSB1Func = { };

    std::function<void(bool)> setAComFunc = { };
    std::function<void(bool)> setBComFunc = { };

    std::function<bool( )> getAComFunc = { };
    std::function<bool( )> getBComFunc = { };
};
}    // namespace MAX14778

class Max14778Module : public cep::Module
{
public:
    Max14778Module( ) = delete;
    Max14778Module(const MAX14778::Config& config, const std::string& label);
    virtual ~Max14778Module( ) override = default;

    virtual void               Run( ) override;
    virtual const std::string& GetLabel( ) const override { return m_label; }

    void SetEnA(bool state);
    void SetEnB(bool state);

    void SelectA0( );
    void SelectA1( );
    void SelectA2( );
    void SelectA3( );

    void SelectB0( );
    void SelectB1( );
    void SelectB2( );
    void SelectB3( );

    void SetACom(bool state);
    void SetBCom(bool state);

    bool GetACom( );
    bool GetBCom( );

private:
    MAX14778::Config m_config;
    std::string      m_label;
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#endif /* _max14778Module */
/**
 * @}
 */
/****** END OF FILE ******/
