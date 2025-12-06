#ifndef STM32H5_MYHAL_H
#define STM32H5_MYHAL_H

#include "stm32h5xx_hal.h"
#include "STM32H5_PinMap.h"
#include "STM32H5_EXTI_Handlers.h"

void DWT_Init(void) {
  /* Enable TRC */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  /* Reset the cycle counter */
  DWT->CYCCNT = 0;
  /* Enable the cycle counter */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void my_pinMode(uint32_t pin, uint32_t mode){
    switch((int) pinMap_Port[pin])
    {
        case GPIOA_BASE_NS:
          __HAL_RCC_GPIOA_CLK_ENABLE();
          break;
        case GPIOB_BASE_NS:
          __HAL_RCC_GPIOB_CLK_ENABLE();
          break;
        case GPIOC_BASE_NS:
          __HAL_RCC_GPIOC_CLK_ENABLE();
          break;
        default:
          return;
    }
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = pinMap_Pin[pin];
    GPIO_InitStruct.Mode  = mode;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    if (mode == GPIO_OUT){
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    }
    HAL_GPIO_Init(pinMap_Port[pin], &GPIO_InitStruct);
}

void my_digitalWrite(uint32_t pin, uint32_t value){
    HAL_GPIO_WritePin(pinMap_Port[pin], pinMap_Pin[pin], (value == GPIO_HIGH_LEVEL) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint32_t my_digitalRead(uint32_t pin){
    GPIO_PinState st = HAL_GPIO_ReadPin(pinMap_Port[pin], pinMap_Pin[pin]);
    return (st == GPIO_PIN_SET) ? GPIO_HIGH_LEVEL : GPIO_LOW_LEVEL;    
}

void my_attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode){
    // Set callback function
    irqCallbacks[interruptNum] = interruptCb;
    // Configure pin to external interrupt mode
    switch((int) pinMap_Port[interruptNum])
    {
        case GPIOA_BASE_NS:
          __HAL_RCC_GPIOA_CLK_ENABLE();
          break;
        case GPIOB_BASE_NS:
          __HAL_RCC_GPIOB_CLK_ENABLE();
          break;
        case GPIOC_BASE_NS:
          __HAL_RCC_GPIOC_CLK_ENABLE();
          break;
        default:
          return;
    }    

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = pinMap_Pin[interruptNum];
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(pinMap_Port[interruptNum], &GPIO_InitStruct);

    // Enable EXTI IRQ in NVIC
    IRQn_Type irqn = get_exti_irqn_from_pinMask(pinMap_Pin[interruptNum]);
    HAL_NVIC_SetPriority(irqn, 5, 0);
    HAL_NVIC_EnableIRQ(irqn);
}

void my_detachInterrupt(uint32_t interruptNum){
    // Reset callback function
    irqCallbacks[interruptNum] = 0;

    // Configure pin as a GPIO input
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin  = pinMap_Pin[interruptNum];
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(pinMap_Port[interruptNum], &GPIO_InitStruct);

    // Need not disable the IRQ in NVIC. 
}

unsigned long my_micros() {
    // Convert DWT cycles to microseconds
    return DWT->CYCCNT / (SystemCoreClock / 1000000U);
}

long my_pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) {
    my_pinMode(pin, GPIO_IN);

    uint32_t start = my_micros();
    uint32_t now;

    // 1) Wait for any current pulse in this state to end
    //    (i.e. wait until pin != state)
    while (my_digitalRead(pin) == state) {
      now = my_micros();
      if ((now - start) > timeout) {
        return 0;  // timeout while waiting for previous pulse to finish
      }
    }

    // 2) Wait for the next pulse to start (pin becomes == state)
    while (my_digitalRead(pin) != state) {
      now = my_micros();
      if ((now - start) > timeout) {
        return 0;  // timeout waiting for pulse to start
      }
    }

    // 3) Pulse just started
    uint32_t pulseStart = now;

    // 4) Wait for pulse to end (pin becomes != state)
    while (my_digitalRead(pin) == state) {
      now = my_micros();
      if ((now - start) > timeout) {
        return 0;  // timeout waiting for pulse to end
      }
    }

    return (long)(now - pulseStart);
}

#endif