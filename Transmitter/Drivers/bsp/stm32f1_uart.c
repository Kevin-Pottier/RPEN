/**
 * Uart.c
 *
 *
 *  Created on: 9 sept. 2013
 *  Modified : 9 mars 2016 -> add interrupt support & comments.
 *      Author: tbouvier, spoiraud
 *
 */

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include <string.h>

/*
 * Ce module logiciel permet l'utilisation des périphériques USART (Universal Synchronous/Asynchronous Receiver Transmitter)
 *
 * 	Un module UART permet d'envoyer des données (i.e. des octets) sur une "liaison série", c'est à dire sur un fil.
 * 	Les octets ainsi envoyés sont découpés en bits. Chaque bit est envoyé pendant une période fixée.
 *
 * 	Selon l'UART choisi, les broches correspondantes sont initialisées et réservé pour cet usage :
 * 		(des 'defines' permettent pour chaque USART de choisir de remapper ou non les ports Rx et Tx)
 * 	USART1 : Rx=PA10 et Tx=PA9 		ou avec remap : Rx=PB7 et Tx=PB6
 * 	USART2 : Rx=PA3 et Tx=PA2 		ou avec remap : Rx=PD6 et Tx=PD5
 * 	USART3 : Rx=PB11 et Tx=PB10 	ou avec remap : Rx=PD9 et Tx=PD8
 *
 * 	On parle de liaison série asynchrone lorsqu'aucune horloge n'accompagne la donnée pour indiquer à celui qui la reçoit l'instant où le bit est transmis.
 * 	Dans ces conditions, il faut impérativement que le récepteur sâche à quelle vitesse précise les données sont transmises.
 * 	Il "prend alors en photo" chaque bit, et reconstitue les octets.
 *
 * 	Au repos, la tension de la liaison série est à l'état logique '1'.
 * 	Pour chaque octets (8 bits) à envoyer, l'UART envoie en fait 10 bits :
 * 		1 bit de start (toujours à 0), 8 bits de données, 1 bit de stop (toujours à 1).
 * 	Ce passage par 0 avant l'envoi des données permet au récepteur de comprendre que l'on va transmettre un octet.
 * 		(sinon il ne saurait détecter le début d'un octet commencant par le bit '1' !)
 *
 * 	Voici un exemple d'utilisation de ce module logiciel.
 *
 * 	si on souhaite initialiser l'UART1, à une vitesse de 115200 bits par seconde, puis envoyer et recevoir des données sur cette liaison.
 *  Un exemple est également disponible dans la fonction UART_test()
 *
 * 	1-> Appeler la fonction UART_init(UART1_ID, 115200);
 * 	2-> Pour envoyer un octet 'A' sur l'UART1 : UART_putc(UART1_ID, 'A');
 * 	3-> Pour recevoir les octets qui auraient été reçus par l'UART1 :
 * 			if(UART_data_ready(UART1_ID))
 * 			{
 * 				uint8_t c;
 * 				c = UART_get_next_byte(UART1_ID);
 * 				//On peut faire ce qu'on souhaite avec l'octet c récupéré.
 * 			}
 * 		Les octets reçus par les périphériques UART initialisés sont traités dans une routine d'interruption, puis mémorisés dans un tableau (que l'on nomme un buffer),
 * 		jusqu'à l'appel à la fonction UART_get_next_byte().
 * 		Ce tableau peut mémoriser 128 octets. (BUFFER_RX_SIZE)
 * 		Cette méthode permet au processeur de ne pas louper des données arrivant sur ce périphérique pendant qu'il est occupé à autre chose dans le programme.
 * 		Il est simplement interrompu très brièvement pour conserver l'octet reçu, et remettre à plus tard son traitement.
 *
 * 	4-> Il est également possible de profiter de la richesse proposée par la fonction printf...
 * 		qui permet d'envoyer un texte 'variable', constitué avec une chaine de format et des paramètres.
 * 			Pour cela :
 * 			Appelez au moins une fois, lors de l'initialisation, la fonction
 * 				SYS_set_std_usart(UART1_ID, UART1_ID, UART1_ID);   //indique qu'il faut utiliser l'UART1 pour sortir les données du printf.
 * 			Puis :
 * 				uint32_t millivolt = 3245;	//une façon éléguante d'exprimer le nombre 3,245 Volts
 * 				printf("Bonjour le monde, voici un joli nombre à virgule : %d,%03d V\n", millivolt/1000, millivolt%1000);
 *
 */

