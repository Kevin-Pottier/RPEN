/*
 * stm32f1_spi.h
 *
 *  Created on: 19 mai 2016
 *      Author: Nirgal
 */

#ifndef BSP_STM32F1_SPI_H_
#define BSP_STM32F1_SPI_H_

#include "stm32f1xx_hal.h"
#include "macro_types.h"

void SPI_Init(SPI_TypeDef* SPIx);

void SPI_config_remap(bool_e SPI_remap);

uint8_t SPI_ReadNoRegister(SPI_TypeDef* SPIx);

void SPI_ReadMultiNoRegister(SPI_TypeDef* SPIx, uint8_t* data, uint16_t count);

void SPI_WriteNoRegister(SPI_TypeDef* SPIx, uint8_t data);

void SPI_WriteMultiNoRegister(SPI_TypeDef* SPIx, uint8_t* data, uint16_t count);

uint8_t SPI_WriteRead(SPI_TypeDef* SPIx, uint8_t Value);

void SPI_WriteReadBuffer(SPI_TypeDef* SPIx, const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);

void SPI_setBaudRate(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler);

typedef enum {
	TM_SPI_DataSize_8b, /*!< SPI in 8-bits mode */
	TM_SPI_DataSize_16b /*!< SPI in 16-bits mode */
} TM_SPI_DataSize_t;

TM_SPI_DataSize_t TM_SPI_SetDataSize(SPI_TypeDef* SPIx, TM_SPI_DataSize_t DataSize);

#endif /* BSP_STM32F1_SPI_H_ */
