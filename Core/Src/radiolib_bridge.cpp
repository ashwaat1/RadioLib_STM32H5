#include "RadioLib.h"      
#include "stm32h5xx_hal.h" 
#include "STM32H5_RadioLibHal.h"

extern "C" {
#include "radiolib_bridge.h"  
}

// create a new instance of the HAL class
STM32H5_RadioLibHal* hal = new STM32H5_RadioLibHal(&hspi2);

// RF69 has the following connections:
// CS pin:    PA10
// DIO0 pin:  PB6
// RESET pin: PB7
RF69 radio = new Module(hal, PA_10, PB_6, PB_7);

extern "C" {

void RadioLib_Init(void) {

    // Module* rf69_mod = radio.getMod();

    // printf_dma("My SPI checks:\n");

    // int version_spi = rf69_mod->SPIgetRegValue(RADIOLIB_RF69_REG_VERSION);
    // printf_dma("Version: %d\n", version_spi);

    // int mode_spi = rf69_mod->SPIgetRegValue(RADIOLIB_RF69_REG_OP_MODE);
    // printf_dma("Mode: %d\n", mode_spi);; 

    // printf_dma("Setting automatic sequencer %d\n", rf69_mod->SPIsetRegValue(RADIOLIB_RF69_REG_OP_MODE, RADIOLIB_RF69_SEQUENCER_ON, 7, 7));
    // HAL_Delay(1);
    // printf_dma("Setting mode to STBY : %d", rf69_mod->SPIsetRegValue(RADIOLIB_RF69_REG_OP_MODE, RADIOLIB_RF69_STANDBY, 4, 2));
    // HAL_Delay(1);
    // printf_dma("Version: %d Mode: %d\n", rf69_mod->SPIgetRegValue(RADIOLIB_RF69_REG_VERSION), rf69_mod->SPIgetRegValue(RADIOLIB_RF69_REG_OP_MODE)); 

    // initialize RF69
    printf_dma("[RF69] Initializing ... ");
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        printf_dma("success!\n");
    } else {
        printf_dma("failed, code %d\n", state);
        while (true) { }
    }

    state = radio.setFrequency(915.0);
    if (state == RADIOLIB_ERR_NONE) {
        printf_dma("frequency success!\n");
    } else {
        printf_dma("failed, code %d\n", state);
        while (true) { }
    }

    state = radio.setBitRate(4.8);
    if (state == RADIOLIB_ERR_NONE) {
        printf_dma("bitrate success!\n");
    } else {
        printf_dma("failed, code %d\n", state);
        while (true) { }
    }

    state = radio.setFrequencyDeviation(5.0);
    if (state == RADIOLIB_ERR_NONE) {
        printf_dma("deviation success!\n");
    } else {
        printf_dma("failed, code %d\n", state);
        while (true) { }
    }

    state = radio.setOutputPower(15.0, true);
    if (state == RADIOLIB_ERR_NONE) {
        printf_dma("power success!\n");
    } else {
        printf_dma("failed, code %d\n", state);
        while (true) { }
    }
}

void RadioLib_Loop(void) {
    int16_t version = radio.getChipVersion();
    printf_dma("Chip version is %d\n", version);

    float freq_read = -1.0;
    int freq_status = radio.getFrequency(&freq_read);
    printf_dma("Frequency is %d status %d\n", (int)freq_read, freq_status);

    char str[] = "Hello World!";
    int state = radio.transmit(str);

    if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        printf_dma("success!\n");
    } else {
        // some other error occurred
        printf_dma("failed, code %d\n", state);
    }

    while(1){}

}


}
