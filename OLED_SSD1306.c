#include "OLED_SSD1306.h"

//GLCD FontName : Tahoma8x8
//GLCD FontSize : 8 x 8

const unsigned short Tahoma[] = {
        0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E, 0x00, 0x00,  // Code for char 0
        0x84, 0x82, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x00,  // Code for char 1
        0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86, 0x00, 0x00,  // Code for char 2
        0x42, 0xC3, 0x89, 0x89, 0xFF, 0x76, 0x00, 0x00,  // Code for char 3
        0x30, 0x28, 0x24, 0xFE, 0xFF, 0x20, 0x00, 0x00,  // Code for char 4
        0x40, 0xCF, 0x8F, 0x89, 0xF9, 0x71, 0x00, 0x00,  // Code for char 5
        0x7E, 0xFF, 0x89, 0x89, 0xF9, 0x70, 0x00, 0x00,  // Code for char 6
        0x01, 0x01, 0xE1, 0xFD, 0x1F, 0x03, 0x00, 0x00,  // Code for char 7
        0x76, 0xFF, 0x89, 0x89, 0xFF, 0x76, 0x00, 0x00,  // Code for char 8
        0x0E, 0x9F, 0x91, 0x91, 0xFF, 0x7E, 0x00, 0x00,  // Code for char 9
        0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
        0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
        0x00, 0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0x82   // Code for char <
        };

void spi1Init()
{
	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;//Включить тактирование SPI1 и GPIOA
	//RCC->AHBENR|=RCC_AHBENR_DMA1EN;//Включить тактирование DMA
	GPIOA->CRL|= GPIO_CRL_MODE5 | GPIO_CRL_MODE7;//PA4,PA5,PA7 в режим выходов 50MHz
	GPIOA->CRL&= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
	GPIOA->CRL|=  GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;//PA5,PA7 - выход с альтернативной функцией push-pull, PA4 - выход push-pull
  
	SPI1->CR1|=SPI_CR1_MSTR;//Режим ведущего
	SPI1->CR1|= (0x00 & SPI_CR1_BR);//Делитель частоты на 2
	SPI1->CR1|=SPI_CR1_SSM;//Программный NSS
	SPI1->CR1|=SPI_CR1_SSI;//NSS - high
//SPI1->CR2|=SPI_CR2_TXDMAEN;//Разрешить запросы DMA
	SPI1->CR1|=SPI_CR1_SPE;//включить SPI1
	
	return;
}

void spiTransmit(uint8_t data)
{
	//CS_RES;	
	/*SPI1->DR = data;
	while((SPI1->SR & SPI_SR_BSY))
	{};*/
	//CS_SET;
	
		for(uint8_t i=1; i<=8; i++)
		{
			if ((data&0x80)==0x80) MOSI_set; //  1
				else MOSI_reset; //  0
			SCK_set;
			data=data<<1;
			SCK_reset;
			for (uint8_t k=0; k<2; k++) {}	
		}
		return ;
}

void ssd1306SendCommand(uint8_t command)
{
	COMMAND;
	spiTransmit(command);
	DATA;
}

static uint8_t displayBuff[BUFFER_SIZE];//Буфер экрана

