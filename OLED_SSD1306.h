#include "main.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define BUFFER_SIZE 1024

#define MOSI_set 		GPIOA->BSRR|=GPIO_BSRR_BS7
#define MOSI_reset 	GPIOA->BSRR|=GPIO_BSRR_BR7
#define SCK_set 		GPIOA->BSRR|=GPIO_BSRR_BS5
#define SCK_reset 	GPIOA->BSRR|=GPIO_BSRR_BR5

#define CS_SET GPIOA->BSRR|=GPIO_BSRR_BS2
#define CS_RES GPIOA->BSRR|=GPIO_BSRR_BR2
#define RESET_SET GPIOA->BSRR|=GPIO_BSRR_BS1
#define RESET_RES GPIOA->BSRR|=GPIO_BSRR_BR1
#define DATA GPIOA->BSRR|=GPIO_BSRR_BS3
#define COMMAND GPIOA->BSRR|=GPIO_BSRR_BR3

#define Pix1 displayBuff[x+(y/8)*SSD1306_WIDTH]|=(1<<(y%8))
#define Pix0 displayBuff[x+(y/8)*SSD1306_WIDTH]&=~(1<<(y%8))

//--------------------------------

void spi1Init();
void ssd1306Init();
void ClearDisplayBuff();
void PrintBufer();
void Number(uint8_t Num, uint8_t x0, uint8_t y0);
void PrintInt(int32_t NUM, uint8_t x0, uint8_t y0, uint8_t MAX_SIZE);
void MarkerPrint(uint8_t x0, uint8_t y0);
//------------------------------------

