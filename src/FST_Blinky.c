
// C standard library includes.
#include <stdbool.h>
#include <string.h>

// Own header.
#include "FST_Blinky.h"

static void mainLoop(void * params);
static void fatalExceptionHandler(void);

// Interfaces to system services.
static FS_GenericModuleSystemBinding_t * sys;
static FS_DT_GPIO_ControlInterface_t * gpio;

// Module internal flags and labels.
static _Bool fatalExceptionDetected;
static int16_t moduleExceptionHandlerLabel;
static int16_t led1LineLabel;
static int16_t led2LineLabel;


void FST_Blinky_InitStructInit(FST_Blinky_InitStruct * initStruct)
{
  initStruct->sys = NULL;
  initStruct->gpio = NULL;
  initStruct->led1LineLabel = -1;
  initStruct->led2LineLabel = -1;
}

bool FST_Blinky_AssertInitStruct(FST_Blinky_InitStruct * initStruct)
{
  if(!initStruct->sys) return false;
  if(!initStruct->gpio) return false;
  if(led1LineLabel < 0) return false;
  if(led2LineLabel < 0) return false;
  return true;
}

void FST_Blinky_Init(FST_Blinky_InitStruct * initStruct)
{
  // Check the incoming parameters.
  #ifdef FSTHING_DEBUG
  if(!FST_Blinky_AssertInitStruct(initStruct))
  {
    while(true);
  }
  #endif // DEBUG

  // Populate the module-internal system binding struct.
  memcpy(&sys, initStruct->sys, sizeof(FS_GenericModuleSystemBinding_t));

  // Register this module with the exception handler.
  moduleExceptionHandlerLabel = sys->exc->registerModule("BLINKY", fatalExceptionHandler);

  // Detect an error in exception handler registration.
  if(-1 == moduleExceptionHandlerLabel)
  {
    sys->log->printf("FST_Blinky failed to register with sys.exc");
  }

  // Populate the GPIO interface.
  memcpy(&gpio, initStruct->gpio, sizeof(FS_DT_GPIO_ControlInterface_t));

  // Initialise all internal flags.
  fatalExceptionDetected = false;

  // Copy in the GPIO line labels.
  led1LineLabel = initStruct->led1LineLabel;
  led2LineLabel = initStruct->led2LineLabel;

  // Give the module's task to the scheduler.
  xTaskCreate( mainLoop,
              "BLINKY",
              FST_BLINKY_MAX_STACK_DEPTH_BYTES,
              NULL,
              FST_BLINKY_TASK_PRIORITY,
              initStruct->taskHandle );
}



static void mainLoop(void * params)
{
  while(1)
  {
    // Standard path through the code.
    if(!fatalExceptionDetected)
    {

    }

    // Do appropriate teardown in the case of a fatal exception.
    else
    {

    }
  }
}

// To catch a signal from the system exception handler.
static void fatalExceptionHandler(void)
{
  fatalExceptionDetected = true;
}
