/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : L3GD20 library header files.
	* Author             : Noureddine-as
	* Email              : noureddine.asni@gmail.com
	* GitHub repository  : https://github.com/noureddine-as
	* Designed using     : STM32CubeMX + ARM Keil v5
  ******************************************************************************/

#ifndef __L3GD20_H
#define __L3GD20_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f4xx_hal.h"

		/* Identification number */
#define L3GD20_DEFAULT_TIMEOUT		100
#define L3GD20_DEFAULT_BAUDERATEPRESCALER		(SPI_BAUDRATEPRESCALER_64)

	
	/* Identification number */
#define L3GD20_WHO_AM_I						0xD4

/* Registers addresses */
#define L3GD20_REG_WHO_AM_I				0x0F
#define L3GD20_REG_CTRL_REG1			0x20
#define L3GD20_REG_CTRL_REG2			0x21
#define L3GD20_REG_CTRL_REG3			0x22
#define L3GD20_REG_CTRL_REG4			0x23
#define L3GD20_REG_CTRL_REG5			0x24
#define L3GD20_REG_REFERENCE			0x25
#define L3GD20_REG_OUT_TEMP				0x26
#define L3GD20_REG_STATUS_REG			0x27
#define L3GD20_REG_OUT_X_L				0x28
#define L3GD20_REG_OUT_X_H				0x29
#define L3GD20_REG_OUT_Y_L				0x2A
#define L3GD20_REG_OUT_Y_H				0x2B
#define L3GD20_REG_OUT_Z_L				0x2C
#define L3GD20_REG_OUT_Z_H				0x2D
#define L3GD20_REG_FIFO_CTRL_REG	0x2E
#define L3GD20_REG_FIFO_SRC_REG		0x2F
#define L3GD20_REG_INT1_CFG				0x30
#define L3GD20_REG_INT1_SRC		  	0x31
#define L3GD20_REG_INT1_TSH_XH		0x32
#define L3GD20_REG_INT1_TSH_XL		0x33
#define L3GD20_REG_INT1_TSH_YH		0x34
#define L3GD20_REG_INT1_TSH_YL		0x35
#define L3GD20_REG_INT1_TSH_ZH		0x36
#define L3GD20_REG_INT1_TSH_ZL		0x37
#define L3GD20_REG_INT1_DURATION	0x38

// Default control gegisters config
#define L3GD20_DEFAULT_CTRL_REG1_CONFIG		0xEF    /* Data Rate | Bandwidth | Power Down active  | X,Y,Z enabled */
#define L3GD20_DEFAULT_CTRL_REG2_CONFIG		0x29   /* normal mode ...*/
#define L3GD20_DEFAULT_CTRL_REG4_CONFIG		0x30    /* 2000 dps full scale   |   ---   | 4-Wire SPI Interface   */
	
	
	/* BAUDRATES defined on the hal_spi.h
	
#define SPI_BAUDRATEPRESCALER_2         ((uint32_t)0x00000000)
#define SPI_BAUDRATEPRESCALER_4         ((uint32_t)0x00000008)
#define SPI_BAUDRATEPRESCALER_8         ((uint32_t)0x00000010)
#define SPI_BAUDRATEPRESCALER_16        ((uint32_t)0x00000018)
#define SPI_BAUDRATEPRESCALER_32        ((uint32_t)0x00000020)
#define SPI_BAUDRATEPRESCALER_64        ((uint32_t)0x00000028)
#define SPI_BAUDRATEPRESCALER_128       ((uint32_t)0x00000030)
#define SPI_BAUDRATEPRESCALER_256       ((uint32_t)0x00000038)

*/
	
typedef struct {
	SPI_TypeDef *SPIx;
	GPIO_TypeDef  *GPIOx;
	uint32_t CS_Pin;
	uint32_t BaudRatePrescaler; // BAUDRATEPRESCALER_64;
} SPI_L3GD20_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
	int8_t temp;
} L3GD20_data_t;

typedef enum {
	L3GD20_NOT_IDENTIDIED = 0,
	L3GD20_IDENTIDIED

}L3GD20_Status;

typedef enum {
	L3GD20_MODE_POWER_DOWN = 0,
	L3GD20_MODE_SLEEP,
	L3GD20_MODE_NORMAL
	
}L3GD20_PowerMode;

typedef enum {
	L3GD20_SENSITIVITY_250 = 0xCF,
	L3GD20_SENSITIVITY_500 = 0xDF,
	L3GD20_SENSITIVITY_2000 = 0xFF
	
}L3GD20_Sensitivity;

/* Functions prototypes  */

// Initializes only the SPI communication *Don't have to be called by the user
void MX_SPIx_Init(SPI_TypeDef *SPIx, uint32_t BaudRatePrescaler);


// This ones initializes the structure SPI_L3GD20_t and all the communication.
// USER has only to create an SPI_L3GD20_t structure
// Then call L3GD20_Init ... that allows to initialize all the necessary things
void L3GD20_Init(SPI_L3GD20_t *myL3GD20, SPI_TypeDef * SPIx, GPIO_TypeDef  *GPIOx, uint32_t CS_Pin,	uint32_t BaudRatePrescaler);


// Read/Write bytes to the gyro.
void WriteOneByte(SPI_L3GD20_t *myL3GD20, uint8_t reg_addr, uint8_t data, uint32_t timeout);
uint8_t ReadOneByte(SPI_L3GD20_t *myL3GD20, uint8_t reg_addr,  uint32_t timeout);


// Returns a L3GD20_data_t containing the data structure containing the accelerations and the temperature
L3GD20_data_t L3GD20_Data_Read(SPI_L3GD20_t *myL3GD20);


// Configurations
void L3GD20_Config_Power_Mode(SPI_L3GD20_t *myL3GD20, L3GD20_PowerMode mode);
void L3GD20_Config_Sensitivity(SPI_L3GD20_t *myL3GD20, L3GD20_Sensitivity fs);
void L3GD20_Default_Config(SPI_L3GD20_t *myL3GD20);
	
/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
