/**
 * demo_motor.h
 *
 *  Created on: 27 octobre 2015
 *      Author: S. Poiraud
 */

#ifndef DEMO_MOTOR_H_
#define DEMO_MOTOR_H_

#include "macro_types.h"
#include "stdint.h"


typedef enum
{
	MOTOR_ID_0,
	MOTOR_ID_1,
	MOTOR_ID_2,
	MOTOR_ID_3,
	MOTOR_NB,		//nombre max de moteurs !
	MOTOR_ID_NONE =-1
}motor_id_e;

running_e DEMO_MOTOR_statemachine (bool_e ask_for_finish, char touch_pressed);

motor_id_e MOTOR_add(GPIO_TypeDef * gpio_forward, uint16_t pin_forward, GPIO_TypeDef * gpio_reverse, uint16_t pin_reverse);

void MOTOR_set_duty(motor_id_e id, int16_t duty);

#endif /* DEMO_MOTOR_H_ */
