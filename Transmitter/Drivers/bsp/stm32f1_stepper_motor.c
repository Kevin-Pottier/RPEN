#include "config.h"
#include "macro_types.h"
#include "stm32f1xx.h"
#include "stm32f1_timer.h"

//Cette brique logiciel est dans un état d'avancement relativement jeune.
//Ces fonctionnalités méritent d'être enrichies.

#if USE_STEPPER_MOTOR

#ifndef GPIO_STEPPER_MOTOR
	#define GPIO_STEPPER_MOTOR			GPIOB
#endif
#ifndef STEPPER_MOTOR_ENABLE_PIN
	#define STEPPER_MOTOR_ENABLE_PIN	13
#endif
#ifndef STEPPER_MOTOR_DIR_PIN
	#define STEPPER_MOTOR_DIR_PIN 		14
#endif
#ifndef STEPPER_MOTOR_DIR_PULSE
	#define STEPPER_MOTOR_PULSE_PIN 	15
#endif
#ifndef STEPPER_MOTOR_TIMER
	#define STEPPER_MOTOR_TIMER			TIMER2_ID
	#define STEPPER_MOTOR_timer_irq_handler		TIMER2_user_handler_it
#endif

#define DEFAULT_PULSE_PERIOD 100

static void STEPPER_MOTOR_pin_set(uint32_t pin, bool_e b);

void STEPPER_MOTORS_init(void)
{
	//Configurer en sortie OD les sorties
	BSP_GPIO_PinCfg(GPIO_STEPPER_MOTOR, STEPPER_MOTOR_ENABLE_PIN, GPIO_MODE_OUTPUT_OD,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIO_STEPPER_MOTOR, STEPPER_MOTOR_DIR_PIN, GPIO_MODE_OUTPUT_OD,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIO_STEPPER_MOTOR, STEPPER_MOTOR_PULSE_PIN, GPIO_MODE_OUTPUT_OD,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);
	
	//Appliquer 1 sur toutes les sorties
	HAL_GPIO_WritePin(GPIO_STEPPER_MOTOR, STEPPER_MOTOR_ENABLE_PIN, 1);
	HAL_GPIO_WritePin(GPIO_STEPPER_MOTOR, STEPPER_MOTOR_DIR_PIN, 1);
	HAL_GPIO_WritePin(GPIO_STEPPER_MOTOR, STEPPER_MOTOR_PULSE_PIN, 1);

	STEPPER_MOTOR_enable(TRUE);
	
	TIMER_run_us(STEPPER_MOTOR_TIMER, DEFAULT_PULSE_PERIOD, TRUE);
}



void STEPPER_MOTOR_timer_irq_handler(void)
{
	STEPPER_MOTORS_do_pulse();
}

void STEPPER_MOTOR_enable(bool_e enable)
{
	STEPPER_MOTOR_pin_set(STEPPER_MOTOR_ENABLE_PIN, enable);
}


void STEPPER_MOTORS_do_pulse(void)
{
	STEPPER_MOTOR_pin_set(STEPPER_MOTOR_PULSE_PIN, 0);
	Delay_us(30);
	STEPPER_MOTOR_pin_set(STEPPER_MOTOR_PULSE_PIN, 1);
}

void STEPPER_MOTORS_set_dir(bool_e direction)
{
	STEPPER_MOTOR_pin_set(STEPPER_MOTOR_DIR_PIN, direction);
}


static void STEPPER_MOTOR_pin_set(uint32_t pin, bool_e b)
{
	if(pin<8)
	{
		if(b)
			GPIO_STEPPER_MOTOR->CRL &= (uint32_t)(~(3<<(4*(pin))));
		else
			GPIO_STEPPER_MOTOR->CRL |= (3<<(4*(pin)));
	}
	else
	{
		if(b)
			GPIO_STEPPER_MOTOR->CRH &= (uint32_t)(~(3<<(4*(pin - 8))));
		else
			GPIO_STEPPER_MOTOR->CRH |= (3<<(4*(pin - 8)));
	}
}

#endif
