#ifndef STM32H5_EXTI_H
#define STM32H5_EXTI_H

#include "STM32H5_PinMap.h"

// Array of IRQs attached to each pin
void (*irqCallbacks[MAX_RADIOLIB_PINS+1])(void) = { 0 };

// Helper to get correct IRQ instance
IRQn_Type get_exti_irqn_from_pinMask(uint16_t pin) {
    switch(pin) {
        case GPIO_PIN_5:  return EXTI5_IRQn;
        case GPIO_PIN_6:  return EXTI6_IRQn;
        case GPIO_PIN_7:  return EXTI7_IRQn;
        case GPIO_PIN_9:  return EXTI9_IRQn;
        case GPIO_PIN_10: return EXTI10_IRQn;
        case GPIO_PIN_12: return EXTI12_IRQn;
        case GPIO_PIN_13: return EXTI13_IRQn;
        default: return EXTI0_IRQn; // fallback, shouldn't happen
    }
}

// Handlers for each EXTI
void EXTI5_IRQHandler(void){
    if (irqCallbacks[PB_5]) {
        irqCallbacks[PB_5]();
    }
}
void EXTI6_IRQHandler(void){
    if (irqCallbacks[PB_6]) {
        irqCallbacks[PB_6]();
    }
}
void EXTI7_IRQHandler(void){
    if (irqCallbacks[PB_7]) {
        irqCallbacks[PB_7]();
    }
}
void EXTI9_IRQHandler(void){
    if (irqCallbacks[PA_9]) {
        irqCallbacks[PA_9]();
    }
}
void EXTI10_IRQHandler(void){
    if (irqCallbacks[PA_10]) {
        irqCallbacks[PA_10]();
    }
}
void EXTI12_IRQHandler(void){
    if (irqCallbacks[PB_12]) {
        irqCallbacks[PB_12]();
    }
}
void EXTI13_IRQHandler(void){
    if (irqCallbacks[PC_13]) {
        irqCallbacks[PC_13]();
    }
}

#endif