void ssd1306Init()
{
	GPIOA->CRL|= GPIO_CRL_MODE5 |GPIO_CRL_MODE7; 
	GPIOA->CRL&= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7); //PA7,PA5 в режим выхода
	
	uint16_t i;
	GPIOA->CRL|= GPIO_CRL_MODE2 |GPIO_CRL_MODE1 | GPIO_CRL_MODE3;
	GPIOA->CRL&= ~(GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3);//PA1,PA2,PA3 в режим выхода
	//Сброс экрана и очистка буфера
	CS_RES;
	RESET_RES;
	for(i=0;i<BUFFER_SIZE;i++)
	{
		spiTransmit(0);
	}
	RESET_SET;
	//CS_SET;//Выбор устройства на шине
	
	ssd1306SendCommand(0xAE); //display off
	ssd1306SendCommand(0xD5); //Set Memory Addressing Mode
	ssd1306SendCommand(0x80); //00,Horizontal Addressing Mode;01,Vertical
	ssd1306SendCommand(0xA8); //Set Page Start Address for Page Addressing
	ssd1306SendCommand(0x3F); //Set COM Output Scan Direction
	ssd1306SendCommand(0xD3); //set low column address
	ssd1306SendCommand(0x00); //set high column address
	ssd1306SendCommand(0x40); //set start line address
	ssd1306SendCommand(0x8D); //set contrast control register
	ssd1306SendCommand(0x14);
	ssd1306SendCommand(0x20); //set segment re-map 0 to 127
	ssd1306SendCommand(0x00); //set normal display
	ssd1306SendCommand(0xA1); //set multiplex ratio(1 to 64)
	ssd1306SendCommand(0xC8); //
	ssd1306SendCommand(0xDA); //0xa4,Output follows RAM
	ssd1306SendCommand(0x12); //set display offset
	ssd1306SendCommand(0x81); //not offset
	ssd1306SendCommand(0x8F); //set display clock divide ratio/oscillator frequency
	ssd1306SendCommand(0xD9); //set divide ratio
	ssd1306SendCommand(0xF1); //set pre-charge period
	ssd1306SendCommand(0xDB); 
	ssd1306SendCommand(0x40); //set com pins hardware configuration
	ssd1306SendCommand(0xA4);
	ssd1306SendCommand(0xA6); //set vcomh
	ssd1306SendCommand(0xAF); //0x20,0.77xVcc
	
	for(uint8_t x=0; x<128; x++)
		for(uint8_t y=0; y<64; y++)
	{
		if (x%5==0) Pix1;
		if (y%5==0) Pix1;
	}
	
	for(uint16_t i=0; i<BUFFER_SIZE; i++) spiTransmit(displayBuff[i]);
}

void ClearDisplayBuff()
{
	for(uint16_t i=0; i<BUFFER_SIZE; i++) displayBuff[i]=0;
}	

void Number(uint8_t Num, uint8_t x0, uint8_t y0)
{
	uint8_t x=x0, y=y0;
	if (Num==0xff) {y+=5; for (uint8_t i=0; i<4; i++){Pix1; x++;} y=y0; return;}
	if (Num==0xfe) {y+=5; for (uint8_t i=0; i<4; i++){Pix0; x++;} y=y0; return;}	
	
	for(uint8_t k=Num*8; k<8+Num*8; k++)
	{
		for(uint8_t j=0; j<8; j++) 
		{
			if ((Tahoma[k]>>j)&0x01) Pix1;
			else Pix0;
			y++;		
		}	
		y=y0; x++;
	}				
}

void PrintBufer()
{	
	for(uint16_t i=0; i<BUFFER_SIZE; i++) spiTransmit(displayBuff[i]);
}

void Ramka()
{		
	for(uint8_t x=0; x<128; x++)
		for(uint8_t y=0; y<64; y++)
	{
		if ((x==0)||(x==127)) Pix1;
		if ((y==0)||(y==63)) Pix1;
		if (x==64) Pix1;
	}
}

#define DIG_BASE  10 
//#define MAX_SIZE 5
//unsigned char SYMBOLS[DIG_BASE] = {'0','1','2','3','4','5','6','7','8','9'};
void PrintInt(int32_t NUM, uint8_t x0, uint8_t y0, uint8_t MAX_SIZE)
{
	int i, m;
	
	if (NUM<0) Number(0xff, x0, y0); // print minus
	else Number(0xfe, x0, y0); 
	
	uint8_t x=x0+10*(MAX_SIZE-1)+5;
	for(i=MAX_SIZE-1; i>=0; --i)
	{	
		m = NUM % DIG_BASE; 
		NUM /= DIG_BASE;  
		//if (i==0) {LCD_Data(','); LCD_Command(p--); }
		if (m<0) m*=-1;
		Number(m, x, y0); 
		x-=10;
	}
	if (MAX_SIZE<6) Ramka();
	PrintBufer();
}

void MarkerPrint(uint8_t x0, uint8_t y0)
{
	ClearDisplayBuff();
	for(uint8_t x=0; x<128; x++)
		for(uint8_t y=0; y<64; y++)
	{
		int dx = x-x0;
		int dy = y-y0;
		if ((dx*dx)+(dy*dy) <= 225) Pix1;
		else Pix0;
	}
	PrintBufer();
}	

