
#ifndef MPU925_H_
#define MPU925_H_

#include "main.h"
//---------------  программный I2C -----------------------------

#define SDA_set 		GPIOB->ODR |= GPIO_ODR_ODR15 
#define SDA_reset 	GPIOB->ODR &= ~GPIO_ODR_ODR15 
#define SCL_set 		GPIOB->ODR |= GPIO_ODR_ODR14
#define SCL_reset 	GPIOB->ODR &= ~GPIO_ODR_ODR14
#define SDA_status (GPIOB->IDR&0x8000) == 0x8000

#define SCL_Init	GPIOB->CRH &= ~(GPIO_CRH_MODE14 | GPIO_CRH_CNF14); GPIOB->CRH |= GPIO_CRH_MODE14_0 | GPIO_CRH_CNF14_0 	

#define SDA_input GPIOB->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15); GPIOB->CRH |= GPIO_CRH_CNF15_1 //Input with pull-up / pull-down
#define SDA_output GPIOB->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15); GPIOB->CRH |= GPIO_CRH_MODE15_0 | GPIO_CRH_CNF15_0	

#define SettingTimer	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; TIM4->SMCR &= ~TIM_SMCR_SMS; TIM4->PSC=0; TIM4->ARR=0xfff0
#define Start_Timer TIM4->CNT=0; TIM4->CR1 |= TIM_CR1_CEN
#define Stop_Timer TIM4->CR1 &= ~TIM_CR1_CEN

#define Peiod 20
#define Pausa(time) while (TIM4->CNT<time){}
	
//----------------------- MPU925 ---------------------------------

typedef enum GyroRange_ {
	GYRO_RANGE_250DPS = 0,
	GYRO_RANGE_500DPS,
	GYRO_RANGE_1000DPS,
	GYRO_RANGE_2000DPS
} GyroRange;

typedef enum AccelRange_ {
	ACCEL_RANGE_2G = 0,
	ACCEL_RANGE_4G,
	ACCEL_RANGE_8G,
	ACCEL_RANGE_16G
} AccelRange;

typedef enum DLPFBandwidth_ {
	DLPF_BANDWIDTH_184HZ = 0,
	DLPF_BANDWIDTH_92HZ,
	DLPF_BANDWIDTH_41HZ,
	DLPF_BANDWIDTH_20HZ,
	DLPF_BANDWIDTH_10HZ,
	DLPF_BANDWIDTH_5HZ
} DLPFBandwidth;

typedef enum SampleRateDivider_ {
	LP_ACCEL_ODR_0_24HZ = 0,
	LP_ACCEL_ODR_0_49HZ,
	LP_ACCEL_ODR_0_98HZ,
	LP_ACCEL_ODR_1_95HZ,
	LP_ACCEL_ODR_3_91HZ,
	LP_ACCEL_ODR_7_81HZ,
	LP_ACCEL_ODR_15_63HZ,
	LP_ACCEL_ODR_31_25HZ,
	LP_ACCEL_ODR_62_50HZ,
	LP_ACCEL_ODR_125HZ,
	LP_ACCEL_ODR_250HZ,
	LP_ACCEL_ODR_500HZ
} SampleRateDivider;

uint8_t MPU9250_Init();
/* read the data, each argiment should point to a array for x, y, and x */
void MPU9250_GetData(int16_t* AccData, int16_t* MagData, int16_t* GyroData);

/* sets the sample rate divider to values other than default */
void MPU9250_SetSampleRateDivider(SampleRateDivider srd);
/* sets the DLPF bandwidth to values other than default */
void MPU9250_SetDLPFBandwidth(DLPFBandwidth bandwidth);
/* sets the gyro full scale range to values other than default */
void MPU9250_SetGyroRange(GyroRange range);
/* sets the accelerometer full scale range to values other than default */
void MPU9250_SetAccelRange(AccelRange range);

void Init_I2C();
void MPU_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/*uint8_t I2C_Start(uint8_t direction);
uint8_t I2C_Stop();
uint8_t I2C_Byte_Write(uint8_t b);
uint8_t I2C_Byte_Read(uint8_t ACK);
*/

#endif /* MPU925_H_ */