//Les buffers de réception accumulent les données reçues, dans la limite de leur taille.
//Les emplacement occupés par les octets reçus sont libérés dès qu'on les consulte.
#define BUFFER_RX_SIZE	128

static UART_HandleTypeDef UART_HandleStructure[UART_ID_NB];	//Ce tableau contient les structures qui sont utilisées pour piloter chaque UART avec la librairie HAL.
static const USART_TypeDef * instance_array[UART_ID_NB] = {USART1, USART2, USART3};
static const IRQn_Type nvic_irq_array[UART_ID_NB] = {USART1_IRQn, USART2_IRQn, USART3_IRQn};

//Buffers
static uint8_t buffer_rx[UART_ID_NB][BUFFER_RX_SIZE];
static uint8_t buffer_rx_write_index[UART_ID_NB] = {0};
static uint32_t buffer_rx_read_index[UART_ID_NB] = {0};
static volatile bool_e buffer_rx_data_ready[UART_ID_NB] = {FALSE};
static volatile bool_e uart_initialized[UART_ID_NB] = {FALSE};


/*
 * Cette fonction blocante a pour but de vous aider à appréhender les fonctionnalités de ce module logiciel.
 * Complètement inutile, cette fonction accumule les octets reçus dans un tableau, puis les renvoie sur l'UART dès qu'un caractère '\n' est reçu.
 */
void UART_demo(void)
{
	#define DEMO_TAB_SIZE 128

	static uint8_t tab[DEMO_TAB_SIZE];
	static uint16_t index = 0;
	uint8_t c;
	while(1)
	{
		if(UART_data_ready(UART2_ID))
		{
			c = UART_getc(UART2_ID);			//lecture du prochain caractère
			tab[index] = c;						//On mémorise le caractère dans le tableau
			if(c=='\n')							//Si c'est la fin de la chaine
			{
				tab[index+1] = 0; 				//fin de chaine, en écrasant le caractère suivant par un 0
				UART_puts(UART2_ID, tab, 0);	//on renvoie la chaine reçue.
				index = 0;						//Remise à zéro de l'index
			}
			else if(index < DEMO_TAB_SIZE - 2)
			{									//Pour tout caractère différent de \n
				index++;						//on incrémente l'index (si < TAB_SIZE -2 !)
			}
		}
	}
}


/**
 * @brief	Initialise l'USARTx - 8N1 - vitesse des bits (baudrate) indiqué en paramètre
 * @func	void UART_init(uint8_t uart_id, uart_interrupt_mode_e mode)
 * @param	uart_id est le numéro de l'UART concerné :
 * 				UART1_ID
 * 				UART2_ID
 * 				UART3_ID
 * @param	baudrate indique la vitesse en baud/sec
 * 				115200	vitesse proposée par défaut
 * 				9600	vitesse couramment utilisée
 * 				19200	...
 * @post	Cette fonction initialise les broches suivante selon l'USART choisit en parametre :
 * 				USART1 : Rx=PA10 et Tx=PA9 		ou avec remap : Rx=PB7 et Tx=PB6
 * 				USART2 : Rx=PA3 et Tx=PA2 		ou avec remap : Rx=PD6 et Tx=PD5
 * 				USART3 : Rx=PB11 et Tx=PB10 	ou avec remap : Rx=PD9 et Tx=PD8
 * 				La gestion des envois et reception se fait en interruption.
 *
 */
