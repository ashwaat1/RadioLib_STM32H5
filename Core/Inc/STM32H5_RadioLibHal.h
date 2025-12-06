#ifndef STM32H5_RADIOLIBHAL_H
#define STM32H5_RADIOLIBHAL_H

#include <RadioLib.h>
#include "stm32h5xx_hal.h"
#include "STM32H5_MyHal.h"

class STM32H5_RadioLibHal : public RadioLibHal {
public:
  STM32H5_RadioLibHal(SPI_HandleTypeDef* SPIHandle)
    : RadioLibHal(GPIO_IN, GPIO_OUT, GPIO_LOW_LEVEL, GPIO_HIGH_LEVEL, GPIO_IRQ_EDGE_RISE, GPIO_IRQ_EDGE_FALL),
    _SPIHandle(SPIHandle){}

  void init() override {
    DWT_Init();
  }

  void term() override {}

  // GPIO-related methods (pinMode, digitalWrite etc.) should check
  // RADIOLIB_NC as an alias for non-connected pins
  void pinMode(uint32_t pin, uint32_t mode) override {
    if (pin == RADIOLIB_NC) {
        return;
    }    
    my_pinMode(pin, mode);
  }

  void digitalWrite(uint32_t pin, uint32_t value) override {
    if (pin == RADIOLIB_NC) {
        return;
    }    
    my_digitalWrite(pin, value);
  }

  uint32_t digitalRead(uint32_t pin) override {
    if (pin == RADIOLIB_NC) {
        return 0;
    }
    return my_digitalRead(pin);
  }

  void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) override {
    if (interruptNum == RADIOLIB_NC) {
      return;
    }

    my_attachInterrupt(interruptNum, interruptCb, mode);
  }

  void detachInterrupt(uint32_t interruptNum) override {
    if (interruptNum == RADIOLIB_NC) {
      return;
    }

    my_detachInterrupt(interruptNum);
  }

  void delay(unsigned long ms) override {
    HAL_Delay(ms);
  }

  void delayMicroseconds(unsigned long us) override {
    // Busy-wait using DWT cycle counter
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = (SystemCoreClock / 1000000U) * us;

    while ((DWT->CYCCNT - start) < ticks) {
      __NOP();
    }
  }

  unsigned long millis() override {
    // milliseconds since HAL_Init()
    return HAL_GetTick();
  }

  unsigned long micros() override {
    // Convert DWT cycles to microseconds
    return DWT->CYCCNT / (SystemCoreClock / 1000000U);
  }

  long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override {
    if (pin == RADIOLIB_NC) {
      return 0;
    }
    return my_pulseIn(pin, state, timeout);
  }

  void spiTransfer(uint8_t *out, size_t len, uint8_t *in) override {
    if(len == 0) {
        return;
    }
    HAL_SPI_TransmitReceive(_SPIHandle, out, in, len, HAL_MAX_DELAY);
  }

  // Not used as we are only concerned with RFM69 for now
  void tone(uint32_t pin, unsigned int frequency, unsigned long duration = 0) override {}
  void noTone(uint32_t pin) override {}

  // Assumed - CubeMX generates spiBegin 
  // and MX_SPI_INIT is called before Radiolib init.
  void spiBegin() override {}

  // Won't be necessary as SPI settings are fixed.
  void spiBeginTransaction() override {}
  void spiEndTransaction() override {}

  // No yield for single core baremetal apps
  void yield() override {}  

  // No need to de initialize SPI on STM32
  void spiEnd() override {}

private:
  // the HAL can contain any additional private members
  SPI_HandleTypeDef* _SPIHandle;
};


#endif
