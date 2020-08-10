/**
 * @addtogroup  Test
 * @{
 * @file        test.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 *
 * @brief       Test file for modules
 */
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "defines/_pch.hpp"
#include "processes/Test.hpp"

/*************************************************************************************************/
/* Instance creation --------------------------------------------------------------------------- */
static TestModule Instance{};
TestModule& TestModule::GetInstance() { return Instance; }


/*************************************************************************************************/
/* Member functions implementation ------------------------------------------------------------- */
void TestModule::BlinkOpenImpl()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
}
void TestModule::BlinkCloseImpl()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
}



/*************************************************************************************************/
/**
 * @}
 */
/* ----- END OF FILE ----- */
