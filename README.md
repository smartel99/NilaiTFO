# Nilai, The Fast One

NilaiTFO is an object-oriented modular application framework for embedded systems made in C++17.
It offers a way to entirely abstract the hardware, universalizing the usage of microcontroler peripherals as well as providing features that are standard with desktop programming, such as a fast and efficient logging library.

It is an on-going project, meaning that new features and new supported microcontrollers will continuously be added to it. 

If you would like a microcontroller to be supported, please submit an issue. Similarly, if you have added support to a new microcontroller, you can submit a push request as well.

# Features
- Support for commonly used peripherals
- Constantly growing list of supported chips and microcontrollers
- Customizable logging interface
- Modern and easy to use


# Currently Supported Microcontrollers
- STM32L4xx
- STM32F4xx
- STM32F7xx
- GD32F4xx
- Mocks

# Supported Peripherals
- ADC
- CAN
- I2C
- PWM
- RTC
- SPI
- UART

# Supported Chips
NilaiTFO also offers interfaces with commonly used chips, such as multiplexers/demultiplexers, ADCs, I/O extenders, etc.
- [ADS131 Analog-to-Digital Converter](https://www.ti.com/product/ADS131E08)
- [LTC2498 Analog-to-Digital Converter](https://www.analog.com/en/products/ltc2498.html)
- [DS2484 1-Wire Master](https://www.maximintegrated.com/en/products/interface/controllers-expanders/DS2484.html)
- ESP32 as a bluetooth module
- [MAX14763 SPDT +/-25V switch](https://www.maximintegrated.com/en/products/analog/analog-switches-multiplexers/MAX14763.html)
- [MAX14778 Dual 4:1 multiplexer](https://www.maximintegrated.com/en/products/analog/analog-switches-multiplexers/MAX14778.html#:~:text=The%20MAX14778%20dual%204%3A1,3.0%20to%205.5V%20supply.&text=The%20MAX14778%20features%20a%20low,entire%20common%2Dmode%20voltage%20range.)
- [PCA9505 40-bit I2C I/O extender](https://www.nxp.com/products/interfaces/ic-spi-serial-interface-devices/ic-general-purpose-i-o/40-bit-ic-bus-i-o-port-with-reset-oe-and-int:PCA9505_06)
- Generic relay interface
- Generic heartbeat module

# Cloning
Through HTTPS:
```
git clone https://github.com/smartel99/NilaiTFO
```

Through SSH:
```
git clone git@github.com:smartel99/NilaiTFO.git
```

# CMake Options
[TODO: Fill this up]

| Option                          | Description                                                                                                                                                                                               | Default | Values                                                    |
| :------------------------------ | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :------ | :-------------------------------------------------------- |
| `NILAI_APPLICATION_HEADER_PATH` | Path to the application header, i.e. where cep::Application is inherited                                                                                                                                  | N/A     | N/A                                                       |
| `NILAI_DRIVERS_PATH`            | Path to a file including the drivers to be used by Nilai                                                                                                                                                  | N/A     | N/A                                                       |
| `NILAI_MCU`                     | Selects the microcontroller to use                                                                                                                                                                        | `MOCK`  | `MOCK`, `STM32F4XX`, `STM32F7XX`, `STM32L4XX`, `GD32F4XX` |
| `NILAI_MOCK_RANDOM_DATA`        | If enabled and if NILAI_MCU is set to MOCK, the mocks will generate random data. If disabled, the mocks will always return fixed data                                                                     | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_WARN_MISSING_DRIVERS`    | Warn if a module is included but the corresponding drivers are missing                                                                                                                                    | `ON`    | `ON`, `OFF`                                               |
| `NILAI_LOGGER_USE_RTC`          | Choose the timebase used by the logger. If true, the real-time clock will be used. Otherwise, the system's timebase will be used. The real-time clock must be enabled to be able to be used by the logger | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_LOG_ENABLE_DEBUG`        | Enables debug-level logging messages                                                                                                                                                                      | `ON`    | `ON`, `OFF`                                               |
| `NILAI_LOG_ENABLE_INFO`         | Enables info-level logging messages                                                                                                                                                                       | `ON`    | `ON`, `OFF`                                               |
| `NILAI_LOG_ENABLE_WARNING`      | Enables warning-level logging messages                                                                                                                                                                    | `ON`    | `ON`, `OFF`                                               |
| `NILAI_LOG_ENABLE_ERROR`        | Enables error-level logging messages                                                                                                                                                                      | `ON`    | `ON`, `OFF`                                               |
| `NILAI_LOG_ENABLE_CRITICAL`     | Enables critical-level logging messages                                                                                                                                                                   | `ON`    | `ON`, `OFF`                                               |
| `NILAI_USE_ADC`                 | Enables the ADC driver                                                                                                                                                                                    | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_CAN`                 | Enables the CAN driver                                                                                                                                                                                    | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_I2C`                 | Enables the I2C driver                                                                                                                                                                                    | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_SPI`                 | Enables the SPI driver                                                                                                                                                                                    | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_UART`                | Enables the UART driver                                                                                                                                                                                   | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_PWM`                 | Enables the PWM driver                                                                                                                                                                                    | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_RTC`                 | Enables the RTC driver                                                                                                                                                                                    | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_ADS`                 | Enables the ADS131 interface                                                                                                                                                                              | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_DS2484`              | Enables the DS2484 interface                                                                                                                                                                              | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_ESP32`               | Enables the ESP32 interface                                                                                                                                                                               | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_HEARTBEAT`           | Enables the Heartbeat interface                                                                                                                                                                           | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_LTC2498`             | Enables the LTC2498 interface                                                                                                                                                                             | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_MAX14763`            | Enables the MAX14763 interface                                                                                                                                                                            | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_MAX14778`            | Enables the MAX14778 interface                                                                                                                                                                            | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_MAX323`              | Enables the MAX323 interface                                                                                                                                                                              | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_PCA9505`             | Enables the PCA9505 interface                                                                                                                                                                             | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_RELAY`               | Enables the Relay interface                                                                                                                                                                               | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_RN2903`              | Enables the RN2903 interface                                                                                                                                                                              | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_TLP3545`             | Enables the TLP3545 interface                                                                                                                                                                             | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_UMO`                 | Enables the Umo service                                                                                                                                                                                   | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_UMO_PROTOCOL`            | Selects the hardware protocol used by the Umo module                                                                                                                                                      | `UART`  | `UART`, `CAN`                                             |
| `NILAI_USE_SYSTEM`              | Enables the System service                                                                                                                                                                                | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_LOGGER`              | Enables the Logger service                                                                                                                                                                                | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_FILE_LOGGER`         | Enables the File Logger service                                                                                                                                                                           | `OFF`   | `ON`, `OFF`                                               |
| `NILAI_USE_FILESYSTEM`          | Enables the Filesystem service (requires FATFS)                                                                                                                                                           | `OFF`   | `ON`, `OFF`                                               |