void UART_init(uart_id_e uart_id, uint32_t baudrate)
{
	assert(baudrate > 1000);
	assert(uart_id < UART_ID_NB);

	buffer_rx_read_index[uart_id] = 0;
	buffer_rx_write_index[uart_id] = 0;
	buffer_rx_data_ready[uart_id] = FALSE;
	/* USARTx configured as follow:
		- Word Length = 8 Bits
		- One Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
		- OverSampling: enable
	*/
	UART_HandleStructure[uart_id].Instance = (USART_TypeDef*)instance_array[uart_id];
	UART_HandleStructure[uart_id].Init.BaudRate = baudrate;
	UART_HandleStructure[uart_id].Init.WordLength = UART_WORDLENGTH_8B;//
	UART_HandleStructure[uart_id].Init.StopBits = UART_STOPBITS_1;//
	UART_HandleStructure[uart_id].Init.Parity = UART_PARITY_NONE;//
	UART_HandleStructure[uart_id].Init.HwFlowCtl = UART_HWCONTROL_NONE;//
	UART_HandleStructure[uart_id].Init.Mode = UART_MODE_TX_RX;//
	UART_HandleStructure[uart_id].Init.OverSampling = UART_OVERSAMPLING_16;//

	/*On applique les parametres d'initialisation ci-dessus */
	HAL_UART_Init(&UART_HandleStructure[uart_id]);
	
	/*Activation de l'UART */
	__HAL_UART_ENABLE(&UART_HandleStructure[uart_id]);

	// On fixe les priorités des interruptions de l'usart PreemptionPriority = 0, SubPriority = 1 et on autorise les interruptions
	HAL_NVIC_SetPriority(nvic_irq_array[uart_id] , 1, 1);
	HAL_NVIC_EnableIRQ(nvic_irq_array[uart_id]);
	HAL_UART_Receive_IT(&UART_HandleStructure[uart_id],&buffer_rx[uart_id][buffer_rx_write_index[uart_id]],1);	//Activation de la réception d'un caractère

	//Config LibC: no buffering
	setvbuf(stdout, NULL, _IONBF, 0 );
	setvbuf(stderr, NULL, _IONBF, 0 );
	setvbuf(stdin, NULL, _IONBF, 0 );

	uart_initialized[uart_id] = TRUE;
}


/*
 * @brief	Déinitialise l'USARTx
 * @param	uart_id est le numéro de l'UART concerné :	UART1_ID, UART2_ID, UART3_ID
 */
void UART_DeInit(uart_id_e uart_id)
{
	assert(uart_id < UART_ID_NB);
	HAL_UART_DeInit(&UART_HandleStructure[uart_id]);
	uart_initialized[uart_id] = FALSE;
}

/*
 * @brief	Fonction permettant de savoir si le buffer de l'UART demandé est vide ou non.
 * @ret		Retourne VRAI si un ou des caractères sont disponibles dans le buffer.
 * @ret		Retourne FAUX si aucun caractère n'est disponible dans le buffer (le buffer est vide)
 * @param	uart_id est le numéro de l'UART concerné :	UART1_ID, UART2_ID, UART3_ID
 */
bool_e UART_data_ready(uart_id_e uart_id)
{
	assert(uart_id < UART_ID_NB);
	return buffer_rx_data_ready[uart_id];
}

/*
 * @brief	Fonction permettant de récupérer le prochain caractère reçu dans le buffer.
 * @ret 	Retourne le prochain caractère reçu. Ou 0 si rien n'a été reçu.
 * @post 	Le caractère renvoyé par cette fonction ne sera plus renvoyé.
 */
