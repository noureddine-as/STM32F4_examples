/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : L3GD20 library.
	* Author             : Noureddine-as
	* Email              : noureddine.asni@gmail.com
	* GitHub repository  : https://github.com/noureddine-as
	* Designed using     : STM32CubeMX + ARM Keil v5
  ******************************************************************************
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "L3GD20.h"

SPI_HandleTypeDef hspix;

/**
  * @brief  Initializes the SPI communication link that will be used for the gyroscope.
  * @param  SPIx : SPI link used.
  * @param  BaudRatePrescaler: the Baud Rate Prescaler choosed for APB2.
  */
void MX_SPIx_Init(SPI_TypeDef *SPIx, uint32_t BaudRatePrescaler)
{
  hspix.Instance = SPIx;
  hspix.Init.Mode = SPI_MODE_MASTER;
  hspix.Init.Direction = SPI_DIRECTION_2LINES;
  hspix.Init.DataSize = SPI_DATASIZE_8BIT;
  hspix.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspix.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspix.Init.NSS = SPI_NSS_SOFT;
  hspix.Init.BaudRatePrescaler = BaudRatePrescaler;   // default = SPI_BAUDRATEPRESCALER_64
  hspix.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspix.Init.TIMode = SPI_TIMODE_DISABLED;
  hspix.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspix.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspix);

}

/**
  * @brief  Initializes the L3GD20 link. the L3GD20 structure.
  * @param  myL3GD20: pointer to a gyroscope.
  * @param  SPIx : SPI link used.
  * @param  GPIOx : GPIO port where CS belongs.
  * @param  CS_Pin : Chip select pin.
  * @param  BaudRatePrescaler: the Baud Rate Prescaler choosed for APB2.
  * @param  reg_addr: register to write in.
  * @param  timeout: ammount of time to wait before response.
  * @retval Data contained in the register
  */

