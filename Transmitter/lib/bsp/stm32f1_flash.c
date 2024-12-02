/*
 * stm32f1_flash.c
 *
 *  Created on: 11 juil. 2019
 *      Author: Nirgal
 */


/*
 * Ce module logiciel se propose d'utiliser le dernier secteur de la mémoire FLASH pour enregistrer des variables non volatiles, utiles à votre application.
 *
 * Limitation de la version actuelle (qui se veut assez simplifiée) :
 * - écriture d'entiers sur 32 bits seulement
 * - 256 entiers possibles
 *
 * ATTENTION, ce module ne fonctionne (en l'état) que sur un STM32F103RBT6 (doté de 128KBytes !)
 * ATTENTION, Pour utiliser ce module logiciel, le fichier LinkerScript.ld doit limiter l'espace utilisé par le programme à 127K sur les 128K disponibles.
 *
 */
#include "stm32f1_flash.h"
#if USE_INTERNAL_FLASH_AS_EEPROM
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash_ex.h"
#include "macro_types.h"

#define BASE_ADDRESS	0x0801FC00		//adresse du début de la dernière page (2kBytes)
#define	SIZE_SECTOR		(1024/4)
#define PAGE_ADDRESS	127

static void FLASH_write_word(uint32_t index, uint32_t data);
static void FLASH_erase_everything_else(uint32_t index);
static void FLASH_erase(void);
extern void FLASH_PageErase(uint32_t PageAddress);

/*
 * @brief	Fonction de démo permettant de se familiariser avec les fonctions de ce module logiciel.
 * @brief	Cette fonction consulte et incrémente le nombre inscrit dans la première case. Puis, elle lance un affichage du contenu complet.
 * @pre	ATTENTION : ne pas appeler cette fonction en tâche de fond. Risque d'endommager la flash en cas d'écritures trop nombreuses.
 *
 */
void FLASH_demo(void)
{
	uint32_t current_word;
	current_word = FLASH_read_word(0);
	printf("mot présent dans la case 0 : %ld = 0x%lx\n", current_word, current_word);
	current_word++;
	FLASH_set_word(0, current_word);
	current_word = FLASH_read_word(0);
	printf("mot présent dans la case 0 après incrémentation : %ld = 0x%lx\n", current_word, current_word);
	FLASH_dump();
}




/*
 * @brief	Enregistre une donnée dans la case souhaitée, sans toucher aux autres cases
 * @param  	index : Numéro de la case (de 0 à 255).
 * @post  	ATTENTION : si la case est déjà occupée par une donnée différente de 0xFFFFFFFF (valeur par défaut après effacement), une sauvegarde complète du secteur est faite, puis un effacement, puis une restitution !
 * @post  	le temps d'exécution de cette fonction peut nettement varier !
 * @pre		//ATTENTION : ne pas appeler cette fonction trop fréquemment. Risque d'endommager la flash en cas d'écritures trop nombreuses. (>10000 sur le cycle de vie complet du produit)
 */
void FLASH_set_word(uint32_t index, uint32_t data)
{
	uint32_t current_word;
	assert(index < SIZE_SECTOR);
	current_word = FLASH_read_word(index);
	if((current_word & data) != data)	//il n'est pas possible d'écrire le mot sans être pollué par des zéros qui seraient déjà écrit ici
		FLASH_erase_everything_else(index);

	FLASH_write_word(index, data);
}

/*
 * @brief	Lit une donnée située dans la case souhaitée.
 * @param	index : Numéro de la case (de 0 à 255).
 */
uint32_t FLASH_read_word(uint32_t index)
{
	uint32_t res = 0;
	assert(index < SIZE_SECTOR);

	uint16_t * p;
	p = (uint16_t *)(BASE_ADDRESS + 4*index);
	res = (uint32_t)(*p);
	p++;
	res |= ((uint32_t)(*p))<<16;

	return res;
}

/*
 * @brief Cette fonction affiche les SIZE_SECTOR données (32 bits) disponibles dans le dernier secteur de la FLASH
 */
void FLASH_dump(void)
{
	uint32_t index;
	uint32_t v;
	printf("Affichage des %d données (32 bits) disponibles dans le dernier secteur de la FLASH\n", SIZE_SECTOR);
	for(index = 0; index<SIZE_SECTOR; index++)
	{
		v = FLASH_read_word(index);
		printf("@%03ld : 0x%08lx = %ld\n", index, v, v);
	}
}



//______________________________________________________________________/
//___Fonctions privées__________________________________________________/

static void FLASH_erase_everything_else(uint32_t index)
{
	uint32_t saved_values[SIZE_SECTOR];
	uint32_t i;
	assert(index < SIZE_SECTOR);
	for(i=0; i<SIZE_SECTOR; i++)
		saved_values[i] = FLASH_read_word(i);
	FLASH_erase();
	for(i=0; i<SIZE_SECTOR; i++)
	{
		if(i!=index && saved_values[i]!=0xFFFFFFFF)
			FLASH_write_word(i, saved_values[i]);	//on réécrit tout sauf le mot
	}
}

static void FLASH_erase(void)
{
	HAL_FLASH_Unlock();
	FLASH_PageErase(BASE_ADDRESS);
	 /* Wait for last operation to be completed */
	FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

	/* If the erase operation is completed, disable the PER Bit */
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	HAL_FLASH_Lock();
}


static void FLASH_write_word(uint32_t index, uint32_t data)
{
	assert(index < SIZE_SECTOR);
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, BASE_ADDRESS+4*index, (uint64_t)(data));
	HAL_FLASH_Lock();
}

/*
 * fonction qui tue une case de la flash à petit feu...
 * ne pas utiliser...
 * un test de cette fonction en juillet 2019 a révélé que la flash pouvait tenir 1,65 millions d'écritures avant de flancher.
 * attention à ne pas considérer ce test comme une preuve formelle... les fabriquants prennent des précautions nettement plus restrictives, et conseillent de ne pas dépasser 10 000 écritures sur le même emplacement.
 */
void FLASH_kill(void)
{
	uint32_t nb_cycles = 0;
	uint32_t read;
	while(1)
	{
		FLASH_erase();
		read = FLASH_read_word(0);
		if(read != 0xFFFFFFFF)
			printf("fail to erase - nb_cycles = %ld ! - read 0x%lx\n", nb_cycles, read);
		FLASH_write_word(0, 0x55555555);
		read = FLASH_read_word(0);
		if(read != 0x55555555)
			printf("fail to write - nb_cycles = %ld ! - read 0x%lx\n", nb_cycles, read);
		else
		{
			nb_cycles++;
			if(nb_cycles % 1000 == 0)
				printf("%ld\n",nb_cycles);
		}
	}

}

#endif
