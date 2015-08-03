/**
  ******************************************************************************
  *
  * @file     main.c
  * @author   Andy Norris [andy@firmwaresavvy.com]
  * @brief    Project main file. Initialises hardware and tasks and starts the
  *           scheduler.
  *
  ******************************************************************************
  */

/*------------------------------------------------------------------------------
-------------------------------- START INCLUDES --------------------------------
------------------------------------------------------------------------------*/

// Project includes.
#include "FSThing.h"

// FreeRTOS includes.
#include "FreeRTOS.h"
#include "task.h"

/*------------------------------------------------------------------------------
--------------------------------- END INCLUDES ---------------------------------
------------------------------------------------------------------------------*/


int main(void)
{
  // Set up hardware and tasks.
  FSThing_Init();

  // Start the scheduler.
  vTaskStartScheduler();

  // We should never get here!
  while(1);
}
