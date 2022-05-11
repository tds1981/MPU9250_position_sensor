#include "Rington.h"

uint16_t Note1[8]={0, 1911, 1702, 1516, 1431, 1275, 1136, 1012};

uint8_t Melody1[7]={1,2,3,4,5,6,7};
uint8_t Melody2[22]={5,3,3,5,3,3,5,4,3,2,1,6,7,6,5,3,3,5,4,3,2,1};
	
uint8_t Melody3[41]	= {16,16,21,21,21,23,24,24,26,24,23,24,16,16,21,21,21,23,24,24,26,28,26,
												24,28,28,26,24,23,21,20,7,23,16,17,26,16,24,14,20,21};	
	
uint8_t Melody4[60]	= {19,14,19,14,19,18,18,18,14,18,
											 14,18,19,19,19,14,19,14,19,18,
											 18,18,14,18,14,18,19,19,21,21,
											 21,21,21,22,22,22,22,22,22,21,
											 19,18,19,19,19,21,21,21,21,21,
											 22,22,22,22,22,22,21,19,18,19}	;

uint8_t Melody5[28]= {10,19,19,17,19,15,10,10,10,19,
										19,20,17,22,22,12,12,20,20,19,
										17,15,15,19,19,17,19,15};
		
uint16_t Tabz[7] = {128, 256, 512, 1024, 2048, 4096, 8192} ; // zaderjki

uint16_t Note[37]={0,
			 7644, 7215, 6810, 6427, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4050, // 1 oktava
			 3822, 3607, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, // 2 oktava
			 1911, 1804, 1703, 1607, 1517, 1432, 1351, 1275, 1204, 1136, 1073, 1012}; // 3 oktava
	 

void StartPWM()
{
	RCC->APB1ENR |= RCC_APB2ENR_AFIOEN;
	RCC_Enabel; 
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
	GPIOA->CRL |= GPIO_CRL_CNF6_1;
	GPIOA->CRL |= GPIO_CRL_MODE6_1; //| GPIO_CRL_MODE6_0;
	
	TIM->SMCR &= ~TIM_SMCR_SMS;  // внутреннее тактирование, шина APB
	TIM->PSC=35;
	TIM->ARR = 7644;
	TIM->CCR1 = 3822;
	
	TIM->CCER|= TIM_CCER_CC1E;	//<! Ch1 out enable
	TIM->BDTR |= TIM_BDTR_MOE; //разрешим использовать выводы таймера как выходы
	
	TIM->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; //PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
	TIM->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM->CR1 |= TIM_CR1_ARPE;
	TIM->CR1 	|= 	TIM_CR1_CEN;	
	
	TIM->CR1 	&= 	~TIM_CR1_CEN;
}

void DelayMiliSec(uint16_t MiliSec)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->SMCR &= ~TIM_SMCR_SMS; 
	TIM4->PSC=72;
	TIM4->ARR=1000;
	TIM4->CR1 |= 	TIM_CR1_CEN;
	uint16_t Count=0;
	while (Count<MiliSec)
	{
		if (TIM4->CNT==1000) {Count++; TIM4->CNT=0;}
	}
	TIM4->CR1 &= ~TIM_CR1_CEN;
}	

void PlayRington(uint8_t Melody[], uint16_t SizeArr) //uint8_t Melody[]
{
	//uint16_t S = sizeof(Melody);
	uint32_t N; 
	for (uint16_t i=0; i<SizeArr; i++)
	{
		N = 0x0000001F; 
		N&= Melody[i];
		Timer_ARP = Note[N];
		Timer_CCR = Timer_ARP/2;
		DelayMiliSec(100);
	}
	Timer_CCR = 0;
}


void PlayRington1(uint8_t *Melody, uint16_t SizeArr) //uint8_t Melody[]
{
	TIM->CR1 	|= 	TIM_CR1_CEN;	
	for (uint16_t i=0; i<SizeArr; i++)
	{
		Timer_ARP = Note[Melody[i]];
		Timer_CCR = Timer_ARP/2;
		DelayMiliSec(300);
	}
	Timer_CCR = 0;
	TIM->CR1 	&= 	~TIM_CR1_CEN;
}

