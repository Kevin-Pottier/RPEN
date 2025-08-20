/**
 * demo_motor.c
 *
 *  Created on: 27 octobre 2015
 *      Author: S. Poiraud
 */

/*
 * Ce module logiciel est destiné au pilotage de moteur DC à travers un pont en H BD6221F-E2
 * 	(ou modèle similaire en terme de signaux de commande)
 *
 * Ce module utilise le pilote stm32f1_pwm.h
 *
 * Table de vérité des signaux de commande de ce pont en H BD6221 :
 * 	(lorsque l'entrée Vref est reliée à Vcc)
 *
 * 		Rin		Fin
 * 		0		0		moteur en roue libre
 * 		0		pwm		moteur en marche avant, 	vitesse proportionelle au rapport cyclique du signal pwm
 * 		pwm		0		moteur en marche arrière, 	vitesse proportionelle au rapport cyclique du signal pwm
 * 		1		1		moteur en court-circuit (freinage maximal)
 *
 * 		Pour un châssis mobile, et dans la majorité des fonctionnement, on utilise les 3 premières lignes de cette table de vérité.
 * 		Selon le signe de la variable "duty", ce module logiciel est donc conçu pour produire  :
 * 					-> une PWM vers une broche Fin, et un 0 logique vers la broche Rin
 * 			OU BIEN -> une PWM vers une broche Rin, et un 0 logique vers la broche Fin
 *
 *
 * Les ports utilisés par ce module logiciel sont imposés.
 * Il est possible de les changer en modifiant le contenu des fonctions MOTOR_init et MOTOR_set_duty.
 */

/* Pour le module TB6612F-NG
 * Table de vérité des signaux de commande de ce pont en H TB6612F-NG
 * 	PWM 	AIN1	AIN2
 * 	1		0		1		moteur en sens horaire inverse
 *  1		1		0		moteur en sens horaire
 *  1       0       0		moteur en roue libre
 *  0 		x		x		moteur en court-circuit (freinage maximal) !!
 *  x		1		1		moteur en court-circuit (freinage maximal) !!
 *
 * Il ne faut donc pas utiliser la broche nommée "PWM" pour y envoyer un signal PWM... sinon on alterne avec un état court-circuit.
 * Pour cela, pour une rotation en sens horaire : on envoie un signal PWM sur AIN1 en maintenant AIN2 à 0 et PWM à 1.
 *            pour une rotation anti-hoaire     : on envoie un signal PWM sur AIN2 en maintenant AIN1 à 0 et PWM à 1.
 *  Exemple fonctionnel de branchements pouvant servir de base :
 *  TB6612F-NG			STM32F103
 *  VM				--	5V ou toute tension "puissance" correspondant à la vitesse max du moteur
 *  VCC				--  3.3V
 *  GND				--	GND (toutes les pins GND du module TB6612F-NG sont reliées entre elles)
 * 	STBY			--	3.3V
 * 	PWMA			--  3.3V
 * 	AIN1			--	PB13 - par exemple... (pour sortir un signal PWM   ou bien   0)
 * 	AIN2			--	PA8  - par exemple... (pour sortir 0               ou bien   un signal PWM)
 * 	AO1 et AO2		--	Reliées aux broches du moteur
 */

/*
 *
 * Pour découvrir ce module logiciel, vous pouvez appeler en tâche de fond DEMO_MOTOR_statemachine(FALSE, UART_get_next_byte(UART2_ID));
 *
 * Les autres fonctions utiles sont :
 * 		MOTOR_add()
 * 		MOTOR_set_duty()
 */


#include "config.h"
#if USE_MOTOR_DC
#include "stm32f1_motorDC.h"
#include "stm32f1_pwm.h"
#include "stm32f1_gpio.h"
#include "stm32f1xx_hal.h"


#define	PWM_PERIOD	50		//Période du signal PWM, en microsecondes
							//[50us <=> 20kHz, fréquence humainement inaudible et électroniquement pas trop élevée]

typedef HAL_StatusTypeDef (*p_func_pwm)(TIM_HandleTypeDef * handler, uint32_t channel);	//Type pointeur sur fonction PWM

typedef struct
{
	GPIO_TypeDef * gpio;
	uint16_t pin;
	uint32_t tim_channel;
	bool_e negative;
	bool_e remap;
	p_func_pwm func_start;
	p_func_pwm func_stop;
	TIM_HandleTypeDef * handler;
	timer_id_e timer_id;
}pwm_channel_t;

