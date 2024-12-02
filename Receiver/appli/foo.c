/*
 * foo.c - ce fichier contient des choses absolument inutiles... sauf à des fins purement pédagogiques.
 *
 *  Created on: 26 août 2015
 *      Author: S. Poiraud
 */
#include "macro_types.h"
#include "stdint.h"

void Fonction_montrant_l_utilisation_de_la_memoire (void)
{
	typedef struct
	{
		char * prenom;				//4 Un pointeur vers une chaine placée ailleurs
		char nom[10];				//10 Un tableau de caractère pour une chaine stockée ici
		uint8_t age;				//1 Un âge de 0 à 255
		int32_t compte_en_banque;	//4 Un nombre allant d'un très gros découvert à une très grande richesse
	}bank_user_t;

	static bank_user_t user1 = {"marcel", {'P','R','O','U','S','T','\0'}, 144, 1000000};
	static bank_user_t user2 = {"bill", {'G','4','T','3','$','\0'}, 144, 792000000};
	static bank_user_t user3 = {"étrange", {'0',0,0x00,1,'\0',3.14}, -10, -3000000000};		//vous remarquerez que cette ligne provoque un warning à la compilation, c'est pas pour du beurre !
	volatile bank_user_t * puser1, * puser2, * puser3;
	puser1 = &user1;
	puser2 = &user2;
	puser3 = &user3;

	//Observez ces structures en mémoires :
		//1- via l'onglet "expression"
		//2- via l'onglet "variables"
		//3- via l'onglet Memory
}
