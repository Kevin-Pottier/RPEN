/*
 * led.h
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#ifndef LED_H_
#define LED_H_

typedef enum
{
	LED_OFF = 0,
	LED_ON,
	LED_BLINK,
	LED_FLASH
}led_state_e;


void LED_init(void);

void LED_set(led_state_e led_state);


#endif /* LED_H_ */
