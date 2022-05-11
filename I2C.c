#include "I2C.h"

#define I2C I2C2
void I2C_init()
{
	GPIOB->CRH |= GPIO_CRH_MODE11 | GPIO_CRH_MODE10; // 11: Output mode, max speed 50 MHz.
	GPIOB->CRH |= GPIO_CRH_CNF11 | GPIO_CRH_CNF10; //11: Alternate function output Open-drain
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
	I2C->CR2 |= 36; //скорость шины 100 кГц, 
	I2C->TRISE = 36+1;
	
	 MODIFY_REG(I2C->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 180); //10
	
	I2C->CR1 |= I2C_CR1_ACK;
	I2C->CR1 |= I2C_CR1_PE;
}	

#define HMC5883L_addr_Read 0x3D
#define HMC5883L_addr_Write 0x3C

void I2C_Write(uint8_t buf[], uint8_t count)
{
	I2C->CR1 &= ~I2C_CR1_POS;
	I2C->CR1 |= I2C_CR1_ACK;
	I2C->CR1 |= I2C_CR1_START;
	
	while (!READ_BIT(I2C->SR1, I2C_SR1_SB)){};
	(void) I2C->SR1;	
		
	I2C->DR=HMC5883L_addr_Write; // передача адреса
	while (!READ_BIT(I2C->SR1, I2C_SR1_ADDR)){};
  (void) I2C->SR1;
  (void) I2C->SR2;	
	
	for (uint8_t i=0; i<count; i++)		
	{	
	  I2C->DR = buf[i];
    while (!READ_BIT(I2C->SR1, I2C_SR1_TXE)){};	
	}	
	I2C->CR1 |= I2C_CR1_STOP;
}

void I2C_Read(uint8_t buf[], uint8_t count)
{
	I2C->CR1 &= ~I2C_CR1_POS;
	I2C->CR1 |= I2C_CR1_ACK;
	I2C->CR1 |= I2C_CR1_START;
  
  //Indicate the status of Start Bit (master mode)
  while (!READ_BIT(I2C->SR1, I2C_SR1_SB)){};
  //read state
  (void) I2C->SR1;
		
  //Transmit Address
  I2C->DR = HMC5883L_addr_Read;
  while (!READ_BIT(I2C->SR1, I2C_SR1_ADDR)){};
  (void) I2C->SR1;
  (void) I2C->SR2;
		
  for(uint8_t i=0; i<count; i++)
  {
    if(i<(count-1))
    {
      while (!READ_BIT(I2C->SR1, I2C_SR1_RXNE)){};
      buf[i] = READ_BIT(I2C->DR, I2C_DR_DR);
    }
    else
    {
      CLEAR_BIT(I2C->CR1, I2C_CR1_ACK);
      SET_BIT(I2C->CR1, I2C_CR1_STOP);
      while (!READ_BIT(I2C->SR1, I2C_SR1_RXNE)){};
      buf[i] = READ_BIT(I2C->DR, I2C_DR_DR);
    }
  }
}