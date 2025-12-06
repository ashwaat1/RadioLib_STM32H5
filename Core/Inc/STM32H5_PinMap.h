#ifndef STM32H5_RADIOLIB_PINMAP_H
#define STM32H5_RADIOLIB_PINMAP_H

#include "stm32h5xx_hal.h"

// Index of final pin number used
#define MAX_RADIOLIB_PINS 6

// GPIO configuration for Radiolib
#define GPIO_IN             GPIO_MODE_INPUT
#define GPIO_OUT            GPIO_MODE_OUTPUT_PP
#define GPIO_LOW_LEVEL      0
#define GPIO_HIGH_LEVEL     1
#define GPIO_IRQ_EDGE_RISE  GPIO_MODE_IT_RISING
#define GPIO_IRQ_EDGE_FALL  GPIO_MODE_IT_FALLING

// Pin indices
#define PA_10 0
#define PB_5  1
#define PB_6  2
#define PB_7  3
#define PB_12 4
#define PC_13 5
#define PA_9  MAX_RADIOLIB_PINS

GPIO_TypeDef* pinMap_Port[] = {GPIOA, GPIOB, GPIOB, GPIOB, GPIOB, GPIOC, GPIOA};
uint16_t      pinMap_Pin[]  = {GPIO_PIN_10, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_9};

#endif