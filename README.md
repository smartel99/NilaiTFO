/**
# How to create a new module:
(all instructions are in the `.hpp` file unless specified otherwise)

------------------------------------
### 1.0 Class declaration:
Create a class with your module name
```cpp
    class TestModule
```

Inherit all functions of the base module class
(Do not forget to include the `defines/module.hpp` file)
(The `public` keyword in inheritance specifies that the functions of the base module class must be
publicly available through the `TestModule` class).
```cpp
    class TestModule : public cep::Module
```


------------------------------------
### 1.1 Constructor:
Create the constructor, that should explicitely call the base module constructor.
(Other parameters can be added to the constructor)
```cpp
    public:
      TestModule() : cep::Module{"moduleName"} { Other initializations goes here }
      TestModule(uint32_t testParameter) : cep::Module{"moduleName"} { Other initializations goes here }
```


------------------------------------
### 1.2 Instance creation:
Create a private `static` vector of integer (use size_t), named `s_modules`, in the `.cpp` file.
```cpp
    static std::vector<size_t> s_modules{};
```

And then create accessors, that will use this vector to access the Application's vector.
(`SetInstance()` can be marked `SETTER` to force inlining)
(`GetInstance()` can be marked `GETTER` to force inlining and return value acquisition)
(`Set_Instance()` and `Remove_Instance()` are configured as `virtual` in the `cep::Module` class, and
 must be marked as `override` when declaring them in your new module.)
In the `.hpp` file:
```cpp
    public:
      GETTER static TestModule* GetInstance(size_t moduleIndex);
    private:
      SETTER void Set_Instance(size_t instanceIndex) override;
      size_t Remove_Instance(size_t moduleIndex) override;
```
In the `.cpp` file:
```cpp
    static std::vector<size_t> s_modules{};

    GETTER TestModule* TestModule::GetInstance(size_t moduleIndex)
    {
        return dynamic_cast<TestModule*>(Application::GetModule(s_modules[moduleIndex]));
    }
    ALWAYS_INLINE void TestModule::SetInstance(size_t instanceIndex)
    {
        m_moduleIndex = s_modules.size();
        s_modules.push_back(instanceIndex);
    }
    ALWAYS_INLINE size_t TestModule::RemoveInstance(size_t moduleIndex)
    {
        /* Remove module index from `s_modules` vector using the erase-remove idiom */
        s_modules.erase(std::remove(s_modules.begin(), s_modules.end(), moduleIndex), s_modules.end());

        return s_modules.size();
    }
```
(If you know you're making a module which will only have a single instance, you can add a default
 parameter to the `GetInstance()` declaration `static TestModule* GetInstance(size_t index = 0);`)


If you want to give easy access to your instances, you can provide macros to get the instances.
```cpp
    #define TESTMODULE1     TestModule::GetInstance(0)
    #define TESTMODULE2     TestModule::GetInstance(1)

    #define TESTMODULE      TestModule::GetInstance()   // Using default parameter
```


------------------------------------
### 1.4 Task Handler:
Create the task handler (this function will be called whenever it's its turn in the scheduling).
```cpp
    void TaskHandler() override
    {
	    // Code here will be executed at every program loop
    }
```


------------------------------------
### 1.5 Init Function:
An `Init()` function can be provided to add additionnal initialization (over the constructor's initializations).
It is however advised to make most initialization in the constructor.
This `Init()` function must call the `Enable()` and the `SetInstance()` function. 
It is created by default with all necessary functionalities in the base `Module` class, but can be overwritten.
```cpp
    void Init(size_t index) override
    {
        // Initialize stuff
        SetInstance(index);
        Enable();
    }
```

This `Init()` function is automatically called in the `AddModule` function.

You can also override a `DeInit()` function, but your module's destructor should be favored.
(`DeInit()` should call `Disable()` and `RemoveInstance()`)
```cpp
void DeInit() override
{
    Disable();
    RemoveInstance(m_moduleIndex);
}
```


------------------------------------
### 1.6 First run
If there is code you wish to execute only during the first run of the `TaskHandler`, you can check
the `FirstRun()` function (inherited from the `Module` class).
```cpp
    if (FirstRun() == true)
    {
        // This code will execute only the first iteration.
    }
```

It is still advised to do most initialization in the constructor and `Init()` function, but there
can be cases where this might be useful.


------------------------------------
### 1.7 Public member functions:

In the function body, use the `CHECK_ENABLED` macro to ensure the function should be called.
Verification of parameters should also be done in the function, using the `CEP_ASSERT` macro for
assertion (if result is `false`, break the program and print the message).
(The `CEP_ASSERT_NULL` macro also exists, specialized in null checking, with a default message.)

In the `.hpp` file:
```cpp
    public:
        void BlinkOpen(const char* somePtr);
```

The actual implementation of the functions should be done in the `.cpp` file.
```cpp
    void TestModule::BlinkOpen(const char* somePtr);
    {
        CHECK_ENABLED();
        CEP_ASSERT_NULL(somePtr);
        CEP_ASSERT(*somePtr == 'H' || *somePtr == 'h', "The string must begin with an H");
        
        /* Do stuff */
    }
