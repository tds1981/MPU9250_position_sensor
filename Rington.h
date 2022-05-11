#ifndef RINGTON_H_
#define RINGTON_H_
#include "main.h"
#endif 

#define TIM TIM3
#define Timer_ARP	  TIM3->ARR 
#define Timer_CCR		TIM3->CCR1
#define RCC_Enabel RCC->APB1ENR |= RCC_APB1ENR_TIM3EN  // разрешение тактирования таймера 3

//#define DelayMicro 5000000

extern uint8_t Melody1[7];
extern uint8_t Melody2[22];
extern uint8_t Melody3[41];
extern uint8_t Melody4[60];
extern uint8_t Melody5[28];
extern uint8_t	mel1[68];
extern uint8_t  mel2[33];
extern uint8_t	mel3[92];

void PlayRington(uint8_t *Melody, uint16_t SizeArr); //
void PlayRington1(uint8_t *Melody, uint16_t SizeArr);
void StartPWM();
void DelayMiliSec(uint16_t MiliSec);