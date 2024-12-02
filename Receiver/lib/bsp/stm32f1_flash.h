/*
 * stm32f1_flash.h
 *
 *  Created on: 11 juil. 2019
 *      Author: Nirgal
 */

#ifndef BSP_STM32F1_FLASH_H_
#define BSP_STM32F1_FLASH_H_

#include "config.h"
#if USE_INTERNAL_FLASH_AS_EEPROM

uint32_t FLASH_read_word(uint32_t index);
void FLASH_set_word(uint32_t index, uint32_t data);
void FLASH_dump(void);

#endif
#endif /* BSP_STM32F1_FLASH_H_ */
