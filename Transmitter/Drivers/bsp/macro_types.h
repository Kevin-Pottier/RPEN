/*
 * macro_types.h
 *
 *  Created on: 30 oct. 2013
 *      Author: Nirgal
 */

#ifndef MACRO_TYPES_H_
#define MACRO_TYPES_H_
#include <stdio.h>

//Macro permettant d'activer un envoi sur l'UART de ce qui est affiché sur le LCD...
//Utile si on a pas de LCD par exemple.
#define PRINT_ON_UART_LCD_DISPLAY

#define TRUE 1
#define FALSE 0
	/*typedef enum
	{
		FALSE = 0,
		TRUE
	}bool_e;*/

#define bool_e int

	typedef enum
	{
		IN_PROGRESS = 0,
		END_OK,
		END_ERROR,
		END_TIMEOUT
	}running_e;
	//Enum renvoyée par une machine à états.

	typedef unsigned char Uint8;
	typedef signed char Sint8;
	typedef unsigned short Uint16;
	typedef signed short Sint16;
	typedef unsigned long int Uint32;
	typedef signed long int Sint32;
	typedef unsigned long long Uint64;
	typedef signed long long Sint64;

	#define HIGHINT(x)				((uint8_t)(((x) >> 8) & 0xFF))
	#define LOWINT(x)				((uint8_t)((x) & 0xFF))
	#define U16FROMU8(high,low)		((((Uint16)(high))<<8)|(Uint16)(low))
	#define U32FROMU16(high,low)		((((Uint32)(high))<<16)|(Uint32)(low))
	#define U32FROMU8(higher,high,low,lower)		((((Uint32)(higher))<<24)|(((Uint32)(high))<<16)|(((Uint32)(low))<<8)|(Uint32)(lower))
	#define BITS_ON(var, mask)		((var) |= (mask))
	/* ~0 est le complement à 1 de 0, donc pour 16 bits OxFFFF) */
	/* ~0 ^ mask permet d'etre indépendant de la taille (en bits) de ~mask */
	#define BITS_OFF(var, mask)		((var) &= ~0 ^ (mask))
	#define BIT_SET(var, bitno)		((var) |= (1 << (bitno)))
	#define BIT_CLR(var, bitno)		((var) &= ~(1 << (bitno)))
	#define BIT_TEST(data, bitno)	(((data) >> (bitno)) & 0x01)
	#define MIN(a, b)				(((a) > (b)) ? (b) : (a))
	#define MAX(a, b)				(((a) > (b)) ? (a) : (b))


	#define nop()					__asm__("nop")
	/* la fonction valeur absolue pour des entiers */
	#define absolute(x)					(((x) >= 0) ? (x) : (-(x)))
	#define MODULO(x, N) ((x % N + N) % N)		//vrai modulo, avec un résultat non signé. Car l'opérateur % renvoit le reste signé de la division entière.


#if !TRACE
	#define assert(condition) if(!(condition)) {printf("assert failed file " __FILE__ " line %d : %s", __LINE__ , #condition ); NVIC_SystemReset();}
#else
	#include "trace/Trace.h"
	#define assert(condition) if(!(condition)) {trace_printf("assert failed file " __FILE__ " line %d : %s", __LINE__ , #condition ); NVIC_SystemReset();}
#endif
	#define debug_printf(...)			printf(__VA_ARGS__)
	#ifdef PRINT_ON_UART_LCD_DISPLAY
		#define lcd_to_uart_printf(...)		printf(__VA_ARGS__)
	#else
	#define lcd_to_uart_printf(...)		(void)(0)
	#endif


	typedef void(*callback_fun_t)(void);	//Type pointeur sur fonction


#endif /* MACRO_TYPES_H_ */