typedef struct
{
	pwm_channel_t forward;
	pwm_channel_t reverse;
	bool_e enable;
}motor_t;


static motor_t motors[MOTOR_NB];
static running_e MOTOR_gpio_and_pin_to_pwm_channel(GPIO_TypeDef * gpio, uint16_t pin, pwm_channel_t * pwm_channel);


/**
 * @brief 	Cette fonction est une machine a etat qui présente un exemple d'utilisation de ce module.
 * @func 	running_e DEMO_MOTOR_statemachine (bool_e ask_for_finish, char touch_pressed)
 * @param 	ask_for_finish : demande que la machine a état se termine.
 * @param	touch_pressed : caractère entré par l'utilisateur. NULL si aucun caractère.
 * @return	cette fonction retourne un element de l'enumeration running_e (END_OK= l'application est quittee avec succes ou IN_PROGRESS= l'application est toujours en cours)
 * @example DEMO_MOTOR_statemachine(FALSE, UART_get_next_byte(UART2_ID));
 */
running_e DEMO_MOTOR_statemachine (bool_e ask_for_finish, char touch_pressed)
{
	typedef enum
	{
		INIT = 0,
		DISPLAY,
		ADJUST_PWM,
		CLOSE
	}state_e;
	running_e ret = IN_PROGRESS;
	static motor_id_e our_motors[4];

	static state_e state = INIT;
	switch(state)
	{
		case INIT:
			our_motors[0] = MOTOR_add(GPIOA, GPIO_PIN_8, GPIOB, GPIO_PIN_13);
			our_motors[1] = MOTOR_add(GPIOA, GPIO_PIN_9, GPIOB, GPIO_PIN_14);
			our_motors[2] = MOTOR_add(GPIOA, GPIO_PIN_10, GPIOB, GPIO_PIN_15);
			our_motors[3] = MOTOR_add(GPIOA, GPIO_PIN_11, GPIOB, GPIO_PIN_11);
			state = DISPLAY;
			break;
		case DISPLAY:{
			printf("Touch -> %% PWM\n");
			uint8_t i;
			for(i=0;i<=9;i++)
				printf("   %d  -> %3d%%\n", i, i*10);
			printf("   *  -> *-1 %%\n");
			printf("   +  -> n+1%%\n");
			printf("   -  -> n-1%%\n");
			for(i=0;i<4;i++)
				printf("   %c  -> Channel %d\n", 'A'+i, i+1);
			state = ADJUST_PWM;
			break;}
		case ADJUST_PWM:{
			static int8_t duty = 50;
			static motor_id_e motor = TIM_CHANNEL_1;
			bool_e duty_updated = FALSE;
			bool_e motor_updated = FALSE;
			int8_t sign;
			sign = (duty<0)?(-1):1;
			switch(touch_pressed)
			{
				case '+':	if(duty<100)			duty++;		duty_updated = TRUE;	break;
				case '-':	if(duty>-100)			duty--;		duty_updated = TRUE;	break;
				case '0':	duty=0; 				duty_updated = TRUE;	break;
				case '1':	duty=(int8_t)(10*sign); 	duty_updated = TRUE;	break;
				case '2':	duty=(int8_t)(20*sign); 	duty_updated = TRUE;	break;
				case '3':	duty=(int8_t)(30*sign); 	duty_updated = TRUE;	break;
				case '4':	duty=(int8_t)(40*sign); 	duty_updated = TRUE;	break;
				case '5':	duty=(int8_t)(50*sign); 	duty_updated = TRUE;	break;
				case '6':	duty=(int8_t)(60*sign); 	duty_updated = TRUE;	break;
				case '7':	duty=(int8_t)(70*sign); 	duty_updated = TRUE;	break;
				case '8':	duty=(int8_t)(80*sign); 	duty_updated = TRUE;	break;
				case '9':	duty=(int8_t)(90*sign); 	duty_updated = TRUE;	break;
				case '*':	duty=(int8_t)(-duty); 	duty_updated = TRUE;	break;
				case 'a':	case 'A':	motor = our_motors[0];	motor_updated = TRUE;	break;
				case 'b':	case 'B':	motor = our_motors[1];	motor_updated = TRUE;	break;
				case 'c':	case 'C':	motor = our_motors[2];	motor_updated = TRUE;	break;
				case 'd':	case 'D':	motor = our_motors[3];	motor_updated = TRUE;	break;
				default:												break;
			}

			if(motor_updated)
			{
				printf("Motor %d\n",motor+1);
			}
			if(duty_updated)
			{
				MOTOR_set_duty(motor, (int16_t)duty); // On applique ce pourcentage au PWM
				printf("Motor %d%c -> %3d%%\n", motor+1, (duty<0)?'N':' ', duty);
			}

			if(ask_for_finish)
				state = CLOSE;
			break;}
		case CLOSE:

			state = INIT;
			ret = END_OK;
			break;
		default:
			break;
	}

	return ret;
}


