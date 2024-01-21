# HAL library for Bosch BME280 sensor

This library facilitates the connection of the Bosch BME280 sensor to STM32 microcontrollers. It is specifically tailored for the STM32F411RE Nucleo board (you can use your own STM microcontroller), interfacing with the Gravity DFRobot BME280 sensor, but it's compatible with any BME280 sensor from Bosch. The library incorporates STM32 HAL-based calibration formulas for accurate data readings. 

If you find this library helpful, please consider giving it a [star on my GitHub repository](https://github.com/denysderihlazov/BME280_HAL_for_STM32). 
Your support is greatly appreciated and helps me to continue improving and developing more useful tools and libraries. Thank you!😊

## Installation

Clone the repository to your local machine:
```sh
git clone git@github.com:denysderihlazov/BME280_HAL_for_STM32.git
```
Navigate to the project directory:
```sh
cd <path_on_your_computer>/BME280_HAL_for_STM32
```

## Usage

Begin by creating an STM32 HAL project, for which STM32CubeIDE is highly recommended. Open the .ioc file in your CubeIDE project and configure the I2C settings according to your needs. In my setup, I use I2C2 with PB9 as I2C2_SDA and PB10 as I2C2_SCL. For clarity, a screenshot of this configuration is provided below:


As you can see in the image above, for our BME280, it is sufficient to simply enable I2C2 and use the default setup:


After that open your `main.c` script and include the library header in your main program file after all HAL includes:
```sh
#include "bme280.h"
```
## Features
 - Establishes a connection with the Bosch BME280 sensor and checks for successful communication.
 - Configures the BME280 sensor by writing to its setup registers.
 - Reads raw data and displays parameters such as temperature, humidity, and pressure.
 - Applies calibration to the raw data, providing real-time, accurate, and calibrated measurements.