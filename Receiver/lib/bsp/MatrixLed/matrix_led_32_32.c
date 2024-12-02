/* 
 * File:   matrix.c
 * Author: ponsguil
 *
 * Created on 19 juin 2017, 11:28
 */
#include "config.h"
#if USE_MATRIX_LED_32_32
#include "matrix_led_32_32.h"
#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include <stm32f1_gpio.h>
#include "macro_types.h"

//IMPORTANT :
//Note : ce code n'a pas été testé sur cible STM32, il est issu d'un portage de code qui fonctionne sur PIC18.
//Il se peut qu'il faille traduire une partie en assembleur afin de limiter le clignotement
//Une réflexion sur le signal OE peut rester à mener.


//Adaptez les broches à vos choix hardware :

#define ZONE_A   	GPIOA, GPIO_PIN_0
#define ZONE_B   	GPIOA, GPIO_PIN_1
#define ZONE_C   	GPIOA, GPIO_PIN_2
#define ZONE_D   	GPIOA, GPIO_PIN_3

//HIGH / LOW : parties haute ou basse de la matrice !
//R0, G0, B0
#define HIGH_RED      GPIOA, GPIO_PIN_4
#define HIGH_GREEN    GPIOA, GPIO_PIN_5
#define HIGH_BLUE     GPIOA, GPIO_PIN_6
//R1, G1, B1
#define LOW_RED       GPIOA, GPIO_PIN_7
#define LOW_GREEN     GPIOA, GPIO_PIN_8
#define LOW_BLUE      GPIOA, GPIO_PIN_9

#define CLK GPIOA, GPIO_PIN_10
#define LATCH GPIOA, GPIO_PIN_11
#define OE  GPIOA, GPIO_PIN_12

#define WRITE(x,y)					HAL_GPIO_WritePin(x,y)
#define CONFIG_OUTPUT(port_pin)		BSP_GPIO_PinCfg(port_pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH)

static bool_e initialized = FALSE;

//Précondition : les GPIO doivent être activés et initialisés.
void MATRIX_init(void)
{
	CONFIG_OUTPUT(ZONE_A);
	CONFIG_OUTPUT(ZONE_B);
	CONFIG_OUTPUT(ZONE_C);
	CONFIG_OUTPUT(ZONE_D);
	CONFIG_OUTPUT(HIGH_RED);
	CONFIG_OUTPUT(HIGH_GREEN);
	CONFIG_OUTPUT(HIGH_BLUE);
	CONFIG_OUTPUT(LOW_RED);
	CONFIG_OUTPUT(LOW_GREEN);
	CONFIG_OUTPUT(LOW_BLUE);
	CONFIG_OUTPUT(CLK);
	CONFIG_OUTPUT(LATCH);
	CONFIG_OUTPUT(OE);

    WRITE(OE, 0);
    WRITE(LATCH, 0);
    initialized = TRUE;
}


void MATRIX_display(color_t color[32][32])
{
    uint8_t zone;
    uint8_t led;
    //Sécurité si l'initialisation n'a pas été appelée.
    if(!initialized)
    	MATRIX_init();
    
	WRITE(OE, 0);
	for(zone= 0 ; zone < 16 ; zone++)
	{
		WRITE(ZONE_A,(zone >> 0)&1);
		WRITE(ZONE_B,(zone >> 1)&1);
		WRITE(ZONE_C,(zone >> 2)&1);
		WRITE(ZONE_D,(zone >> 3)&1);

		for(led = 0; led < 32; led++)
		{
			WRITE(HIGH_RED, 	(color[zone][led] 		>> 2) & 1);
			WRITE(LOW_RED, 		(color[16+zone][led] 	>> 2) & 1);

			WRITE(HIGH_GREEN, 	(color[zone][led] 		>> 1) & 1);
			WRITE(LOW_GREEN, 	(color[16+zone][led] 	>> 1) & 1);

			WRITE(HIGH_BLUE, 	(color[zone][led] 		>> 0) & 1);
			WRITE(LOW_BLUE, 	(color[16+zone][led]	>> 0) & 1);

			WRITE(CLK,1);
			WRITE(CLK,0);
		}
		WRITE(LATCH, 1);
		WRITE(LATCH, 0);

	}
	WRITE(OE, 1);
}
#endif
