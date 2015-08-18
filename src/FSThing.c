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

// C standard library includes.
#include <stdbool.h>

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
  prescalerValue = (uint16_t)((SystemCoreClock / 2) / 50000000) - 1;

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
  // TODO: Work out why it was necessary to divide by 2 here.
  compareValue = ( 50000000 / ( 1 / ( FST_FINE_TIMER_INTERVAL_US * 1e-6 ) ) ) / 2;
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
  FS_STM32F4xxUSART_InitStruct_t initStruct;
  FS_STM32F4xxUSART_InitReturnsStruct_t returns;
  TaskHandle_t taskHandle;

  FS_STM32F4xxUSART_InitStructInit(&initStruct);
  FS_STM32F4xxUSART_InitReturnsStructInit(&returns);

  initStruct.instance = &usartPeripherals;

  /*
  We only want to initialise a single USART. FS_STM32F4xxUSART_InitStructInit()
  will have cleared the initialise flag for all other peripherals so we can
  just populate the init struct for the one USART we want to use.
  */
  initStruct.usart3InitStruct.initialise = true;
  initStruct.usart3InitStruct.peripheral = FST_DEBUG_UART_PERIPHERAL;

  // Tx pin.
  initStruct.usart3InitStruct.txd.port = FST_DEBUG_UART_TXD_PORT;
  initStruct.usart3InitStruct.txd.portRCCMask = FST_DEBUG_UART_TXD_RCC_MASK;
  initStruct.usart3InitStruct.txd.pinMask = FST_DEBUG_UART_TXD_PIN_MASK;
  initStruct.usart3InitStruct.txd.pinSource = FST_DEBUG_UART_TXD_PIN_SOURCE;

  // Rx pin.
  initStruct.usart3InitStruct.rxd.port = FST_DEBUG_UART_RXD_PORT;
  initStruct.usart3InitStruct.rxd.portRCCMask = FST_DEBUG_UART_RXD_RCC_MASK;
  initStruct.usart3InitStruct.rxd.pinMask = FST_DEBUG_UART_RXD_PIN_MASK;
  initStruct.usart3InitStruct.rxd.pinSource = FST_DEBUG_UART_RXD_PIN_SOURCE;

  // Buffers.
  initStruct.usart3InitStruct.txBufferSizeBytes = FST_DEBUG_UART_TX_BUFFER_SIZE_BYTES;
  initStruct.usart3InitStruct.rxBufferSizeBytes = FST_DEBUG_UART_RX_BUFFER_SIZE_BYTES;

  // Peripheral config using ST init struct.
  USART_StructInit( &( initStruct.usart3InitStruct.stInitStruct ) );
  initStruct.usart3InitStruct.stInitStruct.USART_BaudRate = FST_DEBUG_UART_BAUD_RATE;

  // Initialise the module.
  returns = FS_STM32F4xxUSART_Init(&initStruct);

  if(returns.success)
  {
    xTaskCreate( returns.mainLoop,
                "FS_STM32F4xxUSART",
                FS_STM32F4XXUSART_STACK_DEPTH,
                NULL,
                FS_STM32F4XXUSART_TASK_PRIORITY,
                &taskHandle );

    configASSERT(taskHandle);
  }
}


// Misc interrupt handlers.
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    sys.timeMicroseconds += FST_FINE_TIMER_INTERVAL_US;
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_Update);

    // Reset the timer's count.
    TIM2->CNT = 0;
  }
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
