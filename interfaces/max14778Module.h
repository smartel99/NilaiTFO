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
#    if defined(NILAI_USE_MAX14778)
/*****************************************************************************/
/* Includes */

#        include <functional>

/*****************************************************************************/
/* Exported defines */

/*****************************************************************************/
/* Exported macro */

/*****************************************************************************/
/* Exported types */
namespace MAX14778
{
/**
 * @struct  Config
 * @brief
 */
struct Config
{
    std::function<void(bool)> setEnAFunc = {};
    std::function<void(bool)> setEnBFunc = {};

    std::function<void(bool)> setSA0Func = {};
    std::function<void(bool)> setSA1Func = {};

    std::function<void(bool)> setSB0Func = {};
    std::function<void(bool)> setSB1Func = {};

    std::function<void(bool)> setAComFunc = {};
    std::function<void(bool)> setBComFunc = {};

    std::function<bool()> getAComFunc = {};
    std::function<bool()> getBComFunc = {};
};
}    // namespace MAX14778

class Max14778Module
{
public:
    Max14778Module() = default;
    Max14778Module(const MAX14778::Config& config);

    void SetEnA(bool state) const;
    void SetEnB(bool state) const;

    void SelectA0() const;
    void SelectA1() const;
    void SelectA2() const;
    void SelectA3() const;

    void SelectB0() const;
    void SelectB1() const;
    void SelectB2() const;
    void SelectB3() const;

    void SetACom(bool state) const;
    void SetBCom(bool state) const;

    bool GetACom() const;
    bool GetBCom() const;

private:
    MAX14778::Config m_config;
};

/*****************************************************************************/
/* Exported functions */

/* Have a wonderful day :) */
#    endif /* _max14778Module */
#endif
/**
 * @}
 */
/****** END OF FILE ******/
