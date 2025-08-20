/*
 * dialog.c
 *
 *  Created on: 29 mai 2019
 *      Author: Nirgal
 */
#include "config.h"
#if USE_DIALOG
#include "dialog.h"
#include "macro_types.h"
#include "stm32f1_uart.h"

/*
 * Cette brique logicielle vise à permettre le dialogue par trames génériques entre deux entités.
 *
 * Les trames proposées sont constituées ainsi :
 *
 * SOH - lenght - datas - checksum - EOT
 *
 * SOH (Start Of Header) 0x01 			: début de trame
 * Lenght (16 bits) = 0xLLLL 			: taille des données dans datas (de 0 à 65535)
 * Checksum (8 bits) = 0xSS				: somme de contrôle (qui porte sur lenght+datas)
 * EOT (End Of Transmission) 0x04		: fin de transmission
 *
 */

#define SOH	0x01
#define EOT 0x04
/*
 * Pour utiliser ce module logiciel, il faut :
 * 1- appeler DIALOG_init en fournissant une fonction de callback qui sera appelée pour envoyer les octets.
 * 2- appeler DIALOG_process_byte() pour chaque octet reçu
 * 		Si une trame a été trouvée complète, cette fonction la renvoie.
 */

#ifndef DIALOG_BUFFER_SIZE
	#define DIALOG_BUFFER_SIZE	128	//vous pouvez régler cette taille de buffer... pourvu que vos trames les plus longues y logent
#endif


void DIALOG_DEMO_callback_send_byte(uint8_t c)
{
	UART_putc(UART2_ID, c);
}


//Fonction blocante permettant d'appréhender le fonctionnement de ce module logiciel.
void DIALOG_DEMO(void)
{
	DIALOG_init(&DIALOG_DEMO_callback_send_byte);

	#define PACKET_SIZE	50
	uint8_t packet[PACKET_SIZE];
	uint16_t size;
	size = (uint16_t)sprintf((char*)packet, "I love microprocessors\n");
	DIALOG_send_packet(size, packet);

	uint8_t * pdatas_received;	//pdatas_received est un pointeur vers caractère.
	uint8_t c;
	while(1)
	{
		if(UART_data_ready(UART2_ID))
		{
			c = UART_getc(UART2_ID);

			//Attention, on transmets ici l'adresse du pointeur.
			switch(DIALOG_process_byte(c, &pdatas_received))
			{
				case HAL_OK:
					debug_printf("We received datas : %s\n", pdatas_received);
					break;
				case HAL_ERROR:
					debug_printf("Error during trame reception\n");
					break;
				default:
					break;
			}
		}
	}
}




static callback_dialog_send_byte_t callback_send_byte = 0;
static bool_e initialized = FALSE;
static uint8_t datas[DIALOG_BUFFER_SIZE];
static uint16_t index = 0;

/*
 * @param callback est un pointeur sur fonction. Cette fonction sera appelé à chaque envoit d'octet par le module DIALOG.
 */
void DIALOG_init(callback_dialog_send_byte_t callback)
{
	if(callback)
		callback_send_byte = callback;
	index = 0;
	initialized = TRUE;
}

/*
 * Cette fonction doit être appelée chaque fois qu'un octet à été reçu. Cet octet est fourni dans c.
 * Si la fonction renvoit HAL_OK, cela signifie qu'une trame complète et valide a été reçue. On peut alors disposer de son contenu via le pointeur pdatas.
 */
HAL_StatusTypeDef DIALOG_process_byte(uint8_t c, uint8_t ** pdatas)
{
	typedef enum
	{
		STEP_SOH = 0,
		STEP_LENGHT_HIGH,
		STEP_LENGHT_LOW,
		STEP_DATAS,
		STEP_CHECKSUM,
		STEP_EOT
	}step_e;
	static step_e step = STEP_SOH;
	static uint16_t size;
	static uint32_t checksum;
	HAL_StatusTypeDef ret = HAL_BUSY;

	switch(step)
	{
		case STEP_SOH:
			if(c == SOH)
			{
				index = 0;
				step = STEP_LENGHT_HIGH;
				checksum = 0;
			}
			break;
		case STEP_LENGHT_HIGH:
			size = U16FROMU8(c, 0);
			checksum += c;
			step = STEP_LENGHT_LOW;
			break;
		case STEP_LENGHT_LOW:
			size += c;
			checksum += c;
			step = STEP_DATAS;
			break;
		case STEP_DATAS:
			datas[index] = c;
			checksum += c;
			index++;
			if(index == size)
				step = STEP_CHECKSUM;
			break;
		case STEP_CHECKSUM:
			if(c == (uint8_t)(checksum))
				step = STEP_EOT;
			else
			{
				ret = HAL_ERROR;		//somme de contrôle invalide.
				step = STEP_SOH;
			}
			break;
		case STEP_EOT:
			if(c == EOT)
			{
				ret = HAL_OK;			//trame complète !
				*pdatas = datas;		//on renvoit l'adresse du tableau de données qui peut être interprété tout de suite (avant le prochain appel de la fonction !)
			}
			else
			{
				ret = HAL_ERROR;
			}
			step = STEP_SOH;
			break;
		default:
			break;
	}
	return ret;
}






/**
 * @brief	Cette fonction permet l'envoi d'un message sur la liaison série.
 * @pre		Le tableau datas doit contenir au moins 'size' octet. Sinon, le pointeur 'datas' peut être NULL.
 */
void DIALOG_send_packet(uint16_t size, uint8_t * datas)
{
	uint16_t i;
	uint32_t checksum;
	if(callback_send_byte != NULL && datas != NULL)
	{
		(*callback_send_byte)(SOH);
		(*callback_send_byte)(HIGHINT(size));
		(*callback_send_byte)(LOWINT(size));
		checksum = HIGHINT(size) + LOWINT(size);
		for(i=0; i<size; i++)
		{
			(*callback_send_byte)(datas[i]);
			checksum += datas[i];
		}
		(*callback_send_byte)(LOWINT(checksum));
		(*callback_send_byte)(EOT);
	}
}


#endif
