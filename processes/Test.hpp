/**
 * @addtogroup  Test
 * @{
 * @file        test.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 *
 * @brief       Test file for modules, with instructions on how to use them.
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "defines/module.hpp"


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */

class TestModule : public cep::Module
{
    /*********************************************************************************************/
    /* Private member variables ---------------------------------------------------------------- */
private:
    std::uint32_t m_firstDelay  = 1000;
    std::uint32_t m_secondDelay = 250;


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    TestModule() : Module("uwu") {}


    /*********************************************************************************************/
    /* Handler --------------------------------------------------------------------------------- */
    void TaskHandler() override
    {
        if (FirstRun() == true)
        {
            LOG_WARNING("Hello World");
        }

        BlinkOpenImpl();
        HAL_Delay(GetFirstDelay());
        BlinkCloseImpl();
        HAL_Delay(GetSecondDelay());
    }


    /*********************************************************************************************/
    /* Accessors ------------------------------------------------------------------------------- */
#pragma region Accessors
    [[nodiscard]] static std::uint32_t GetFirstDelay() { return GetInstance().m_firstDelay; }
    static void SetFirstDelay(std::uint32_t value) { GetInstance().m_firstDelay = value; }

    [[nodiscard]] static std::uint32_t GetSecondDelay() { return GetInstance().m_secondDelay; }
    static void SetSecondDelay(std::uint32_t value) { GetInstance().m_secondDelay = value; }
    
    static TestModule& GetInstance();
#pragma endregion


    /*********************************************************************************************/
    /* Public member functions ----------------------------------------------------------------- */
#pragma region Public member functions
    static void BlinkOpen()
    {
        CHECK_ENABLED();
        GetInstance().BlinkOpenImpl();
    }
    static void BlinkClose()
    {
        CHECK_ENABLED();
        GetInstance().BlinkCloseImpl();
    }
#pragma endregion


    /*********************************************************************************************/
    /* Implementation of member functions ------------------------------------------------------ */
private:
    void BlinkOpenImpl();
    void BlinkCloseImpl();
};


/*************************************************************************************************/
/**
 * @}
 */
/* ----- END OF FILE ----- */
