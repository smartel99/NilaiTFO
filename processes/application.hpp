/**
 * @addtogroup  processes
 * @{
 * @addtogroup  application
 * @{
 *
 * @file        application.hpp
 * @author      Pascal-Emmanuel Lachance
 * @author      Samuel Martel
 * @date        2020/07/06  -  09:02
 */
#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */


namespace cep
{

/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class Application
{
public:
    virtual ~Application() = default;

    virtual void Init() = 0;
    virtual void Run() = 0;
    
    static void AssertFailed()
    {
        __builtin_trap();

        while (true)
        {
            // An assertion failed, block the program.
        }
    }
};


}

/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
