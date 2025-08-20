/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "config.h"
#if USE_I2C
#include "stm32f1_i2c.h"
#include "stm32f1_gpio.h"

#ifndef I2C_TIMEOUT
	#define I2C_TIMEOUT	100
#endif

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

typedef enum
{
	I2C1_ID = 0,
	I2C2_ID,
	I2C_NB
}I2C_ID_e;

static I2C_HandleTypeDef  hi2c[I2C_NB];
void HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef *hi2c);
/**
 * @brief  Initializes I2C
 * @param  *I2Cx: I2C used
 * @param  clockSpeed: Clock speed for SCL in Hertz
 * @retval None
 */
HAL_StatusTypeDef I2C_Init(I2C_TypeDef* I2Cx, uint32_t clockSpeed)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);

	hi2c[id].Instance = I2Cx;
	hi2c[id].Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c[id].Init.ClockSpeed = clockSpeed;
	hi2c[id].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c[id].Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c[id].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c[id].Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	hi2c[id].Init.OwnAddress1 = 0x00;
	hi2c[id].Init.OwnAddress2 = 0x00;
	hi2c[id].Mode = HAL_I2C_MODE_NONE;
	hi2c[id].State = HAL_I2C_STATE_RESET;


	//Notes : en cas de soucis avec l'I2C busy : penser à initialiser l'I2C avant l'UART.
	//+ activer ceci :
	//HAL_I2C_ClearBusyFlagErrata_2_14_7(&hi2c[id]);

	if(id == I2C1_ID)
	{
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_I2C1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		#if I2C1_ON_PB6_PB7
			//I2C1 sur PB6 et PB7
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_6, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
		#else
			__HAL_AFIO_REMAP_I2C1_ENABLE();		//I2C1 sur PB8 et PB9.
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_8, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_9, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
		#endif
	}
	else
	{
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_I2C2_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		//I2C2 sur PB10 et PB11.
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_10, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_11, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	}


	return HAL_I2C_Init(&hi2c[id]);
}


void HAL_GPIO_WRITE_ODR(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->ODR |= GPIO_Pin;
}

/**
1. Disable the I2C peripheral by clearing the PE bit in I2Cx_CR1 register.
2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level
(Write 1 to GPIOx_ODR).
3. Check SCL and SDA High level in GPIOx_IDR.
4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to
GPIOx_ODR).
5. Check SDA Low level in GPIOx_IDR.
6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to
GPIOx_ODR).
7. Check SCL Low level in GPIOx_IDR.
8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to
GPIOx_ODR).
9. Check SCL High level in GPIOx_IDR.
10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to
GPIOx_ODR).
11. Check SDA High level in GPIOx_IDR.
12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
13. Set SWRST bit in I2Cx_CR1 register.
14. Clear SWRST bit in I2Cx_CR1 register.
15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register.
**/
void HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef *hi2c) {

    static uint8_t resetTried = 0;
    if (resetTried == 1) {
        return ;
    }
#if I2C1_ON_PB6_PB7
    uint32_t SDA_PIN = GPIO_PIN_7;
    uint32_t SCL_PIN = GPIO_PIN_6;
#else
    uint32_t SDA_PIN = GPIO_PIN_9;
    uint32_t SCL_PIN = GPIO_PIN_8;
#endif
    GPIO_InitTypeDef GPIO_InitStruct;

    if(hi2c->Instance == I2C2)
    {
    	SDA_PIN = GPIO_PIN_11;
    	SCL_PIN = GPIO_PIN_10;
    }
    // 1
    __HAL_I2C_DISABLE(hi2c);

	// 2
	GPIO_InitStruct.Pin = SDA_PIN|SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, SDA_PIN, 1);
	HAL_GPIO_WritePin(GPIOB, SCL_PIN, 1);

	// 3
	GPIO_PinState pinState;
	while(HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET);

	while(HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_RESET);

	// 4
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, SDA_PIN, 0);

	// 5
	while(HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_SET);

	// 6
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, SCL_PIN, 0);

	// 7
	while(HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_SET);

	// 8
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, SDA_PIN, 1);

	// 9
	while(HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET);

	// 10
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, SCL_PIN, 1);

	// 11
	while(HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_RESET);

	// 12
	GPIO_InitStruct.Pin = SDA_PIN|SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// 13
	hi2c->Instance->CR1 |= I2C_CR1_SWRST;

	// 14
	hi2c->Instance->CR1 ^= I2C_CR1_SWRST;

	// 15
	__HAL_I2C_ENABLE(hi2c);

   resetTried = 1;
}