uint8_t UART_get_next_byte(uart_id_e uart_id)
{
	uint8_t ret;
	assert(uart_id < UART_ID_NB);

	if(!buffer_rx_data_ready[uart_id])	//N'est jamais sensé se produire si l'utilisateur vérifie que UART_data_ready() avant d'appeler UART_get_next_byte()
		return 0;

	ret =  buffer_rx[uart_id][buffer_rx_read_index[uart_id]];
	buffer_rx_read_index[uart_id] = (buffer_rx_read_index[uart_id] + 1) % BUFFER_RX_SIZE;

	//Section critique durant laquelle on désactive les interruptions... pour éviter une mauvaise préemption.
	NVIC_DisableIRQ(nvic_irq_array[uart_id]);
	if (buffer_rx_write_index[uart_id] == buffer_rx_read_index[uart_id])
		buffer_rx_data_ready[uart_id] = FALSE;
	NVIC_EnableIRQ(nvic_irq_array[uart_id]);
	return ret;
}


/**
 * @func 	char UART_getc(uart_id_e uart_id))
 * @brief	Fonction NON blocante qui retourne le dernier caractere reçu sur l'USARTx. Ou 0 si pas de caractere reçu.
 * @param	UART_Handle : UART_Handle.Instance = USART1, USART2 ou USART6
 * @post	Si le caractere reçu est 0, il n'est pas possible de faire la difference avec le cas où aucun caractere n'est reçu.
 * @ret		Le caractere reçu, sur 8 bits.
 */
uint8_t UART_getc(uart_id_e uart_id)
{
	return UART_get_next_byte(uart_id);
}

/**
 * @func 	char UART_getc_blocking(uart_id_e uart_id, uint32_t timeout))
 * @brief	Fonction blocante !! qui retourne le dernier caractere reçu sur l'USARTx. Ou 0 si pas de caractere reçu au delà du timeout
 * @param	UART_Handle : UART_Handle.Instance = USART1, USART2 ou USART6
 * @param	timeout au delà duquel on abandonne le blocage, sauf si timeout vaut 0 (attente infinie)
 * @post	Si le caractere reçu est 0, il n'est pas possible de faire la difference avec le cas où aucun caractere n'est reçu.
 * @ret		Le caractere reçu, sur 8 bits.
 */
uint8_t UART_getc_blocking(uart_id_e uart_id, uint32_t timeout)
{
	uint32_t initial = HAL_GetTick();
	uint8_t c = 0;
	do
	{
		if(UART_data_ready(uart_id))
		{
			c = UART_get_next_byte(uart_id);
			break;
		}
	}while(timeout==0 || HAL_GetTick() - initial < timeout);
	return c;
}


/*
 * @func
 * @brief	Lit "len" caractères reçus, s'ils existent...
 * @post	Fonction non blocante : s'il n'y a plus de caractère reçu, cette fonction renvoit la main
 * @ret		Le nombre de caractères lus.
 */
uint32_t UART_gets(uart_id_e uart_id, uint8_t * datas, uint32_t len)
{
	uint32_t i;
	for(i=0; i<len ; i++)
	{
		if(UART_data_ready(uart_id))
			datas[i] = UART_get_next_byte(uart_id);
		else
			break;
	}
	return i;
}

/**
 * @func 	uint32_t UART_gets_blocking(uart_id_e uart_id, uint8_t * datas, uint32_t len, uint32_t timeout)
 * @brief	Fonction blocante !! qui retourne "len" caracteres reçus sur l'USARTx
 * @param	les caractères reçus sont rangés dans le buffer datas.
 * @param	UART_Handle : UART_Handle.Instance = USART1, USART2 ou USART6
 * @param	timeout au delà duquel on abandonne le blocage, sauf si timeout vaut 0 (attente infinie)
 * @ret		Le nombre de caracteres reçus
 */
uint32_t UART_gets_blocking(uart_id_e uart_id, uint8_t * datas, uint32_t len, uint32_t timeout)
{
	uint32_t i;
	uint32_t initial = HAL_GetTick();
	for(i=0; i<len ; i++)
	{
		if(UART_data_ready(uart_id))
			datas[i] = UART_get_next_byte(uart_id);
		else
		{
			while(!UART_data_ready(uart_id) || timeout==0 || (HAL_GetTick() - initial < timeout));
		}
	}
	return i;
}

