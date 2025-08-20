/*
 * stm32f1_extit.c
 *
 *  Created on: 22 mai 2019
 *      Author: Nirgal
 */

#include "config.h"
#if USE_BSP_EXTIT
#include "stm32f1_extit.h"
#include "stm32f1xx_hal.h"


static callback_extit_t callbacks[16] = {0};
static uint16_t enables = 0;

/*
 * @brief cette fonction permet de déclarer une fonction de callback, associée à un numéro de broche.
 */
void EXTIT_set_callback(callback_extit_t fun, uint8_t pin_number, bool_e enable)
{
	callbacks[pin_number] = fun;
	if(enable)
		EXTIT_enable(pin_number);
}

/*
 * @brief cette fonction autorise les interruptions externes correspondant au numéro de broche demandé.
 */
void EXTIT_enable(uint8_t pin_number)
{
	if(pin_number < 16)
		BIT_SET(enables, pin_number);
	switch(pin_number)
	{
		case 0:	NVIC_EnableIRQ(EXTI0_IRQn);	break;
		case 1:	NVIC_EnableIRQ(EXTI1_IRQn);	break;
		case 2:	NVIC_EnableIRQ(EXTI2_IRQn);	break;
		case 3:	NVIC_EnableIRQ(EXTI3_IRQn);	break;
		case 4:	NVIC_EnableIRQ(EXTI4_IRQn);	break;
		default:
			if(pin_number < 10)
				NVIC_EnableIRQ(EXTI9_5_IRQn);
			else if(pin_number < 16)
				NVIC_EnableIRQ(EXTI15_10_IRQn);
			break;
	}
}

/*
 * @post : l'it correspondante ne sera désactivée que si aucune autre source partageant la même it n'est active.
 */
void EXTIT_disable(uint8_t pin_number)
{
	if(pin_number < 16)
		BIT_CLR(enables, pin_number);

	switch(pin_number)
	{
		case 0:	NVIC_DisableIRQ(EXTI0_IRQn);	break;
		case 1:	NVIC_DisableIRQ(EXTI1_IRQn);	break;
		case 2:	NVIC_DisableIRQ(EXTI2_IRQn);	break;
		case 3:	NVIC_DisableIRQ(EXTI3_IRQn);	break;
		case 4:	NVIC_DisableIRQ(EXTI4_IRQn);	break;
		default:
			if(pin_number < 10)
			{
				if(((enables >> 5)&0b11111) == 0)
					NVIC_DisableIRQ(EXTI9_5_IRQn);
			}
			else if(pin_number < 16)
			{
				if(enables >> 10 == 0)
					NVIC_DisableIRQ(EXTI15_10_IRQn);
			}
			break;
	}
}


uint8_t EXTI_gpiopin_to_pin_number(uint16_t GPIO_PIN_x)
{
	uint8_t ret = -1;
	switch(GPIO_PIN_x)
	{
		case GPIO_PIN_0:	ret = 0;	break;
		case GPIO_PIN_1:	ret = 1;	break;
		case GPIO_PIN_2:	ret = 2;	break;
		case GPIO_PIN_3:	ret = 3;	break;
		case GPIO_PIN_4:	ret = 4;	break;
		case GPIO_PIN_5:	ret = 5;	break;
		case GPIO_PIN_6:	ret = 6;	break;
		case GPIO_PIN_7:	ret = 7;	break;
		case GPIO_PIN_8:	ret = 8;	break;
		case GPIO_PIN_9:	ret = 9;	break;
		case GPIO_PIN_10:	ret = 10;	break;
		case GPIO_PIN_11:	ret = 11;	break;
		case GPIO_PIN_12:	ret = 12;	break;
		case GPIO_PIN_13:	ret = 13;	break;
		case GPIO_PIN_14:	ret = 14;	break;
		case GPIO_PIN_15:	ret = 15;	break;
		default:
			break;
	}
	return ret;
}

void EXTI_ack_it(uint8_t pin_number)
{
	uint16_t gpio_pin;
	gpio_pin = (uint16_t)(1) << (uint16_t)(pin_number);
	if(__HAL_GPIO_EXTI_GET_IT(gpio_pin))
		__HAL_GPIO_EXTI_CLEAR_IT(gpio_pin);
}


/*
 * pin vaut GPIO_PIN_x
 */
static void EXTI_call(uint8_t pin_number)
{
	uint16_t gpio_pin;
	gpio_pin = (uint16_t)(1) << (uint16_t)(pin_number);
	if(__HAL_GPIO_EXTI_GET_IT(gpio_pin))
	{
		__HAL_GPIO_EXTI_CLEAR_IT(gpio_pin);
		if(enables & gpio_pin)
		{
			if(callbacks[pin_number])
				(*callbacks[pin_number])(gpio_pin);
		}
	}
}



void EXTI0_IRQHandler(void)
{
	EXTI_call(0);
}

void EXTI1_IRQHandler(void)
{
	EXTI_call(1);
}

void EXTI2_IRQHandler(void)
{
	EXTI_call(2);
}

void EXTI3_IRQHandler(void)
{
	EXTI_call(3);
}

void EXTI4_IRQHandler(void)
{
	EXTI_call(4);
}


void EXTI9_5_IRQHandler(void)
{
	EXTI_call(5);
	EXTI_call(6);
	EXTI_call(7);
	EXTI_call(8);
	EXTI_call(9);
}

void EXTI15_10_IRQHandler(void)
{
	EXTI_call(10);
	EXTI_call(11);
	EXTI_call(12);
	EXTI_call(13);
	EXTI_call(14);
	EXTI_call(15);
}

#endif
