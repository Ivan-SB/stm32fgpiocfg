# gpiocfg

## Overview
This program read pin definitions from an STCubeMX project and write C code to set up quickly STM32F1xx GPIO.
The generated code setup and apply all the configuration in as fewer step as possible in spite of looping over every pin.
Configuration is defined gpio-cfg.h in a vector of struct that contain:
- a function name
- a port group (GPIOA, GPIOB...)
- if pull up|down setup should be grouped at the end of port setup or applied ASAP
- if all input ports should be configured before output ports
- a vector of GPIO_InitTypeDef to define port setup

##HOWTO
1. gpiocfg_install.py generate a self sufficient python program (gpiocfg.py) that contains all the necessary C code that has to be compiled
   gpiocfg_install.py > gpiocfg.py
2. gpiocfg.py accept as parameters the eclipse stm32f project directory and the path to gpiodef.h that should contain the pin configuration
3. change gpiodef.h to your needs
4. run gpiocfg.py -d [eclipse project directory path] -c [gpiodef.h path]
   This will compile and run gpiocfg that will generate GPIO.c and GPIO.h in the proper eclipse project directory

##Whath's wrong with this
I did want to avoid to rewrite pin configuration definition, I didn't want to invent a new syntax to define GPIO setup other than the GPIO_InitTypeDef defined in ST Cube library.
I didn't want to write a C parser to read mxconstants.h and stm32f1xx_hal_gpio.h and to parse a C struct.
So you'll have to compile this every time you want a new pin configuration.
