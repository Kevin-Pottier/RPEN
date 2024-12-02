/**
 * @author Samuel Poiraud.
 * 			Ce code pour STM32F103 est inspiré du code de Tilen Majerle (pour STM32F4).
 * 			Les références vers le (superbe) travail de Tilen Majerle sont disponibles ci dessous.
 *
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/05/library-09-i2c-for-stm32f4xx/
 * @version v1.6.1
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   I2C library for STM32F4xx
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_I2C_H
#define TM_I2C_H 161
#include "macro_types.h"
/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

#include "stm32f1xx_hal.h"

HAL_StatusTypeDef I2C_Init(I2C_TypeDef* I2Cx, uint32_t clockSpeed);

HAL_StatusTypeDef I2C_Read(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t * received_data);

HAL_StatusTypeDef I2C_ReadMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

HAL_StatusTypeDef I2C_ReadNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t * data);

HAL_StatusTypeDef I2C_ReadMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

HAL_StatusTypeDef I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t data);

HAL_StatusTypeDef I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);

HAL_StatusTypeDef I2C_WriteNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t data);

HAL_StatusTypeDef I2C_WriteMultiNoRegister(I2C_TypeDef* I2Cx, uint8_t address, uint8_t* data, uint16_t count);

bool_e I2C_IsDeviceConnected(I2C_TypeDef* I2Cx, uint8_t address);

I2C_HandleTypeDef * I2C_get_handle(I2C_TypeDef* I2Cx);


#endif