```


------------------------------------
### 1.8 Accessors:
Data should be accessed via accessor functions.

First, create your data:  
(You can assign default values here, or initialize them via the constructor)
```cpp
    private:
        uint32_t m_firstDelay  = 1000;
        uint32_t m_secondDelay = 250;
```

Then, you create a read and write (`Get()` and `Set()`) accessor functions for each member.
```cpp
  public:
    GETTER uint32_t GetFirstDelay() const noexcept { return m_firstDelay; }
    SETTER void SetFirstDelay(uint32_t value) noexcept { m_firstDelay = value; }

    GETTER uint32_t GetSecondDelay() const noexcept { return m_secondDelay; }
    SETTER void SetSecondDelay(uint32_t value) noexcept { m_secondDelay = value; }
```
(The `GETTER` and `SETTER` macros are used to force inlining and add more efficient warnings, and are not required)
(It is good practice to mark these with `const` and `noexcept` attributes, but these can also be omitted, especially `noexcept`)


------------------------------------
### 1.9 Calling functions:
Calling a function of module `TestModule` from another module would go like this:
```cpp
    // Using a macro to access the instance
    uint32_t firstDelay = TESTMODULE1->GetFirstDelay();
    TESTMODULE1->SetSecondDelay(1000);
    TESTMODULE1->BlinkOpen("Hello World!");

    // Directly using instance accessor
    uint32_t firstDelay = TestModule::GetInstance(0)->GetFirstDelay();
    TestModule::GetInstance(0)->SetSecondDelay(1000);
    TestModule::GetInstance(0)->BlinkOpen("Hello World!");
```

Default functions implemented in the `Module` class are also available in this way:
```cpp
    if (TESTMODULE2->IsEnabled() == true)
    
    if (TestModule::GetInstance(1)->IsEnabled() == true)
```


**IMPORTANT: Do not call the `TaskHandler`, `Run`, `Init` or `DeInit` functions directly**


------------------------------------
### 1.10 Driver Module
When creating driver classes (such as UART, I2C or CAN), a specialized class `DriverModule` was created.  
Your driver module can then inherit from `cep::DriverModule` instead of `cep::Module`.

`DriverModule` is a templated class that takes as template parameters an HAL handle, a status enum, and two types of packets for reception and transmission respectively.
```cpp
template<typename HandlerType, typename StatusType, typename RxPacket, typename TxPacket = RxPacket>
class DriverModule : public cep::Module
``` 

A way of using it to create a drive module could be:
```cpp 
using DriverModuleType = cep::DriverModule<UART_HandleTypeDef, UART::Status, std::vector<uint8_t>>;
class UartModule : public DriverModuleType
```

The `DriverModule` class contains:
- the handle `m_handle`
- the status `m_status`
- two vectors of data `m_rxBuffer` and `m_txBuffer`
- a vector of callback functions `m_callbacks`.  
(Callback functions are `std::function<void(const RxPacket&)>`)

As well as definitions for functions to receive and transmit packets (actually simply add to the buffer), add, remove and call callback functions, the `TaskHandler()` function, and accessors for the handle, the status and the buffers.


------------------------------------
### 2.0 Application
The `Application` class has ownership of all the modules.  
It will call each subscribed module's `TaskHandler()` during its run loop.  
In order to create a module subscribed to the `Application` class,
use the function `AddModule()` in the `Application::InitializeModules()` function.  
(The `static_cast` turns a specialized module into a generic `Module` class instance, and is used to remove warnings).
```cpp
    void Application::InitializeModules()
    {
        //...
        AddModule(static_cast<cep::Module*>(new ModuleA{}));
        AddModule(static_cast<cep::Module*>(new ModuleB{115200}));
        AddModule(static_cast<cep::Module*>(new ModuleC{&huart1, "Hello World"}));
        //...
    }
```

------------------------------------
### 2.1 Application's other initializations
The `Application` class also has 2 other functions used for initialisations, `Application::InitializeHAL()` and `Application::InitializeServices`.

The HAL peripherals are not to be enabled in the `main()`, but rather in the `InitializeHAL()` function
```cpp
    void Application::InitializeHAL()
    {
        //...
        MX_GPIO_Init();
        MX_USART1_UART_Init();
        MX_CAN1_Init();
        //...
    }
```

If you have non-modular* services, they can be initialized/started in the `InitializeServices()` function
\*(not inheriting from the `cep::Module` class)
```cpp
    void Application::InitializeServices()
    {
        //...
        Log::Init();
        SYSTICK.Start();
        //...
    }
