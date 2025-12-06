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
    // initialize RF69
    printf_dma("[RF69] Initializing ... ");
    int state = radio.begin(868.0, 300.0, 60.0, 250.0, 17, 32);
    if (state == RADIOLIB_ERR_NONE) {
        printf_dma("success!\n");
    } else {
        printf_dma("failed, code %d\n", state);
        while (true) { }
    }

    // Set sync word
    uint8_t syncWord[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    if (radio.setSyncWord(syncWord, 8) == RADIOLIB_ERR_INVALID_SYNC_WORD) {
        printf_dma("[RF69] Selected sync word is invalid for this module!\n");
        while (true) { }
    }
    printf_dma("[RF69] All settings changed successfully!\n");
}

void RadioLib_Loop(void) {
    printf_dma("[RF69] Waiting for incoming transmission ... ");

    // receive data as byte array
    uint8_t byteArr[8];
    int state = radio.receive(byteArr, 8, 1000);

    if (state == RADIOLIB_ERR_NONE) {
        // packet was successfully received
        printf_dma("success!\n[RF69] Data: %d %d %d %d RSSI: %f dBm\n",  
                    byteArr[0], byteArr[1], byteArr[2], byteArr[3], radio.getRSSI());
    } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
        // timeout occurred while waiting for a packet
        printf_dma("timeout!\n");
    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
        // packet was received, but is malformed
        printf_dma("CRC error!\n");
    } else {
        // some other error occurred
        printf_dma("failed, code %d\n", state);
    }
}


}
