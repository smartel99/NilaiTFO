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
#include <string>
#include <vector>

// Explanation of static_cast: https://stackoverflow.com/a/1255015/11443498
#define SPI1_MODULE    static_cast<SpiModule*>(Application::GetModule("spi1"))

#define ADS_MODULE      static_cast<AdsModule*>(Application::GetModule("ads"))

namespace cep
{
// Forward declaration.
class Module;
}

/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class Application
{
public:
    Application() = default;
    ~Application() = default;

    static void Init();
    static void Run();

    void AddModule(cep::Module* newModule)
    {
        m_modules.push_back(newModule);
    }

    static cep::Module* GetModule(const std::string& moduleName);

    
private:
    static Application*        s_instance;
    std::vector<cep::Module*> m_modules;
    
    
private:
    void InitializeHAL();
    void InitializeServices();
    void InitializeModules();
};


/*************************************************************************************************/
/**
 * @}
 * @}
 */
#endif
/* ----- END OF FILE ----- */
