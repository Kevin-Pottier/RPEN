/*
 * led.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#include "config.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"
#include "systick.h"
#include "led.h"

#define LED_BLINK_PERIOD	200
#define LED_FLASH_PERIOD	500
#define LED_FLASH_DURATION	50


static void LED_process_ms(void);
static volatile led_state_e led_state;
static volatile uint32_t t = 0;

void LED_init(void)
{
	//Initialisation du port de la led Verte  en sortie Push-Pull
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	LED_set(LED_OFF);
	Systick_add_callback_function(&LED_process_ms);
}


void LED_set(led_state_e state)
{
	if(led_state != state)
		t = 0;			//si on change de mode, on reset le compteur.

	led_state = state;	//on enregistre l'état demandé
}



static void LED_process_ms(void)
{
	switch(led_state)
	{
		case LED_BLINK:
			t++;
			if(t>LED_BLINK_PERIOD)
			{
				HAL_GPIO_TogglePin(LED_GREEN_GPIO, LED_GREEN_PIN);
				t = 0;
			}
			break;
		case LED_FLASH:
			t++;
			if(t==LED_FLASH_DURATION)
			{
				HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
			}
			else if(t>LED_FLASH_PERIOD)
			{
				HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
				t = 0;
			}
			break;
		case LED_OFF:
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, FALSE);
			break;
		case LED_ON:
			HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, TRUE);
			break;
	}
}
