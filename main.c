#include "main.h"
#include "Rington.h"
#include "OLED_SSD1306.h"
//#include "I2C.h"
#include "MPU9250_progI2C.h"

#define resetLed GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR11 | GPIO_ODR_ODR12)

int16_t AccData[3];
int16_t MagData[3];
int16_t GyroData[3];
	

int main()
{
	SystemInit();
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN;	// Разрешить тактирование портов A B .
	
	GPIOB->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
	GPIOB->CRL |= GPIO_CRL_CNF7_1;
	GPIOB->ODR |= GPIO_ODR_ODR7;

	GPIOA->CRH&= ~(GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12);
	GPIOA->CRH|= GPIO_CRH_MODE10_1 | GPIO_CRH_MODE11_1 | GPIO_CRH_MODE12_1 ;// в режим выходов 10MHz
	
	StartPWM();
	
	ssd1306Init();
	Init_I2C();
	//MPU9250_Init();
	PlayRington1(Melody5, 10);
		
	uint8_t y=0;
	ClearDisplayBuff();
	
	resetLed; GPIOA->ODR |= GPIO_ODR_ODR10;
	uint8_t Regim=1;

	while (1)
  {
		if ((GPIOB->IDR&0x80) == 0) 
		{
			if (++Regim>3) Regim=1; 
			resetLed; 
			ClearDisplayBuff(); 
			DelayMiliSec(500);
			
			//if (Regim == 3) {Vx=0; Sx=0; GPIOA->ODR |= GPIO_ODR_ODR10;}
		}
		
		if (Regim==1) 
		{
			GPIOA->ODR |= GPIO_ODR_ODR11;
			MPU9250_GetData(GyroData, MagData, AccData);
			PrintInt((int32_t)GyroData[0], 4, 10, 5);
			PrintInt((int32_t)GyroData[1], 4, 20, 5);
			PrintInt((int32_t)GyroData[2], 4, 30, 5);
		
	/*	PrintInt((int32_t)MagData[0], 68, 10, 5);
			PrintInt((int32_t)MagData[1], 68, 20, 5);
			PrintInt((int32_t)MagData[2], 68, 30, 5); */
			
			PrintInt((int32_t)AccData[0], 68, 10, 5);
			PrintInt((int32_t)AccData[1], 68, 20, 5);
			PrintInt((int32_t)AccData[2], 68, 30, 5);
			DelayMiliSec(10);
		}
		if (Regim==2) 
		{
			resetLed; GPIOA->ODR |= GPIO_ODR_ODR12;
			int y=32;
			int x=64;
			MPU9250_GetData(GyroData, MagData, AccData);			
			y-=GyroData[0]/500;
			x+=GyroData[1]/250;
			MarkerPrint(x, y);
			DelayMiliSec(20);
		}
		
	}
}