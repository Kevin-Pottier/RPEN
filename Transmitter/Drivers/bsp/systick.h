/*
 * systick.h
 *
 *  Created on: 4 mai 2016
 *      Author: Nirgal
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "macro_types.h"
#include <stdint.h>

void Systick_init(void);

//Routine d'interruption appel�e automatiquement � chaque ms.
void SysTick_Handler(void);


bool_e Systick_add_callback_function(callback_fun_t func);


bool_e Systick_remove_callback_function(callback_fun_t func);

uint32_t SYSTICK_get_time_us(void);

#endif /* SYSTICK_H_ */
