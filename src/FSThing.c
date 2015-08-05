/**
  ******************************************************************************
  *
  * @file     FSThing.c
  * @author   Andy Norris [andy@firmwaresavvy.com]
  * @brief    Project glue file. Contains all project admin, deals with
  *           individual module init and coordinates how modules interact with
  *           each other.
  *
  ******************************************************************************
  */

/*------------------------------------------------------------------------------
-------------------------------- START INCLUDES --------------------------------
------------------------------------------------------------------------------*/

// Own header.
#include "FSThing.h"

// FS library includes.
#include "FS_System.h"
#include "FS_DT_Conf.h"

// ST includes.
#include "system_stm32f4xx.h" // Basic startup config etc.
#include "stm32f4xx_conf.h" // Global config file fot Standard Periph Lib.

/*------------------------------------------------------------------------------
--------------------------------- END INCLUDES ---------------------------------
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
---------------------- START PRIVATE FUNCTION PROTOTYPES  ----------------------
------------------------------------------------------------------------------*/

// Top-level init functions.
static void initHardware(void);
static void initSystem(void);
static void initModules(void);

// Hardware init functions.
static void initTimers(void);
static void initWatchdog(void);
static void initDebugUART(void);

/*------------------------------------------------------------------------------
----------------------- END PRIVATE FUNCTION PROTOTYPES  -----------------------
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
------------------------ START PRIVATE GLOBAL VARIABLES  -----------------------
------------------------------------------------------------------------------*/

// System <-> module binding data structure.
static FS_GenericModuleSystemBinding_t sys;

// Hardware interfaces.
// static FS_STM32F4xxUSARTDriver_t usartPeripherals;
// static FS_STM32F4xxI2CDriver_t i2cPeripherals;
// static FS_STM32F4xxSPIDriver_t spiPeripherals;
static FS_DT_GPIO_ControlInterface_t gpio;

// Software services.
// static FS_

/*------------------------------------------------------------------------------
------------------------- END PRIVATE GLOBAL VARIABLES  ------------------------
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
---------------------------- START PUBLIC FUNCTIONS ----------------------------
------------------------------------------------------------------------------*/

void FSThing_Init(void)
{
  initHardware();
  initSystem();
  initModules();

}

/*------------------------------------------------------------------------------
----------------------------- END PUBLIC FUNCTIONS -----------------------------
------------------------------------------------------------------------------*/

static void initHardware(void)
{
  //Ensure all priority bits are assigned as preemption priority bits.
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  initTimers();
  initWatchdog();

  // Comms peripherals.
  initDebugUART();
  //initI2C();
  //initSPI();

  // Init network hardware.
  //initEthernetMAC();
  //initEthernetPHY();

  // Connected devices.
  //initSdCard();
  //initWifi();

}

static void initSystem(void)
{
  // Module deals with system time, exceptions, file system etc.
  //FS_System_Init();
}

static void initModules(void)
{
  // Network stack.
  //lwipInit();

  //FST_Blinky_TaskInit();
}

static void initTimers(void)
{

}

static void initWatchdog(void)
{

}

static void initDebugUART(void)
{

}

// FreeRTOS application-specific hook functions.
void vApplicationMallocFailedHook(void)
{

}

void vApplicationTickHook(void)
{

}

void vApplicationStackOverflowHook(void)
{

}

void vApplicationIdleHook(void)
{

}