/*
 * @brief	Cette fonction doit être appelée pour chaque moteur.
 * @param : on doit founir pour chaque moteur le GPIO et la PIN pour les commandes forward et reverse.
 * @ret : 	cette fonction retourne MOTOR_ID_NONE si les broches fournies ne sont pas acceptable.
 * 			sinon, cette fonction retourne l'ID attribué à ce moteur. Conservez cet id pour l'utiliser ultérieurement (notamment pour mettre à jour la PWM !)
 */
motor_id_e MOTOR_add(GPIO_TypeDef * gpio_forward, uint16_t pin_forward, GPIO_TypeDef * gpio_reverse, uint16_t pin_reverse)
{
	motor_id_e ret;
	ret = MOTOR_ID_NONE;

	for(motor_id_e id = 0; id<MOTOR_NB; id++)
	{
		if(motors[id].enable == FALSE)	//si on a trouvé une case vide dans le tableau des motors... on la choisit !
		{
			debug_printf("Ajout du moteur %d\n", id);
			ret = id;	//pour renvoyer l'id choisi.
			if(	MOTOR_gpio_and_pin_to_pwm_channel(gpio_forward, pin_forward, &motors[id].forward) == END_OK
					&&
				MOTOR_gpio_and_pin_to_pwm_channel(gpio_reverse, pin_reverse, &motors[id].reverse) == END_OK)
			{
				motors[id].enable = TRUE;
				PWM_run(motors[id].forward.timer_id, motors[id].forward.tim_channel,motors[id].forward.negative,PWM_PERIOD,0, motors[id].forward.remap);
				PWM_run(motors[id].reverse.timer_id, motors[id].reverse.tim_channel,motors[id].reverse.negative,PWM_PERIOD,0, motors[id].reverse.remap);
				debug_printf("\tforward - sur le timer %d - channel %ld%s%s\n",  motors[id].forward.timer_id+1, motors[id].forward.tim_channel/4+1, (motors[id].forward.negative)?" negative":"", (motors[id].forward.remap)?" (remap)":"");
				debug_printf("\treverse - sur le timer %d - channel %ld%s%s\n",  motors[id].reverse.timer_id+1, motors[id].reverse.tim_channel/4+1, (motors[id].reverse.negative)?" negative":"", (motors[id].reverse.remap)?" (remap)":"");
			}
			else
			{
				debug_printf("échec d'initialisation des PWM liées au moteur demandé\n");
			}
			break;
		}
	}

	return ret;
}