```


------------------------------------
### 2.2 Application's run loop
Each module subscribed to the `Application` class has its `TaskHandler` called at most once every systick, in a round-robin scheduler fashion.  
If the `TaskHandler()` function of a module takes too long to execute, it may stall the other modules.  
The timing of the `TaskHandler` is thus not exact to the systick and should not be relied upon.  
(For time-sensitive functions, see the ticks modules in section `3.0`)


------------------------------------
### 3.0 Ticks
Two Tickers are provided to handle time, `SysTick` and `FastTick`.  
`SysTick` ticks every milliseconds.  
`FastTick` ticks every 100 microseconds.        __FASTTICK IS CURRENTLY NOT ENABLED__

The Tickers use two different units to handle their time (see section `3.1`).

They provide the following functionalities:
- Getting the current tick
- Checking if a tick has increased since the last time it was checked
- Finding if a timeout has been hit and finding timeout tick (see section `3.2` for the `Timeout` class)
- Handling callbacks at specific ticks
(callback functions are `std::function<void(TickType)>`)

To access the tickers, you can use the `SYSTICK` and `FASTTICK` macros
```cpp
    SysTick_t currentTick = SYSTICK.Now();
    SysTick_t currentTick = FASTTICK.Now();     // Will be converted to SysTick_t
    SYSTICK.AddCallback(myFunction, SYSTICK.Now() + 100)    // Will call `myFunction` in 100 systicks
```


------------------------------------
### 3.1 Tick units
To go with the 2 different tickers, there are 2 tick units, `SysTick_t` and `FastTick_t`.  
The different unit types can be converted into one another with no problems (at compile-time)
```cpp
    SysTick_t mySysTick = 1000;
    FastTick_t myFastTick = FASTTICK.Now();
    mySysTick = mySysTick + myFastTick;     // The `+=` operator is not yet overloaded
    mySysTick = mySysTick + 1000;           // The `+=` operator is not yet overloaded

    void myFunction(SysTick_t val);
    myFunction(myFastTick);                 // Will convert to `SysTick_t`
```

To create a tick unit variable, you can also use the literal operators `_SysTicks` and `_FastTicks`
```cpp
    myFunction(100);            // Will call with 100 systicks
    myFunction(100_FastTicks);  // Will convert into SysTicks

    auto myTick = 1000_SysTicks;    // auto is `SysTick_t`

    #define DELAY 100_SysTicks
    myFunction(DELAY);
```


------------------------------------
### 3.2 Timeouts
There also exists a `Timeout` class, using the `FastTick` ticker by default.  
The `Timeout` class allows to create callback functions and check if the timeout was hit.  
You can start a `Timeout` with the `START_TIMEOUT` macro.

```cpp
    Timeout myTimeout = START_TIMEOUT(FASTTICK.Now() + 1000_FastTicks);
    Timeout myTimeoutWithCallback = START_TIMEOUT(SYSTICK.Now() + 1000, myFunction);

    Timeout<SysTick_t> mySysTickTimeout = START_TIMEOUT(1000);  // Will timeout when SysTick 100 is hit

    if (myTimeout.WasHit() == true)
    {
        // Do something
    }
```

When the timeout is hit, unless it has a specified callback, it will print a `WARNING` message.  
(the callback function is a `std::function<void(void)>`)



------------------------------------
### 3.3 Logging
The logging library used is [`spdlog`](https://github.com/gabime/spdlog), based around
[`libfmt` ](https://github.com/fmtlib/fmt)
We've redirected `spdlog`'s output to the UART port.

A wrapper class, `Log`, is provided to access 5 different logging levels:
   * Debug
   * Info
   * Warning
   * Error
   * Critical

And helper macros are also provided to access these levels
(`LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR` and `LOG_CRITICAL`)
Is it advised that you use the macros directly instead of calling the static functions of the
`Log` class, as these macros will fill in the required `__FILE__`, `__FUNCTION__` and `__LINE__` fields.

To use a macro to print a simple message, you can simply pass it as a macro parameter
```cpp
    LOG_INFO("Hello World");

    std::string message{"uwu"};
    LOG_CRITICAL(message);
```

Messages will be printed with the following format:
```
    // In Debug mode:
    [Master][I] - [Test.hpp:271 (TaskHandler)] Hello World\r\n
    [Master][C] - [Test.hpp:274 (TaskHandler)] uwu\r\n

    // In Release mode:
    [Master][I] - Hello World\r\n
    [Master][C] - uwu\r\n
```

To add parameters, formatting follows [`libfmt` standard](https://fmt.dev/latest/syntax.html).
```cpp
    LOG_DEBUG("data size: {}", sizeof(int));
    LOG_INFO("Hello {}!", "World");
    LOG_WARNING("Received {} bytes when expected size was {} bytes",
                receivedData.Size(), expectedSize);
    LOG_INFO("Tension was {}{}", tension, 'V');
``` 
```
    [Master][D] - data size: 4\r\n
    [Drive ][I] - Hello World!\r\n
    [Master][W] - Received 14 bytes when expected size was 16 bytes\r\n
    [Master][I] - Tension was 3.141592V\r\n
```

(More examples on syntax, alignment, width, types, signs, floating-point precision, hexadecimal
 representation, number of digits, specific arguments references and more can be found on [the
 official `libfmt` documentation](https://fmt.dev/latest/syntax.html)).




*/