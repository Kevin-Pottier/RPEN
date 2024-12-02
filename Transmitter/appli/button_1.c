/*
 * button.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */
#include <button_1.h>
#include "config.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#define LONG_PRESS_DURATION	1000	//unité : [1ms] => 1 seconde.

static void process_ms(void);

static volatile bool_e flag_10ms;
static volatile uint32_t t_1 = 0;
static volatile uint32_t t_2 = 0;
static volatile uint32_t t_3 = 0;
static bool_e initialized = FALSE;

void BUTTON_1_init(void)
{
	//Initialisation du port du bouton bleu en entrée
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_12, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	Systick_add_callback_function(&process_ms);

	initialized = TRUE;
}
void BUTTON_2_init(void)
{
	//Initialisation du port du bouton bleu en entrée
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_11, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	Systick_add_callback_function(&process_ms);

	initialized = TRUE;
}
void BUTTON_3_init(void)
{
	//Initialisation du port du bouton bleu en entrée
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	Systick_add_callback_function(&process_ms);

	initialized = TRUE;
}

static void process_ms(void)
{
	static uint32_t t10ms = 0;
	t10ms = (t10ms + 1)%10;		//incrémentation de la variable t10ms (modulo 10 !)
	if(!t10ms)
		flag_10ms = TRUE; //toutes les 10ms, on lève ce flag.
	if(t_1)
		t_1--;
	if(t_2)
			t_2--;
	if(t_3)
			t_3--;
}

/**
	Ces machines à états gèrent la détection d'appuis sur le bouton bleu.
	Elle doit être appelée en boucle très régulièrement.
	Précondition : avoir appelé auparavant BUTTON_init();
	Si un appui vient d'être fait, elle renverra BUTTON_EVENT_SHORT_PRESS ou BUTTON_EVENT_LONG_PRESS
*/
button_event_e BUTTON_1_state_machine(void)

