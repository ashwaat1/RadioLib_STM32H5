#include <stdio.h>

// printf_dma definitions
#define UART_DMA_TX_WAIT() 	while(UART_TX_inUse==1);
#define printf_dma(...)     UART_DMA_TX_WAIT(); \
                            sprintf_len=sprintf((char*)TxBuffer, __VA_ARGS__ ); \
                            HAL_UART_Transmit_DMA(&huart2, TxBuffer, sprintf_len); \
                            UART_TX_inUse=1;

// Private variables for printf_dma
extern uint8_t TxBuffer[100];
extern volatile uint16_t Counter;
extern volatile uint8_t UART_TX_inUse;
extern volatile uint16_t sprintf_len;

// Handles
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;