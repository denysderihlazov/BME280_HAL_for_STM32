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
![screenshot of I2C setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/i2c2_setup1.png?raw=true)


As illustrated in the image above, for our BME280 sensor, it is sufficient to simply enable I2C2 and use the default setup:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/i2c2_setup2.png?raw=true)

With I2C2 now configured, the next step is to create a libs folder and add it to the CubeIDE Paths. Right-click on your project name in the Project Explorer, select `New` -> `Folder`:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/new_libs_folder.png?raw=true)
And name your folder as desired (I named mine `libs`):
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/new_libs_folder2.png?raw=true)

When we have created our folder, we should add it to paths to make it vissible for CubeIDE and linker. You can siply do this by clicking right mouse button on your project name in Project Explorer and by choosing option `Properties`:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/adding_to_paths.png?raw=true)

Then `Paths and Symbols` -> `Library Paths` then click on `Add` button:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/adding_to_paths2.png?raw=true)

And check all 3 checkboxes, after that click on a `Workspace` button:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/adding_to_paths3.png?raw=true)

And in your current project find folder you have just created (in my case `libs`):
![screenshot of I2C def.setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/adding_to_paths4.png?raw=true)

And the same for `Properties` -> `Paths and Symbols` -> `Source Location` -> `Add Folder`:
![screenshot of I2C def.setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/Source_Location1.png?raw=true)

Then choose your folder (for me it is `libs` folder). And click on `OK` button:
![screenshot of I2C def.setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/Source_Location2.png?raw=true)

After configuring the project just press on `Apply and Close` button. Next place the `bme280.c` and `bme280.h` files into the `libs` folder. Once you have done this, open your `main.c` file and include the library header. Make sure to include it after all HAL-specific includes:
```sh
#include "bme280.h"
```
As on the screenshot below:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/include_bme280h.png?raw=true)

Once you have completed all the above steps, add the `BME280_Init();` function to your `main.c` file or wherever appropriate in your code. In my example (though not recommended for production use), I've added it to the `while(1)` loop just to demonstrate its functionality:
![screenshot of I2C def. setup](https://github.com/denysderihlazov/BME280_HAL_for_STM32/blob/main/readme_screenshots/init_bme280.png?raw=true)

Finally, to see your project in action, simply run it and upload it to the STM32 board. To view the results, you can use a console or an LCD screen if you have one connected.

I also have a project with the same BME280 sensor and a WaveShare ILI9341 SPI 240×320, General 2.4inch LCD. As soon as I add it to my GitHub, I will include a link here.

## Features
 - Establishes a connection with the Bosch BME280 sensor and checks for successful communication.
 - Configures the BME280 sensor by writing to its setup registers.
 - Reads raw data and displays parameters such as temperature, humidity, and pressure.
 - Applies calibration to the raw data, providing real-time, accurate, and calibrated measurements.

If you find this library helpful, please consider giving it a [star on my GitHub repository](https://github.com/denysderihlazov/BME280_HAL_for_STM32). 
Your support is greatly appreciated and helps me to continue improving and developing more useful tools and libraries. Thank you!😊