{
	typedef enum
	{
		INIT = 0,
		WAIT_BUTTON,	//En C, les nombres se suivent dans une enum.
		BUTTON_PRESSED,
		WAIT_RELEASE
	}state_e;

	static state_e state = INIT; //La variable d'état, = INIT au début du programme !
	/**	Le mot clé static est INDISPENSABLE :
	* 	"state" DOIT GARDER SA VALEUR d'un appel à l'autre de la fonction.
	*	Une place lui est réservée en mémoire de façon permanente
	*	(et non pas temporaire dans la pile !)
	*/

	button_event_e ret = BUTTON_EVENT_NONE;
	bool_e current_button;

	if(flag_10ms && initialized)	//le cadencement de cette portion de code à 10ms permet d'éliminer l'effet des rebonds sur le signal en provenance du bouton.
	{
		flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
		switch(state)
		{
			case INIT:
				state = WAIT_BUTTON;	//Changement d'état
				break;
			case WAIT_BUTTON:
				ret =BUTTON_EVENT_NONE;
				if(current_button)
				{

					t_1=LONG_PRESS_DURATION;	//Action réalisée sur la transition.
					state = BUTTON_PRESSED;	//Changement d'état conditionné à "if(current_button)"
				}
				break;
			case BUTTON_PRESSED:
				if(t_1==0)
				{
					ret = BUTTON_EVENT_LONG_PRESS;

					state = WAIT_RELEASE;		//le temps est écoulé, c'était un appui long !
				}
				else if(!current_button)
				{
					ret = BUTTON_EVENT_SHORT_PRESS;

					state = WAIT_BUTTON;	//le bouton a été relâché avant l'écoulement du temps, c'était un appui court !
				}
				break;

			case WAIT_RELEASE:
				if(!current_button)
				{

					state = WAIT_BUTTON;
				}
				break;
			default:
				state = INIT;	//N'est jamais sensé se produire.
				break;
		}
	}
	return ret;
}
button_event_e BUTTON_2_state_machine(void)
{
	typedef enum
	{
		INIT = 0,
		WAIT_BUTTON,	//En C, les nombres se suivent dans une enum.
		BUTTON_PRESSED,
		WAIT_RELEASE
	}state_e;

	static state_e state = INIT; //La variable d'état, = INIT au début du programme !
	/**	Le mot clé static est INDISPENSABLE :
	* 	"state" DOIT GARDER SA VALEUR d'un appel à l'autre de la fonction.
	*	Une place lui est réservée en mémoire de façon permanente
	*	(et non pas temporaire dans la pile !)
	*/

	button_event_e ret = BUTTON_EVENT_NONE;
	bool_e current_button;

	if(flag_10ms && initialized)	//le cadencement de cette portion de code à 10ms permet d'éliminer l'effet des rebonds sur le signal en provenance du bouton.
	{
		flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
		switch(state)
		{
			case INIT:
				state = WAIT_BUTTON;	//Changement d'état
				break;
			case WAIT_BUTTON:
				ret =BUTTON_EVENT_NONE;
				if(current_button)
				{

					t_2=LONG_PRESS_DURATION;	//Action réalisée sur la transition.
					state = BUTTON_PRESSED;	//Changement d'état conditionné à "if(current_button)"
				}
				break;
			case BUTTON_PRESSED:
				if(t_2==0)
				{
					ret = BUTTON_EVENT_LONG_PRESS;

					state = WAIT_RELEASE;		//le temps est écoulé, c'était un appui long !
				}
				else if(!current_button)
				{
					ret = BUTTON_EVENT_SHORT_PRESS;

					state = WAIT_BUTTON;	//le bouton a été relâché avant l'écoulement du temps, c'était un appui court !
				}
				break;

			case WAIT_RELEASE:
				if(!current_button)
				{

					state = WAIT_BUTTON;
				}
				break;
			default:
				state = INIT;	//N'est jamais sensé se produire.
				break;
		}
	}
	return ret;
}
button_event_e BUTTON_3_state_machine(void)
{
	typedef enum
	{
		INIT = 0,
		WAIT_BUTTON,	//En C, les nombres se suivent dans une enum.
		BUTTON_PRESSED,
		WAIT_RELEASE
	}state_e;

	static state_e state = INIT; //La variable d'état, = INIT au début du programme !
	/**	Le mot clé static est INDISPENSABLE :
	* 	"state" DOIT GARDER SA VALEUR d'un appel à l'autre de la fonction.
	*	Une place lui est réservée en mémoire de façon permanente
	*	(et non pas temporaire dans la pile !)
	*/

	button_event_e ret = BUTTON_EVENT_NONE;
	bool_e current_button;

	if(flag_10ms && initialized)	//le cadencement de cette portion de code à 10ms permet d'éliminer l'effet des rebonds sur le signal en provenance du bouton.
	{
		flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
		switch(state)
		{
			case INIT:
				state = WAIT_BUTTON;	//Changement d'état
				break;
			case WAIT_BUTTON:
				ret =BUTTON_EVENT_NONE;
				if(current_button)
				{

					t_3=LONG_PRESS_DURATION;	//Action réalisée sur la transition.
					state = BUTTON_PRESSED;	//Changement d'état conditionné à "if(current_button)"
				}
				break;
			case BUTTON_PRESSED:
				if(t_3==0)
				{
					ret = BUTTON_EVENT_LONG_PRESS;

					state = WAIT_RELEASE;		//le temps est écoulé, c'était un appui long !
				}
				else if(!current_button)
				{
					ret = BUTTON_EVENT_SHORT_PRESS;

					state = WAIT_BUTTON;	//le bouton a été relâché avant l'écoulement du temps, c'était un appui court !
				}
				break;

			case WAIT_RELEASE:
				if(!current_button)
				{

					state = WAIT_BUTTON;
				}
				break;
			default:
				state = INIT;	//N'est jamais sensé se produire.
				break;
		}
	}
	return ret;
}