/**
 * @brief	Envoi un caractere sur l'USARTx. Fonction BLOCANTE si un caractere est deja en cours d'envoi.
 * @func 	void UART_putc(UART_HandleTypeDef * UART_Handle, char c)
 * @param	c : le caractere a envoyer
 * @param	USARTx : USART1, USART2 ou USART6
 */
void UART_putc(uart_id_e uart_id, uint8_t c)
{
	HAL_StatusTypeDef state;
	assert(uart_id < UART_ID_NB);
	if(uart_initialized[uart_id])
	{
		do
		{
			NVIC_DisableIRQ(nvic_irq_array[uart_id]);
			state = HAL_UART_Transmit_IT(&UART_HandleStructure[uart_id], &c, 1);
			NVIC_EnableIRQ(nvic_irq_array[uart_id]);
		}while(state == HAL_BUSY);
	}
}

/**
 * @brief	Envoi une chaine de caractere sur l'USARTx. Fonction BLOCANTE si un caractere est deja en cours d'envoi.
 * @func 	void UART_putc(UART_HandleTypeDef * UART_Handle, char c)
 * @param	str : la chaine de caractère à envoyer
 * @param	USARTx : USART1, USART2 ou USART6
 */
void UART_puts(uart_id_e uart_id, uint8_t * str, uint32_t len)
{
	HAL_StatusTypeDef state;
	HAL_UART_StateTypeDef uart_state;
	assert(uart_id < UART_ID_NB);
	if(uart_initialized[uart_id])
	{
		if(!len)
			len = strlen((char*)str);
		do
		{
			NVIC_DisableIRQ(nvic_irq_array[uart_id]);
			state = HAL_UART_Transmit_IT(&UART_HandleStructure[uart_id], str, (uint16_t)len);
			NVIC_EnableIRQ(nvic_irq_array[uart_id]);
		}while(state == HAL_BUSY);

		do{
			uart_state = HAL_UART_GetState(&UART_HandleStructure[uart_id]);
		}while(uart_state == HAL_UART_STATE_BUSY_TX || uart_state == HAL_UART_STATE_BUSY_TX_RX);	//Blocant.
	}
}

//ecriture impolie forcée bloquante sur l'UART (à utiliser en IT, en cas d'extrême recours)
void UART_impolite_force_puts_on_uart(uart_id_e uart_id, uint8_t * str, uint32_t len)
{
	uint32_t i;
	if(uart_initialized[uart_id])
	{
		USART_TypeDef * pusart;
		pusart = UART_HandleStructure[uart_id].Instance;
		for(i=0; i<len; i++)
		{
			while(!(pusart->SR & USART_FLAG_TXE));
			pusart->DR = str[i];
		}
	}
}
/*
 * @brief Fonction blocante qui présente un exemple d'utilisation de ce module logiciel.
 */
void UART_test(void)
{
	UART_init(UART1_ID,115200);
	UART_init(UART2_ID,115200);
	UART_init(UART3_ID,115200);
	uint8_t c;
	while(1)
	{
		if(UART_data_ready(UART1_ID))
		{
			c = UART_get_next_byte(UART1_ID);
			UART_putc(UART1_ID,c);					//Echo du caractère reçu sur l'UART 1.
		}

		if(UART_data_ready(UART2_ID))
		{
			c = UART_get_next_byte(UART2_ID);
			UART_putc(UART2_ID,c);					//Echo du caractère reçu sur l'UART 2.
		}

		if(UART_data_ready(UART3_ID))
		{
			c = UART_get_next_byte(UART3_ID);
			UART_putc(UART3_ID,c);					//Echo du caractère reçu sur l'UART 3.
		}
	}
}

/////////////////  ROUTINES D'INTERRUPTION  //////////////////////////////

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART_HandleStructure[UART1_ID]);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART_HandleStructure[UART2_ID]);
}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART_HandleStructure[UART3_ID]);
}



