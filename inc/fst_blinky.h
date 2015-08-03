/**
 *******************************************************************************
 *
 * @file  FST_Blinky.h
 *
 * @brief FSThing blinky example task header.
 *
 * @author Andy Norris [andy@firmwaresavvy.com]
 *
 *******************************************************************************
 */

//Preprocessor guard.
#ifndef FST_BLINKY_H
#define FST_BLINKY_H

#include "FS_System.h"
#include "FS_DT_Conf.h"

// FreeRTOS includes.
#include "FreeRTOS.h"
#include "task.h"

// GPIO pin definitions.
#define FST_BLINKY_LD3_BLOCK                  0 // Native pin.
#define FST_BLINKY_LD3_PORT                   FS_STM32F4XXGPIO_PORTA
#define FST_BLINKY_LD3_BIT                    0

#define FST_BLINKY_LD4_BLOCK                  0 // Native pin.
#define FST_BLINKY_LD4_PORT                   FS_STM32F4XXGPIO_PORTA
#define FST_BLINKY_LD4_BIT                    0

// FreeRTOS control.
#define FST_BLINKY_MAX_STACK_DEPTH_BYTES      100 // Arbitrary at the moment - review!
#define FST_BLINKY_TASK_PRIORITY              5 // Arbitrary at the moment - review!


typedef struct
{
  // A handle to allow the system supervisor to monitor the status of the task.
  TaskHandle_t * taskHandle;

  // System bindings/hardware abstraction.
  FS_GenericModuleSystemBinding_t * sys;
  FS_DT_GPIO_ControlInterface_t * gpio;

  // GPIO line labels for LED control.
  int16_t led1LineLabel;
  int16_t led2LineLabel;

}FST_Blinky_InitStruct;

void FST_Blinky_InitStructInit(FST_Blinky_InitStruct * initStruct);
bool FST_Blinky_AssertInitStruct(FST_Blinky_InitStruct * initStruct);
void FST_Blinky_Init(FST_Blinky_InitStruct * initStruct);

#endif // FST_BLINKY_H