static running_e MOTOR_gpio_and_pin_to_pwm_channel(GPIO_TypeDef * gpio, uint16_t pin, pwm_channel_t * pwm_channel)
{
	running_e ret;
	ret = END_ERROR;

	pwm_channel_t local_pwm_channel;
	local_pwm_channel.timer_id = TIMER_ID_NB;
	local_pwm_channel.gpio = gpio;
	local_pwm_channel.pin = pin;
	local_pwm_channel.negative = FALSE;
	local_pwm_channel.remap = FALSE;

	if(gpio == GPIOA)
	{
		switch(pin)
		{
			case GPIO_PIN_1:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;		break;
			case GPIO_PIN_2:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;		break;
			case GPIO_PIN_3:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_4;		break;
			case GPIO_PIN_6:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;		break;
			case GPIO_PIN_7:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;		break;
			case GPIO_PIN_8:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;		break;
			case GPIO_PIN_9:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;		break;
			case GPIO_PIN_10:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;		break;
			case GPIO_PIN_11:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_4;		break;
			case GPIO_PIN_15:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;		break;
			default:
				break;
		}
	}
	else if(gpio == GPIOB)
	{
		switch(pin)
		{
			case GPIO_PIN_0:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;										break;
			case GPIO_PIN_1:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_4;										break;
			case GPIO_PIN_3:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_4:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_5:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_6:	local_pwm_channel.timer_id = TIMER4_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;										break;
			case GPIO_PIN_7:	local_pwm_channel.timer_id = TIMER4_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;										break;
			case GPIO_PIN_8:	local_pwm_channel.timer_id = TIMER4_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;										break;
			case GPIO_PIN_9:	local_pwm_channel.timer_id = TIMER4_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_4;										break;
			case GPIO_PIN_10:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_11:	local_pwm_channel.timer_id = TIMER2_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_4;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_13:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;	local_pwm_channel.negative = TRUE;	break;
			case GPIO_PIN_14:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;	local_pwm_channel.negative = TRUE;	break;
			case GPIO_PIN_15:	local_pwm_channel.timer_id = TIMER1_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;	local_pwm_channel.negative = TRUE;	break;
			default:
				break;
		}
	}
#if NUCLEO
	else if(gpio == GPIOC)
	{
		switch(pin)
		{
			case GPIO_PIN_6:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_1;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_7:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_2;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_8:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_3;	local_pwm_channel.remap = TRUE;		break;
			case GPIO_PIN_9:	local_pwm_channel.timer_id = TIMER3_ID;	local_pwm_channel.tim_channel = TIM_CHANNEL_4;	local_pwm_channel.remap = TRUE;		break;
			default:
				break;
		}
	}
#endif

	if(local_pwm_channel.timer_id != TIMER_ID_NB)	//on a trouvé !
	{
		local_pwm_channel.handler = TIMER_get_phandler(local_pwm_channel.timer_id);
		if(local_pwm_channel.negative)
		{
			local_pwm_channel.func_start = &HAL_TIMEx_PWMN_Start;
			local_pwm_channel.func_stop = &HAL_TIMEx_PWMN_Stop;
		}
		else
		{
			local_pwm_channel.func_start = &HAL_TIM_PWM_Start;
			local_pwm_channel.func_stop = &HAL_TIM_PWM_Stop;
		}

		*pwm_channel = local_pwm_channel;
		ret = END_OK;
	}
	else
	{
		debug_printf("échec d'ajout du moteur. Avez vous renseigné une broche reliée à un timer ?!\n");
	}

	return ret;
}


/*
 * @param id : indiquer l'id du moteur à piloter.
 * @param duty : indiquer un rapport cyclique. Ce rapport peut être négatif pour un sens de rotation inversé !
 * @pre : le moteur indiqué doit avoir été ajouté préalablement !
 * @post : la PWM sera mise à jour en conséquence. Attention, l'autre broche de ce moteur (reverse si duty>0 ; forward si duty <0) sera mise à 0 !
 */
void MOTOR_set_duty(motor_id_e id, int16_t duty)
{
	//Ecretage... Le rapport cyclique est exprimé dans la même unité que la PWM_PERIOD, il ne peut donc pas être plus grand !
	if(duty > 100)
		duty = 100;
	else if(duty < -100)
		duty = -100;


	if(motors[id].enable == FALSE)
	{
		debug_printf("Vous cherchez à piloter un moteur qui n'a pas été ajouté !\n");
		return;
	}


	/*
	 * Pour chaque moteur, on dispose de deux signaux PWM.
	 * Selon le sens demandé, démarre et on arrête les broches correspondantes.
	 */
	if(duty < 0)
	{
		duty = (int16_t)(-duty);
		motors[id].forward.func_stop(motors[id].forward.handler, motors[id].forward.tim_channel);
		PWM_set_duty(motors[id].reverse.timer_id, motors[id].reverse.tim_channel, (uint32_t)duty);
		motors[id].reverse.func_start(motors[id].reverse.handler, motors[id].reverse.tim_channel);
	}
	else
	{
		motors[id].reverse.func_stop(motors[id].reverse.handler, motors[id].reverse.tim_channel);
		PWM_set_duty(motors[id].forward.timer_id, motors[id].forward.tim_channel, (uint32_t)duty);
		motors[id].forward.func_start(motors[id].forward.handler, motors[id].forward.tim_channel);
	}
}



#endif
