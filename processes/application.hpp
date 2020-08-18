/**
 * @addtogroup  processes
 * @{
 * @addtogroup  application
 * @{
 *
 * @file        application.hpp
 * @author      Pascal-Emmanuel Lachance
 * @p           https://www.github.com/Raesangur
 * @date        2020/07/06  -  09:02
 */
#pragma once
/*************************************************************************************************/
/* File includes ------------------------------------------------------------------------------- */
#include "shared/defines/pch.hpp"
#include "shared/defines/module.hpp"


/*************************************************************************************************/
/* Classes ------------------------------------------------------------------------------------- */
class Application
{
    /*********************************************************************************************/
    /* Private member variables ---------------------------------------------------------------- */
private:
    static Application        s_instance;
    std::vector<cep::Module*> m_modules{};


    /*********************************************************************************************/
    /* Constructor ----------------------------------------------------------------------------- */
public:
    Application()  = default;
    ~Application() = default;


    /*********************************************************************************************/
    /* Public member functions ----------------------------------------------------------------- */
    static void Init()
    {
        s_instance.InitializeHAL();
        s_instance.InitializeServices();
        s_instance.InitializeModules();
    }
    static void Run()
    {
        while (true)
        {
            for (cep::Module* module : s_instance.m_modules)
            {
                module->Run();
            }
        }
    }

    void AddModule(cep::Module* newModule)
    {
        m_modules.push_back(newModule);
        newModule->Init(m_modules.size() - 1);
    }

    [[nodiscard]] static cep::Module* GetModule(std::size_t instanceIndex)
    {
        return s_instance.m_modules[instanceIndex];
    }
    [[nodiscard]] static cep::Module* GetModule(const std::string_view moduleName)
    {
        /* clang-format off */
        auto namedSearch = [=](const cep::Module* currentModule)
                           {
                               return currentModule->GetName() == moduleName;
                           };
        /* clang-format on */

        auto& vec      = s_instance.m_modules;
        auto  iterator = std::find_if(vec.begin(), vec.end(), namedSearch);

        return *iterator;
    }


    /*********************************************************************************************/
    /* Private member functions ---------------------------------------------------------------- */
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
/* ----- END OF FILE ----- */
