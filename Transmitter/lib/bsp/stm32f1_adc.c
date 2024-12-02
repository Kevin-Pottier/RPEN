/*
 * stm32f1_adc.c
 *
 *  Created on: 4 mai 2016
 *      Author: Nirgal
 */


/**
 * demo_adc.c
 *
 *  Created on: 3 mars 2015
 *      Author: Samuel Poiraud
 */
#include "config.h"
#if USE_ADC
#include "stm32f1_adc.h"
#include "stm32f1xx_hal.h"
#include "stm32f1_gpio.h"
#include "stm32f1_timer.h"
#include "systick.h"
#define ADC_NB_OF_CHANNEL_USED	(USE_AN0 + USE_AN1 + USE_AN2 + USE_AN3 + USE_AN4 + USE_AN5 + USE_AN6 + USE_AN7 + USE_AN8 + USE_AN9 + USE_AN10 + USE_AN11 + USE_AN12 + USE_AN13 + USE_AN14 + USE_AN15 + USE_AN16 + USE_AN17)

#if ADC_NB_OF_CHANNEL_USED > 16
	#error	"16 entrées analogiques maximum peuvent être activées"
#endif
#if ADC_NB_OF_CHANNEL_USED < 2
	#error "Ce module logiciel stm32f1_adc ne fonctionne qu'à partir de 2 canaux activés !"
#endif

static volatile uint16_t t = 0;								//Chrono utilisé dans la machine a état. décrémenté toutes les millisecondes.
static uint32_t adc_converted_value[ADC_CHANNEL_NB];		//Ce tableau contient les valeurs mesurées par l'ADC pour chaque canal.
static int8_t adc_id[ADC_CHANNEL_NB];						//Ce tableau contient les rang de chaque canal dans le tableau adc_converted_value. Usage privé à ce fichier.
static ADC_HandleTypeDef	hadc;							//Handler pour l'ADC.
static DMA_HandleTypeDef	hdma;							//Handler pour le DMA.



/** @brief 	fonction devant être appelée chaque milliseconde si on utilise la DEMO_adc_statemachine
 * @func	void DEMO_adc_process_1ms(void)
 */
void DEMO_adc_process_1ms(void)
{
	if(t)
		t--;
}

/**
 * @brief 	Cette fonction est une machine a etat de démonstration qui présente une utilisation de ce module de gestion de l'ADC.
 * @func 	running_e DEMO_adc_state_machine(bool_e ask_for_finish)
 * @param 	exit_asked: demande a l'application de quitter ou non
 * @return	cette fonction retourne un element de l'enumeration running_e (END_OK= l'application est quittee avec succes ou IN_PROGRESS= l'application est toujours en cours)
 */
void DEMO_adc_statemachine (void)
{
	typedef enum
	{
		INIT = 0,
		DISPLAY
	}state_e;

	static state_e state = INIT;
	switch(state)
	{
		case INIT:
			ADC_init();
			Systick_add_callback_function(DEMO_adc_process_1ms);
			printf("Analog to Digital Converter\n");
			state = DISPLAY;
			break;
		case DISPLAY:{
			int16_t value;
			int16_t millivolt;
			uint8_t channel;
			if(!t)
			{
				t = 400;
				for(channel = 0; channel < ADC_CHANNEL_NB; channel++)
				{
					if(adc_id[channel] != -1)
					{
						value = ADC_getValue(channel);
						millivolt = (int16_t)((((int32_t)value)*3300)/4096);		//On la convertie en volts
						printf("Ch%d : %4d = %1d.%03dV | ",channel, value, millivolt/1000, millivolt%1000);
						//printf("Ch%d=%4d ",channel, value);
					}
				}
				printf("\n");
			}
			break;}

		default:
			break;
	}
}





/**
 * @brief	Cette fonction privée initialise en entrées analogiques les ports dont le USE_ANx est défini.
 * @func	void PORTS_adc_init(void)
 * @post	A l'issue de l'exécution de cette fonction, le tableau adc_id contient pour chaque canal son rang dans le tableau des valeurs mesurées (adc_converted_value)
 * @post	Chaque canal non utilisé comme analogique a un champ à -1 dans le tableau adc_id.
 */
static void PORTS_adc_init(void)
{
	uint8_t channel;
	ADC_ChannelConfTypeDef sConfig;

	// Initialisation du tableau des id des convertisseurs analogique numérique. Chaque canal non utilisé verra sa case à -1.
	for(channel=0;channel<ADC_CHANNEL_NB;channel++)
		adc_id[channel] = -1;

	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	sConfig.Rank = 0;

	#if	USE_AN0
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_0] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_0;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN1
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_1] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_1;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN2
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_2, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_2] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_2;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN3
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_3, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_3] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_3;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN4
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_4, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_4] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_4;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN5
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_5, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_5] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_5;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN6
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_6, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_6] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_6;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN7
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_7, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_7] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_7;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN8
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_8] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_8;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN9
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_9] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_9;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN10
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_10] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_10;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN11
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_11] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_11;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN12
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_2, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_12] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_12;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN13
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_3, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_13] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_13;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN14
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_4, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_14] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_14;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN15
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_5, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
		adc_id[ADC_15] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_15;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN16
		adc_id[ADC_16] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_16;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif
	#if	USE_AN17
		adc_id[ADC_17] = (int8_t)sConfig.Rank;
		sConfig.Rank++;
		sConfig.Channel = ADC_CHANNEL_17;
		HAL_ADC_ConfigChannel(&hadc,&sConfig);
	#endif

}


