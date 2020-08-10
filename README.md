
/**
# How to create a new module:

------------------------------------
### Class declaration:
Create a class with your module name
```cpp
    class TestModule
```

Inherit all functions of the base module class
(Do not forget to include the `defines/module.hpp` file)
(The `public` keyword in inheritance specifies that the functions of the base module class must be  publicly available through the `TestModule` class).
```cpp
    class TestModule : public cep::Module
```


------------------------------------
### Constructor:
Create the constructor, that should explicitely call the base module constructor.
```cpp
    TestModule() : Module("moduleName") { Other initializations goes here }
```


------------------------------------
### Instance creation:
Create a private `static` instance of the module, named `Instance`, in the `.cpp` file.
```cpp
    static TestModule Instance{};
```

And then create an accessor for this instance.
In the `.hpp` file:
```cpp
    public:
      static TestModule& GetInstance();
```
In the `.cpp` file:
```cpp
    TestModule& TestModule::GetInstance() { return Instance; }
```


------------------------------------
### Task Handler:
Create the task handler (this function will be called whenever it's its turn in the scheduling).
```cpp
    void TaskHandler() override
    {
	    // Code here will be executed at every program loop
    }
```


------------------------------------
### Init Function:
An `Init()` function can be provided to add additionnal initialization (over the constructor's initializations).
It is advised to make most initialization in the `Init()` function, as this will ensure that all system ressources are properly initialized (which is not the case in the module constructor, that should only initialize member variables).
This `Init()` function must call the `Enable()` function. 
It is created by default in the base `Module` class, but can be overwritten.
```cpp
    void Init() override
    {
        // Initialize stuff
        GetInstance().Enable();
    }
```

It is only after the `Init()` function that the module should really start.
You can also overload the `Init()` function to take parameters, if needed for initialization.

This `Init()` function should be called by the `InitializeModules()` function of the `Application` module, in `processes/application.cpp`. This call is included in the `ADD_MODULE` macro.
```cpp
    void Application::InitializeModules()
    {
        //...
		ADD_MODULE(ModuleA);
        ADD_MODULE(ModuleB, 115200);
        ADD_MODULE(ModuleC, &huart1, "Hello World");
        //...
    }
```


------------------------------------
### First run
If there is code you wish to execute only during the first run of the `TaskHandler`, you can check
the `FirstRun()` function (inherited from the `Module` class).
```cpp
    if (FirstRun() == true)
    {
        // This code will execute only the first iteration.
    }
```

It is still advised to do most initialization in the `Init()` function, but there can be cases 
where this might be useful.
You do not need to handle the `m_firstRun` flag of the `Module` class, it is lowered automatically
when calling the `FirstRun()` function for the first time.


------------------------------------
### Public member functions:
Since the instance is static, it cannot be accessed from other files.
However, static member functions can be accessed from other files, so we make use of forwarding functions to access the class and its data and functions instead.
Create the forwarding function with its normal name.
(This function will be the one to be called from another file).
```cpp
    public:
        static void BlinkOpen(const char* somePtr) //...
```

In the function body, use the `CHECK_ENABLED` macro to ensure the function should be called.
Verification of parameters should also be done in this enabling function, using the `CEP_ASSERT` macro for assertion (if result is `false`, break the program and print the message).
(The `CEP_ASSERT_NULL` macro also exists, specialized in null checking, with a default message.)
Finally, the real implementation function should be called through the static instance.
```cpp
    public:
        static void BlinkOpen(const char* somePtr)
        {
            CHECK_ENABLED();
            CEP_ASSERT_NULL(somePtr);
            CEP_ASSERT(*somePtr == 'H' || *somePtr == 'h', "The string must begin with an H");
            
            GetInstance().BlinkOpenImpl(somePtr);
        }
```

Create the implementation function declaration (privately).
```cpp
    private:
        void BlinkOpenImpl(const char* somePtr);
```

The actual implementation of the functions should be done in the `.cpp` file.
```cpp
    void TestModule::BlinkOpenImpl(const char* somePtr)
    {
        UNUSED(somePtr);	// The `UNUSED` macro avoids warnings
        
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
    }
```


------------------------------------
### Accessors:
Just like functions, data cannot be normally accessed through the class instance.
It must be read and written to via accessor functions.

First, create your data:
(You can assign default values, or initialize them via the constructor or the `Init()` function)
```cpp
    private:
        std::uint32_t m_firstDelay  = 1000;
        std::uint32_t m_secondDelay = 250;
```

Then, you create a `static` read and write (`get()` and `set()`) accessor functions for each member.
```cpp
  public:
    [[nodiscard]] static std::uint32_t GetFirstDelay() { return GetInstance().m_firstDelay; }
    static void SetFirstDelay(std::uint32_t value)     { GetInstance().m_firstDelay = value; }

    [[nodiscard]] static std::uint32_t GetSecondDelay() { return GetInstance().m_secondDelay; }
    static void SetSecondDelay(std::uint32_t value)     { GetInstance().m_secondDelay = value; }
```
(The `[[nodiscard]]` qualifier will simply give warnings if the return value is not kept. It is encouraged to use it)


------------------------------------
### Calling functions:
Calling a function of module `TestModule` from another module would go like this:
```cpp
    std::uint32_t firstDelay = TestModule::GetFirstDelay();
    TestModule::SetSecondDelay(1000);
    TestModule::BlinkOpen("Hello World!");
```

Default functions implemented in the `Module` class are also available in this way:
```cpp
    TestModule::IsEnabled()
```
or 
```cpp
    TestModule::Init()
```

**IMPORTANT: Do not call the `TaskHandler` directly, let the `Application` module handle it.**


------------------------------------
### Logging
The logging library used is [`spdlog`](https://github.com/gabime/spdlog), based around [`libfmt` ](https://github.com/fmtlib/fmt)
We've redirected `spdlog`'s output to the UART port.

A wrapper class, `Log`, is provided to access 5 different logging levels:
   * Debug
   * Info
   * Warning
   * Error
   * Critical

And helper macros are also provided to access these levels
(`LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR` and `LOG_CRITICAL`)
Is it advised that you use the macros directly instead of calling the static functions of the `Log` class, as these macros will fill in the required `__FILE__`, `__FUNCTION__` and `__LINE__` fields.

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
