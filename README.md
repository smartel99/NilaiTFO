# Nilai, The Fast One

A framework for embedded applications in modern C++.

NilaiTFO offers a framework based around the concepts of modules and application and is event-driven.

Mainly intended to be used in conjunction
with [STMicro's STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html), but the complete support of most
microcontroller is planned.

Under continuous development!

## Fully Supported Microcontroller

- STM32F405
- STM32L452
- GD32F405

## Limited Support

- STM32F4/GD32F4
- STM32F7/GD32F7
- STM32L4

## Portable to with modification

- Every STM32 microcontroller

## Getting Started

1. Configure your microcontroller
   using [STMicro's STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
2. Configure Nilai for your application following the template found under `processes/NilaiTFO_config_Template.h`
    1. Comment/Un-comment the defines to enable/disable the various options
3. Add the configuration header to the compiler's include files (using `-include` with GCC)
4. Create an application
5. Add modules to the application
6. Run the application

## Creating an Application

To create an application, simply make a class that inherits from `Nilai::Application`.

The basic `Application` handles the dispatch of events and the execution of every module.
Two methods must however be overridden by your application:

- `Application::OnInit`: Called once at the start of the program, this is where modules should be added to the
  application.
- `Application::OnPost`: Called after the initialisation, this is where each module should verify that it is ready to
  operate.
    - If `Application::OnPost` returns false, the microcontroller enters an infinite loop.

Additionally, `Application::Run` can be overridden to specify custom runtime behaviors. The overridden method should
however still call `Application::OnRun` since it calls the run method of every module, as well as handling
the removal of modules, if needed.

### Example

```cpp
#include "Nilai.h"

class MyApplication : public Nilai::Application
{
public:
    ~MyApplication() override = default;

    void OnInit() override
    {
        m_pin = {LED_GPIO_Port, LED_Pin};

        Nilai::Ref<UartModule> uart = AddModule<UartModule>(&huart1, "uart");
        m_logger                    = Nilai::CreateRef<Nilai::Logger>(uart3);
        m_logger->Log("Hello, world!");
        
        AddModule<MyModule>(123);
    }

    bool OnPost() override
    {
        m_logger->Log("\n\r----- Started POST...\n\r");
        bool allModulesPassedPost = true;

        for (auto& module : m_modules)
        {
            if (!module.Mod->DoPost())
            {
                allModulesPassedPost = false;
            }
        }

        if (allModulesPassedPost)
        {
            m_logger->Log("----- POST OK!\n\r");
        }
        else
        {
            m_logger->Log("----- POST ERROR! %0.3f seconds.\n\r");
        }

        return allModulesPassedPost;
    }

    void Run() override
    {
        while (true)
        {
            m_pin.Toggle();

            Application::OnRun();
        }
    }

private:
    Nilai::Pin                m_pin;
    Nilai::Ref<Nilai::Logger> m_logger = nullptr;
};
```

## Creating Modules

### Example

```cpp
#include "Nilai.h"

class MyModule : public Nilai::Module
{
public:
    MyModule(int number) : m_number(number)
    {
    }

    ~MyModule() override = default;

    /**
     * @brief Called when the module is being attached to the application.
     */
    void OnAttach() override 
    {
        LOG_INFO("MyModule::OnAttach()");
    }

    /**
     * @brief Called when the module is being detached from the application.
     */
    void OnDetach() override 
    {
        LOG_INFO("MyModule::OnDetach()");
    }

    /**
     * @brief Called whenever an event happens.
     * @param type The type of event
     * @param event The event data object.
     * @returns True if the event was handled and shouldn't be propagated further.
     * @returns False if the event was not handled and/or should be propagated to the other modules.
     */
    bool OnEvent(Events::Event* event) override 
    {
        if(event->Type == Nilai::Events::EventTypes::Exti0)
        {
            auto* extEvent = reinterpret_cast<Nilai::Events::ExtEvent*>(event);

            if(extEvent->IsFromSoft)
            {
                LOG_INFO("Event triggered by software!");
            }
            else
            {
                LOG_INFO("Event triggered by External Interrupt 0!");
            }

            return true;
        } 
        return false; 
    }

    /**
     * @brief Called during the application startup.
     *
     * Use this function to verify the functionalities of the module and to check for problems.
     *
     * Returning false from this function causes the POST to fail, the application blocking.
     *
     * @return True if the POST passed, false otherwise.
     */
    bool DoPost() override 
    {
        if(m_number == 0)
        {
            LOG_ERROR("Number cannot be 0!");
            return false;
        } 
        return true; 
    }

    /**
     * @brief Called once every frame.
     */
    void Run() override 
    {
        ++m_number;
        LOG_INFO("Number is %i", m_number);
    }

private:
    int m_number = 0;
};
```

## Running the Application

Now that you have your application ready to run, you can make your `main.cpp`.
You can simply copy-paste the contents of the `main.c` file generated by STM32CubeMX.

```cpp
#include "Nilai.h"
#include "MyApplication.h"

int main()
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    // Make sure that SYSCLK isn't set to PLL before configuring it.
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);

    /* Configure the system clock */
    SystemClock_Config();
    PeriphCommonClock_Config();

    /* Initialize program */
    MyApplication app;

    app.Init();
    if (app.DoPost())
    {
        app.Run();
    }

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    Nilai::Pin led = {LED_GPIO_Port, LED_Pin};
    while (true)
    {
        led.Toggle();
        Nilai::Delay(100);
    }
}
```
