/*
 * stm32f1_extit.h
 *
 *  Created on: 22 mai 2019
 *      Author: Nirgal
 */

#ifndef STM32F1_EXTIT_H_
#define STM32F1_EXTIT_H_
#include "config.h"
#if USE_BSP_EXTIT
#include "macro_types.h"

typedef void(*callback_extit_t)(uint16_t pin);	//Type pointeur sur fonction
//le paramètre pin vaudra GPIO_PIN_x (x sur l'intervalle [0..15])

void EXTIT_set_callback(callback_extit_t fun, uint8_t pin_number, bool_e enable);

void EXTIT_enable(uint8_t pin_number);

void EXTIT_disable(uint8_t pin_number);

uint8_t EXTI_gpiopin_to_pin_number(uint16_t GPIO_PIN_x);

void EXTI_ack_it(uint8_t pin_number);

#endif //USE_BSP_EXTIT
#endif /* STM32F1_EXTIT_H_ */
