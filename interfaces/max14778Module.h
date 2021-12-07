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
    [[maybe_unused]] std::function<void(bool)> setEnAFunc = [](bool) {};
    [[maybe_unused]] std::function<void(bool)> setEnBFunc = [](bool) {};

    [[maybe_unused]] std::function<void(bool)> setSA0Func = [](bool) {};
    [[maybe_unused]] std::function<void(bool)> setSA1Func = [](bool) {};

    [[maybe_unused]] std::function<void(bool)> setSB0Func = [](bool) {};
    [[maybe_unused]] std::function<void(bool)> setSB1Func = [](bool) {};

    [[maybe_unused]] std::function<void(bool)> setAComFunc = [](bool) {};
    [[maybe_unused]] std::function<void(bool)> setBComFunc = [](bool) {};

    [[maybe_unused]] std::function<bool()> getAComFunc = []() -> bool { return false; };
    [[maybe_unused]] std::function<bool()> getBComFunc = []() -> bool { return false; };
};
}    // namespace MAX14778

class Max14778Module
{
public:
    Max14778Module() = default;
    explicit Max14778Module(MAX14778::Config config);

    [[maybe_unused]] void SetEnA(bool state) const;
    [[maybe_unused]] void SetEnB(bool state) const;

    [[maybe_unused]] void SelectA0() const;
    [[maybe_unused]] void SelectA1() const;
    [[maybe_unused]] void SelectA2() const;
    [[maybe_unused]] void SelectA3() const;

    [[maybe_unused]] void SelectB0() const;
    [[maybe_unused]] void SelectB1() const;
    [[maybe_unused]] void SelectB2() const;
    [[maybe_unused]] void SelectB3() const;

    [[maybe_unused]] void SetACom(bool state) const;
    [[maybe_unused]] void SetBCom(bool state) const;

    [[maybe_unused]] [[nodiscard]] bool GetACom() const;
    [[maybe_unused]] [[nodiscard]] bool GetBCom() const;

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