/*
 * @brief	Cette fonction est appelée en interruption UART par le module HAL_UART.
 * @post	L'octet reçu est rangé dans le buffer correspondant.
 * @post	La réception en IT du prochain octet est ré-activée.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t uart_id;
	if(huart->Instance == USART1)		uart_id = UART1_ID;
	else if(huart->Instance == USART2)	uart_id = UART2_ID;
	else if(huart->Instance == USART3)	uart_id = UART3_ID;
	else return;

	buffer_rx_data_ready[uart_id] = TRUE;	//Le buffer n'est pas (ou plus) vide.
	buffer_rx_write_index[uart_id] = (uint8_t)((buffer_rx_write_index[uart_id] + 1) % BUFFER_RX_SIZE);						//Déplacement pointeur en écriture
	HAL_UART_Receive_IT(&UART_HandleStructure[uart_id],&buffer_rx[uart_id][buffer_rx_write_index[uart_id]],1);	//Réactivation de la réception d'un caractère
}

/*
 * @brief	Cette fonction est appelée par la fonction d'initialisation HAL_UART_Init().
 * 			Selon le numéro de l'UART, on y defini la configuration des broches correspondantes (voir la doc)
 * @param	huart: uart handler utilisé
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

	if(huart->Instance == USART1)
	{
		#ifdef UART1_ON_PA9_PA10
			__HAL_RCC_GPIOA_CLK_ENABLE();		//Horloge des broches a utiliser
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Tx as AF
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_10, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Rx as AF
		#endif
		#ifdef UART1_ON_PB6_PB7
			//Remap :
			__HAL_RCC_AFIO_CLK_ENABLE();
			__HAL_RCC_GPIOB_CLK_ENABLE();		//Horloge des broches a utiliser
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Tx as AF
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_7, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Rx as AF
			__HAL_AFIO_REMAP_USART1_ENABLE();
		#endif
		__HAL_RCC_USART1_CLK_ENABLE();		//Horloge du peripherique UART
	}
	else if(huart->Instance == USART2)
	{
		#ifdef UART2_ON_PA2_PA3
			__HAL_RCC_GPIOA_CLK_ENABLE();		//Horloge des broches a utiliser
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);	//Configure Tx as AF
			BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_3, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Rx as AF
		#endif
		#ifdef UART2_ON_PD5_PD6
			__HAL_RCC_AFIO_CLK_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			BSP_GPIO_PinCfg(GPIOD, GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);	//Configure Tx as AF
			BSP_GPIO_PinCfg(GPIOD, GPIO_PIN_6, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Rx as AF
			__HAL_AFIO_REMAP_USART2_ENABLE()
		#endif
		__HAL_RCC_USART2_CLK_ENABLE();		//Horloge du peripherique UART
	}
	else if(huart->Instance == USART3)
	{
		#ifdef  UART3_ON_PB10_PB11
			__HAL_RCC_GPIOB_CLK_ENABLE();		//Horloge des broches a utiliser
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);	//Configure Tx as AF
			BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_11, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Rx as AF
		#endif
		#ifdef UART3_ON_PD8_PD9
			//Remap...
			__HAL_RCC_AFIO_CLK_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();		//Horloge des broches a utiliser
			BSP_GPIO_PinCfg(GPIOD, GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);	//Configure Tx as AF
			BSP_GPIO_PinCfg(GPIOD, GPIO_PIN_9, GPIO_MODE_AF_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH); //Configure Rx as AF
			__HAL_AFIO_REMAP_USART3_ENABLE();
		#endif
		__HAL_RCC_USART3_CLK_ENABLE();		//Horloge du peripherique UART
	}
}

/*
 * @brief	Function called when the uart throws an error
 * @param	huart handler used to throw errors
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->ErrorCode == HAL_UART_ERROR_ORE){
        // remove the error condition
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        // set the correct state, so that the UART_RX_IT works correctly
        huart->RxState = HAL_UART_STATE_BUSY_RX;
    }

}




