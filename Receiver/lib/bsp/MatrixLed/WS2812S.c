/*
 * WS2812S.c
 *
 *  Created on: 3 mai 2016
 *      Author: Samuel Poiraud
 *
 *
 *      les envois de pixels sont sous-trait�s � la fonction assembleur WS2812S_send_pixel
 *      	https://github.com/Daedaluz/stm32-ws2812/tree/master/src
 *
 */

 /*	Sur la matrice de 64 WS2812, les leds sont chain�es ligne apr�s ligne.
 */
 
 
#include "config.h"
#if USE_MATRIX_LED

#include "stm32f1xx_hal.h"
#include "WS2812S.h"
#include "config.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"


#define COLOR_GREEN	0x000010
#define COLOR_BLUE	0x100000
#define COLOR_RED	0x001000
#define COLOR_BLACK	0x000000
#define T1H		1
#define T1L		1
#define T0H		0
#define T0L		1
#define RES     200

#define OUTPUT(x)	HAL_GPIO_WritePin(LED_MATRIX_PORT_DATA, LED_MATRIX_PIN_DATA, x)

void LED_MATRIX_init(void)
{
	BSP_GPIO_PinCfg(LED_MATRIX_PORT_DATA, LED_MATRIX_PIN_DATA, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
}


#if MY_BLUEPILL_IS_COUNTERFEIT
	//fonction asm
	extern void WS2812S_send_pixel_for_chinese_clone_STM32(uint32_t pixel, uint32_t gpio_pin_x, uint32_t * gpiox_bsrr);
	#define WS2812S_send_pixel	WS2812S_send_pixel_for_chinese_clone_STM32
#else
	//fonction asm
	extern void WS2812S_send_pixel(uint32_t pixel, uint32_t gpio_pin_x, uint32_t * gpiox_bsrr);
#endif

/*
extern void send_pixel(uint32_t pixel, uint32_t * gpiox_bsrr, uint32_t * gpiox_brr);

//Cette fonction est cod�e en assembleur et respecte les d�lais des bits impos�s par le fabricant.
void send_pixel(uint32_t pixel, uint32_t * gpiox_bsrr, uint32_t * gpiox_brr)
{
	uint32_t bit;
	uint32_t i;
	i = 0;
	do
	{
		bit = (pixel >> i) & 1;
		if(bit)
			*gpiox_bsrr = 1;
		else
			*gpiox_brr = 1;
	}while(i<32);
}
*/


void LED_MATRIX_send_pixel(uint32_t pixel)
{
	WS2812S_send_pixel(pixel, LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
}

void LED_MATRIX_demo(void)
{
	static uint8_t j = 0;
	uint8_t i;
	LED_MATRIX_init();
	uint32_t pixels[1];
	pixels[0] = COLOR_BLACK;
	LED_MATRIX_display(pixels, 1);
	HAL_Delay(500);
	pixels[0] = 0x100000;
	LED_MATRIX_display(pixels, 1);
	HAL_Delay(500);
	pixels[0] = 0x001000;
	LED_MATRIX_display(pixels, 1);
	HAL_Delay(500);
	pixels[0] = 0x000010;
	LED_MATRIX_display(pixels, 1);
	HAL_Delay(500);
	pixels[0] = 0x101000;
	LED_MATRIX_display(pixels, 1);
	HAL_Delay(500);
	pixels[0] = 0x100010;
	LED_MATRIX_display(pixels, 1);

}

void LED_MATRIX_color(int color){

	LED_MATRIX_init();
	uint32_t pixels[1];
	uint32_t colors[5] = {COLOR_BLACK, COLOR_RED, COLOR_BLUE, COLOR_GREEN, 0x45734D };
	pixels[0] = colors[color];
	LED_MATRIX_display(pixels, 1);

}

void LED_MATRIX_PutColor(uint32_t color){
	LED_MATRIX_init();
	uint32_t pixels[1];
	pixels[0] = color;
	LED_MATRIX_display_full(pixels,1);
}

/*
 * @brief	Cette fonction envoie 64 pixels vers la matrice de leds.
 * @note	les envois de pixels sont sous-trait�s � la fonction assembleur WS2812S_send_pixel
 * 			Cette fonction est r�dig�e en assembleur pour respecter scrupuleusement les d�lais de production des signaux pour les leds de la matrice.
 * 			Remarque : les interruptions sont d�sactiv�es temporairement pendant l'ex�cution de cette fonction pour �viter qu'elles provoquent des 'pauses' lors de la production des signaux.
 * 			La dur�e d'ex�cution de cette fonction est de l'ordre de 2,5ms. Dur�e pendant laquelle aucune interruption ne peut survenir !!!
 * @param 	pixels est un tableau de 64 cases absolument...
 * @note	attention, le tableau de pixels correspond aux leds dans l'ordre o� elles sont c�bl�es. Sur la matrice 8x8, elles sont reli�es en serpentin ! (et non en recommancant � gauche � chaque nouvelle ligne)...
 */
void LED_MATRIX_display(uint32_t * pixels, uint8_t size)
{
	uint8_t i;
	__disable_irq();
	LED_MATRIX_reset();
	for(i=0;i<size;i++)
		WS2812S_send_pixel(pixels[i], LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
	__enable_irq();
}

void LED_MATRIX_display_only_one_pixel(uint32_t pixel, uint8_t rank, uint8_t size)
{
	uint8_t i;
	__disable_irq();
	LED_MATRIX_reset();
	for(i=0;i<size;i++)
		WS2812S_send_pixel((i==rank)?pixel:COLOR_BLACK, LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
	__enable_irq();
}

void LED_MATRIX_display_full(uint32_t pixel, uint8_t size)
{
	uint8_t i;
	__disable_irq();
	LED_MATRIX_reset();
	for(i=0;i<size;i++)
		WS2812S_send_pixel(pixel, LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
	__enable_irq();
}

void LED_MATRIX_reset(void){

	int i;
	OUTPUT(0);
	Delay_us(100);
	//for(i = 0; i < RES; i++);	//Utilisez cette fonction et regl�e RES si la fonction Delay_us n'est pas disponible.
}
#endif