/**
 * @brief  Reads single byte from slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to read from
 * @retval Data from slave
 */
HAL_StatusTypeDef I2C_Read16(I2C_TypeDef* I2Cx, uint16_t address, uint8_t reg, uint8_t * received_data)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Mem_Read(&hi2c[id],address,reg,I2C_MEMADD_SIZE_16BIT,received_data,1,I2C_TIMEOUT);
}


/**
 * @brief  Reads single byte from slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to read from
 * @retval Data from slave
 */
HAL_StatusTypeDef I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t * received_data)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Mem_Read(&hi2c[id],address,reg,I2C_MEMADD_SIZE_8BIT,received_data,1,I2C_TIMEOUT);
}


/**
 * @brief  Reads multi bytes from slave
 * @param  *I2Cx: I2C used
 * @param  uint8_t address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  uint8_t reg: register to read from
 * @param  uint8_t *data: pointer to data array to store data from slave
 * @param  uint8_t count: how many bytes will be read
 * @retval None
 */
HAL_StatusTypeDef I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Mem_Read(&hi2c[id],address,reg,I2C_MEMADD_SIZE_8BIT,data,count,I2C_TIMEOUT);
}

/**
 * @brief  Reads byte from slave without specify register address
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Data from slave
 */
HAL_StatusTypeDef I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t * data)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Master_Receive(&hi2c[id],address,data,1,I2C_TIMEOUT);
}

/**
 * @brief  Reads multi bytes from slave without setting register from where to start read
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to store data from slave
 * @param  count: how many bytes will be read
 * @retval None
 */
HAL_StatusTypeDef I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Master_Receive(&hi2c[id],address,data,count,I2C_TIMEOUT);
}

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
HAL_StatusTypeDef I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);

	return HAL_I2C_Mem_Write(&hi2c[id],address,reg,I2C_MEMADD_SIZE_8BIT,&data,1,I2C_TIMEOUT);
}

/**
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
HAL_StatusTypeDef I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Mem_Write(&hi2c[id],address,reg,I2C_MEMADD_SIZE_8BIT,data,count,I2C_TIMEOUT);
}

/**
 * @brief  Writes byte to slave without specify register address
 *
 *         Useful if you have I2C device to read like that:
 *            - I2C START
 *            - SEND DEVICE ADDRESS
 *            - SEND DATA BYTE
 *            - I2C STOP
 *
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  data: data byte which will be send to device
 * @retval None
 */
HAL_StatusTypeDef I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Master_Transmit(&hi2c[id],address,&data,1,I2C_TIMEOUT);
}

/**
 * @brief  Writes multi bytes to slave without setting register from where to start write
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  *data: pointer to data array to write data to slave
 * @param  count: how many bytes you want to write
 * @retval None
 */
HAL_StatusTypeDef I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return HAL_I2C_Master_Transmit(&hi2c[id],address,data,count,I2C_TIMEOUT);
}

/**
 * @brief  Checks if device is connected to I2C bus
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @retval Device status:
 *            - 0: Device is not connected
 *            - > 0: Device is connected
 */
bool_e I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return (HAL_I2C_IsDeviceReady(&hi2c[id],address,2,I2C_TIMEOUT) == HAL_OK)?TRUE:FALSE;
}

I2C_HandleTypeDef * I2C_get_handle(I2C_TypeDef* I2Cx)
{
	assert(I2Cx == I2C1 || I2Cx == I2C2);
	I2C_ID_e id = ((I2Cx == I2C2)?I2C2_ID:I2C1_ID);
	return &hi2c[id];
}

#endif

