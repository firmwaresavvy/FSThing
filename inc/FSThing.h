// Preprocessor guard.
#ifndef FSTHING_H
#define FSTHING_H

#include "stm32f4xx_conf.h"

#define FST_FINE_TIMER_INTERVAL_US            50

// Debug UART.
#define FST_DEBUG_UART_PERIPHERAL             USART2

#define FST_DEBUG_UART_TXD_PORT               GPIOA
#define FST_DEBUG_UART_TXD_RCC_MASK           RCC_AHB1Periph_GPIOA
#define FST_DEBUG_UART_TXD_PIN_MASK           GPIO_Pin_2
#define FST_DEBUG_UART_TXD_PIN_SOURCE         GPIO_PinSource2

#define FST_DEBUG_UART_RXD_PORT               GPIOA
#define FST_DEBUG_UART_RXD_RCC_MASK           RCC_AHB1Periph_GPIOA
#define FST_DEBUG_UART_RXD_PIN_MASK           GPIO_Pin_3
#define FST_DEBUG_UART_RXD_PIN_SOURCE         GPIO_PinSource3

#define FST_DEBUG_UART_NVIC_IRQ_CHANNEL       USART2_IRQn

#define FST_DEBUG_UART_RX_BUFFER_SIZE_BYTES   1024
#define FST_DEBUG_UART_TX_BUFFER_SIZE_BYTES   1024
#define FST_DEBUG_UART_BAUD_RATE              57600




void FSThing_Init(void);

#endif // FSTHING_H