void L3GD20_Init(SPI_L3GD20_t *myL3GD20, SPI_TypeDef * SPIx, GPIO_TypeDef  *GPIOx, uint32_t CS_Pin,	uint32_t BaudRatePrescaler)
{	
	myL3GD20->SPIx = SPIx;
	myL3GD20->GPIOx = GPIOx;
	myL3GD20->CS_Pin = CS_Pin;
	myL3GD20->BaudRatePrescaler = BaudRatePrescaler;
	
	MX_SPIx_Init(SPIx, BaudRatePrescaler);
	
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Write an 8-bit value to a register in the L3GD20
  * @param  myL3GD20: pointer to a gyroscope.
  * @param  reg_addr: register to write in.
  * @param  timeout: ammount of time to wait before response.
  * @retval Data contained in the register
  */
void WriteOneByte(SPI_L3GD20_t *myL3GD20, uint8_t reg_addr, uint8_t data, uint32_t timeout)
{
	reg_addr = reg_addr & 0x3F;   //  R/W = 0 -> writing     and   M/S=0 -> fixed address
	
	HAL_GPIO_WritePin(myL3GD20->GPIOx, myL3GD20->CS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspix, &reg_addr, 1, timeout);
	HAL_SPI_Transmit(&hspix, &data, 1, timeout);
	
	HAL_GPIO_WritePin(myL3GD20->GPIOx, myL3GD20->CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Reads an 8-bit value from a register in the L3GD20
  * @param  myL3GD20: pointer to a gyroscope.
  * @param  reg_addr: register to read
  * @param  timeout: ammount of time to wait before response.
  * @retval Data contained in the register
  */

uint8_t ReadOneByte(SPI_L3GD20_t *myL3GD20, uint8_t reg_addr,  uint32_t timeout)
{
	reg_addr = reg_addr | 0x80;   // Reading
	reg_addr = reg_addr & 0xBF;   //  fixed address
	
  HAL_GPIO_WritePin(myL3GD20->GPIOx, myL3GD20->CS_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit(&hspix, &reg_addr, 1, timeout);
	
	uint8_t data;
	HAL_SPI_Receive(&hspix, &data, 1, timeout);
	
	HAL_GPIO_WritePin(myL3GD20->GPIOx, myL3GD20->CS_Pin, GPIO_PIN_SET);
	
	return data;
}

/**
  * @brief  Checks whether the gyroscope is well idetified or not.
  * @param  myL3GD20: pointer to a gyroscope.
  * @retval The status of the gyroscope, identified or not.
  */
L3GD20_Status isIdentified(SPI_L3GD20_t *myL3GD20)
{
	if (ReadOneByte(myL3GD20, L3GD20_REG_WHO_AM_I , L3GD20_DEFAULT_TIMEOUT) == L3GD20_WHO_AM_I)
		return L3GD20_IDENTIDIED;
	else
		return L3GD20_NOT_IDENTIDIED;
}

/**
  * @brief  Reads a 4-element data structure containing x, y, z and temp data retrieved from the gyroscope.
  * @param  myL3GD20: pointer to a gyroscope.
  * @retval A data-structure containing x, y, z, temperature.
  */
L3GD20_data_t L3GD20_Data_Read(SPI_L3GD20_t *myL3GD20)
{
	L3GD20_data_t dt;
	
	dt.x = ReadOneByte(myL3GD20 , L3GD20_REG_OUT_X_H, L3GD20_DEFAULT_TIMEOUT) << 8;
	dt.x |= ReadOneByte(myL3GD20, L3GD20_REG_OUT_X_L, L3GD20_DEFAULT_TIMEOUT);
	
	dt.y = ReadOneByte(myL3GD20 , L3GD20_REG_OUT_Y_H, L3GD20_DEFAULT_TIMEOUT) << 8;
	dt.y |= ReadOneByte(myL3GD20, L3GD20_REG_OUT_Y_L, L3GD20_DEFAULT_TIMEOUT);
	
	dt.z = ReadOneByte(myL3GD20 , L3GD20_REG_OUT_Z_H, L3GD20_DEFAULT_TIMEOUT) << 8;
  dt.z |= ReadOneByte(myL3GD20, L3GD20_REG_OUT_Z_L, L3GD20_DEFAULT_TIMEOUT);

	dt.temp = ReadOneByte(myL3GD20 , L3GD20_REG_OUT_TEMP, L3GD20_DEFAULT_TIMEOUT);
		
	return dt;
}

/**
  * @brief  Sets the power mode of the gyroscope.
  * @param  myL3GD20: pointer to a gyroscope.
  * @param  L3GD20_PowerMode: the mode that you would like to set.
  */
void L3GD20_Config_Power_Mode(SPI_L3GD20_t *myL3GD20, L3GD20_PowerMode mode)
{
	uint8_t old;
	
	old = ReadOneByte(myL3GD20 , L3GD20_REG_CTRL_REG1, L3GD20_DEFAULT_TIMEOUT);
	
	if(mode == L3GD20_MODE_POWER_DOWN){
		old &= 0xF7;
	}
	else if (L3GD20_MODE_SLEEP){
		old &= 0xF0;
		old |= 0x08;
	}
	else if(L3GD20_MODE_NORMAL){
		old |= 0x08;
	}
	WriteOneByte(myL3GD20 , L3GD20_REG_CTRL_REG1, old , L3GD20_DEFAULT_TIMEOUT);
	
}

/**
  * @brief  Sets the sensitivity (full scale) of the gyroscope.
  * @param  myL3GD20: pointer to a gyroscope.
	* @param  fs: full scale.
  */
void L3GD20_Config_Sensitivity(SPI_L3GD20_t *myL3GD20, L3GD20_Sensitivity fs)
{
	uint8_t old;
	
	old = ReadOneByte(myL3GD20 , L3GD20_REG_CTRL_REG4, L3GD20_DEFAULT_TIMEOUT);
	old &= 0xCF;
	old |= fs;
	
	WriteOneByte(myL3GD20 , L3GD20_REG_CTRL_REG4, old , L3GD20_DEFAULT_TIMEOUT);
}


/**
  * @brief  Sets the default configuration to the gyroscope.
  * @param  myL3GD20: pointer to a gyroscope.
  */
void L3GD20_Default_Config(SPI_L3GD20_t *myL3GD20)
{
	WriteOneByte(myL3GD20 , L3GD20_REG_CTRL_REG1, L3GD20_DEFAULT_CTRL_REG1_CONFIG , L3GD20_DEFAULT_TIMEOUT);
	WriteOneByte(myL3GD20 , L3GD20_REG_CTRL_REG2, L3GD20_DEFAULT_CTRL_REG2_CONFIG , L3GD20_DEFAULT_TIMEOUT);
	WriteOneByte(myL3GD20 , L3GD20_REG_CTRL_REG4, L3GD20_DEFAULT_CTRL_REG4_CONFIG , L3GD20_DEFAULT_TIMEOUT);
	
}