static void PORTS_adc_Deinit(void)
{
	#if	USE_AN0
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN1
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN2
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN3
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN4
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN5
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN6
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN7
		BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN8
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN9
		BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN10
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN11
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN12
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN13
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN14
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
	#if	USE_AN15
		BSP_GPIO_PinCfg(GPIOC, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
	#endif
}

/**
 * @brief	Cette fonction initialise le périphérique ADC1, ainsi que le DMA (direct memory access).
 * @func	void ADC_init(void)
 * @post	A l'issue de l'exécution de cette fonction, des conversions analogiques sont menées en permanence par l'ADC... et mettent à jour un tableau adc_converted_value que l'on peut consulter.
 */
void ADC_init(void)
{
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC_CONFIG(RCC_ADCPCLK2_DIV6);

	//Déclenchements de l'ADC par le TIMER3 (TRGO)
	TIM_HandleTypeDef * htim;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIMER_run_us(TIMER3_ID,1000, FALSE);			//1 mesure par ms. (choix arbitraire, c'est un exemple...vous pouvez changer cette période de mesure !)
	htim = TIMER_get_phandler(TIMER3_ID);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig);
	HAL_TIM_GenerateEvent(htim,TIM_EVENTSOURCE_TRIGGER);
	hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;

	// Initialisation de l'ADC1
	hadc.Instance = ADC1;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;					//Alignement a droite des donnees mesurees dans le resultat sur 16 bits.
	hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;							//Mode de conversion : ENABLE si plusieurs canaux. DISABLE si un seul canal.
	hadc.Init.ContinuousConvMode = DISABLE;						//Mode continu : ENABLE si mesures en continu. DISABLE si une seule mesure a effectuer.
	hadc.Init.NbrOfConversion = ADC_NB_OF_CHANNEL_USED;			//Nombre de conversions a effectuer dans un cycle de mesure. (minimum 1, maximum 18).
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.NbrOfDiscConversion = 0;

	HAL_ADC_Init(&hadc);										//Initialisation de l'ADCx avec la structure remplie

	// Initialisation des ports en analogique.
	PORTS_adc_init();

	//Configuration du DMA.
	__HAL_RCC_DMA1_CLK_ENABLE();
	hdma.Instance = DMA1_Channel1;
	hdma.XferCpltCallback = NULL;
	hdma.XferErrorCallback = NULL;
	hdma.XferHalfCpltCallback = NULL;
	hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;				//On indique au périphérique DMA qu'il doit copier des données d'un périphérique vers la mémoire.
	hdma.Init.PeriphInc = DMA_PINC_DISABLE;					//A chaque copie, l'adresse source des données n'est pas incrémentée (il se sert dans le même registre de l'ADC pour chaque nouvelle donnée)
	hdma.Init.MemInc = DMA_MINC_ENABLE;						//A chaque copie, l'adresse destination des données est  incrémentée (il range les données en mémoire dans un tableau)
	hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma.Init.Mode = DMA_CIRCULAR;
	hdma.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&hdma);

	// Association entre le handler du DMA et celui de l'ADC
	__HAL_LINKDMA(&hadc, DMA_Handle, hdma);

	//Configuration des interruptions.
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);				//Si vous souhaitez activer les interruption à chaque fin de remplissage DMA... décommentez cette ligne.

	//Lancement de l'ADC, avec usage du DMA.
	HAL_ADC_Start_DMA(&hadc,adc_converted_value,ADC_NB_OF_CHANNEL_USED);

}

void ADC_Deinit(void)
{
	Systick_remove_callback_function(DEMO_adc_process_1ms);
	HAL_ADC_Stop_DMA(&hadc);
	HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
	HAL_DMA_DeInit(&hdma);
	PORTS_adc_Deinit();
	HAL_ADC_DeInit(&hadc);
	__HAL_RCC_ADC1_CLK_DISABLE();
}


/**
* @brief  	Cette fonction permet de récupérer les valeurs mesurées par l'ADC.
* @func  	int16_t ADC_getValue(adc_id_e channel)
* @param	channel : un canal de ADC_0 à ADC_15
* @retval 	un entier signé sur 16 bits, correspondant à la valeur demandée, pouvant aller de 0 à 4095 (l'ADC mesure sur 12 bits).
* @retval	Cette fonction retourne -1 si l'on demande un canal ADC qui n'a pas été initialisé (le define USE_ADCx correspondant est commenté)
*/
int16_t ADC_getValue(adc_id_e channel)
{
	if(adc_id[channel] == -1 || channel >= ADC_CHANNEL_NB)
	{
		printf("Lecture de la valeur du canal analogique numérique %d : non utilisé ou non initialisé !\n", channel);
		return -1;
	}
	return (int16_t)adc_converted_value[adc_id[channel]];
}

static callback_fun_t callback_function;

void ADC_set_callback_function(callback_fun_t callback)
{
	callback_function = callback;
}

static volatile bool_e flag_new_sample_available = FALSE;

bool_e ADC_is_new_sample_available(void)
{
	bool_e ret = flag_new_sample_available;
	if(ret)
		flag_new_sample_available = FALSE;
	return ret;
}

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void DMA1_Channel1_IRQHandler(void)
{
	flag_new_sample_available = TRUE;
	HAL_DMA_IRQHandler(&hdma);
	if(callback_function)
		callback_function();
}

#endif
