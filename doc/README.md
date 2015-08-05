# FSThing README

This is the main README file for the FSThing project. It includes information about the project tree structure, dependencies and toolchain setup.


## Tree Structure

The project is version controlled using GIT and the master repo can be found at the following URL: https://github.com/firmwaresavvy/FSThing.git. 

The project depends upon several external libraries:

Third Party Libraries
* ARM CMSIS Abstraction Layer (common C abstraction layer for all Cortex-M4F cores)
* FatFS (our chosen FAT filesystem implementation)
* FreeRTOS (our chosen RTOS)
* STMicroelectronics Standard Peripheral Driver Library (C library for interaction with STM32F4xx peripherals)
* lwIP (our chosen TCP/IP implementation)

FirmwareSavvy Libraries
* FS_Datatypes(commonly used type definitions and helper functions including interface types)
* FS_DeviceDrivers (device driver modules for all external ICs used by the project)
* FS_System (a layer for interfacing software modules to basic system services, e.g. debug UARTs, logging etc)
* FS_STM32F4xx_HAL (hardware abstraction layer for STM32F4xx devices)

The libraries are included in the project tree using the GIT submodules functionality as follows:

    FSThing --+
              |
              +--lib--+
                      |
                      +--ThirdParty--+
                      |              |
                      |              +--ARM--+
                      |              |       |
                      |              |       +--CMSIS
                      |              |
                      |              +--FatFS
                      |              |  
                      |              +--FreeRTOS
                      |              |
                      |              +--lwIP
                      |              |
                      |              +--STMicroelectronics--+
                      |                                     |
                      |                                     +--StdPeriphLib
                      |
                      +--FirmwareSavvy--+
                                        |
                                        +--FS_Datatypes
                                        |
                                        +--FS_DeviceDrivers
                                        |
                                        +--FS_STM32F4xx_HAL
                                        |
                                        +--FS_System
