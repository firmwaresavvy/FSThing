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
#include "FS_STM32F4xxUSART.h"

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
static void initFineTimer(void);
static void initDebugUART(void);

// Misc interrupt handlers.
void TIM2_IRQHandler(void);

/*------------------------------------------------------------------------------
----------------------- END PRIVATE FUNCTION PROTOTYPES  -----------------------
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
------------------------ START PRIVATE GLOBAL VARIABLES  -----------------------
------------------------------------------------------------------------------*/

// System <-> module binding data structure.
static FS_GenericModuleSystemBinding_t sys;

// Hardware interfaces.
static FS_STM32F4xxUSARTDriver_t usartPeripherals;
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
FS_STM32F4xxI2CDriver
static void initHardware(void)
{
  //Ensure all priority bits are assigned as preemption priority bits.
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  initFineTimer();

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
  FS_System_InitStruct_t initStruct;

  FS_System_InitStructInit(&initStruct);
  initStruct.sysInstance = &sys;
  initStruct.timerIntervalMicroseconds = FST_FINE_TIMER_INTERVAL_US;
  initStruct.usart = NULL; // TODO: FIX THIS!!
  FS_System_Init(&initStruct);
}

static void initModules(void)
{
  // Network stack.
  //lwipInit();

  //FST_Blinky_TaskInit();
}

/**
 * @brief Initialise timer TIM2 (32-bit general purpose) to
 *        provide interrupts every FST_FINE_TIMER_INTERVAL_US
 *        microseconds.
 *
 */
static void initFineTimer(void)
{
  NVIC_InitTypeDef nvicInitStruct;
  TIM_OCInitTypeDef timOCInitStruct;
  TIM_TimeBaseInitTypeDef timeBaseInitStruct;
  uint16_t prescalerValue;
  uint32_t compareValue;

  // Enable clock to the timer peripheral.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // Enable the timer interrupt with highest (lowest numerical) priority.
  nvicInitStruct.NVIC_IRQChannel = TIM2_IRQn;
  nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  nvicInitStruct.NVIC_IRQChannelSubPriority = 1;
  nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInitStruct);

  /*
  We want the timer to cause an interrupt every FST_FINE_TIMER_INTERVAL_US
  us. The counter clock frequency we choose is arbitrary. We just
  need to ensure that the value in the compare register is reached
  appropriately frequently. We'll arbitrarily use 50MHz as our
  counter clock frequency as this gives scope to tweak the global
  timer to be *really* fine later on.
  */
  prescalerValue = (uint16_t)((SystemCoreClock / 2) / 500000) - 1;

  // Time base configuration.
  TIM_TimeBaseStructInit(&timeBaseInitStruct);
  timeBaseInitStruct.TIM_Period = 0xFFFFFFFF;
  timeBaseInitStruct.TIM_Prescaler = 0;
  timeBaseInitStruct.TIM_ClockDivision = 0;
  timeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &timeBaseInitStruct);

  // Prescaler configuration.
  TIM_PrescalerConfig(TIM2, prescalerValue, TIM_PSCReloadMode_Immediate);

  // Output Compare Timing Mode configuration: Channel1:
  compareValue = 50000000 / ( 1 / ( FST_FINE_TIMER_INTERVAL_US * 1e-6 ) );
  timOCInitStruct.TIM_OCMode = TIM_OCMode_Timing;
  timOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  timOCInitStruct.TIM_Pulse = compareValue;
  timOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &timOCInitStruct);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  // Interrupt when compare value reached.
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

  // Enable counter.
  TIM_Cmd(TIM2, ENABLE);
}
static void initWatchdog(void)
{

}

static void initDebugUART(void)
{

}


// Misc interrupt handlers.
void TIM2_IRQHandler(void)
{
  sys.timeMicroseconds += FST_FINE_TIMER_INTERVAL_US;
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
