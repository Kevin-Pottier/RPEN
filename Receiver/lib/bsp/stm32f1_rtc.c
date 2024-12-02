/*
 * stm32f1_rtc.c
 *
 *  Created on: 21 août 2015
 *      Author: Nirgal
 */
#include "config.h"
#if USE_RTC
#include "stm32f1_rtc.h"
#include "stm32f1_uart.h"

//______________________________________________

/**	running_e DEMO_RTC_process_main(void)	 // Fonction de démonstration (partielle) de l'utilisation de ce module.
 * @brief exemple d'utilisation du module RTC.
 */
running_e DEMO_RTC_process_main(bool_e ask_for_finish)
{
	typedef enum
	{
		INIT = 0,
		RUN
	}state_e;
	static state_e state = INIT;
	running_e ret;
	ret = IN_PROGRESS;
	static bool_e flag_alarm;
	const char * weekday_str[7] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};

	switch(state)
	{
		case INIT: {
			state = RUN;
			//RTC_init(FALSE);	//Oscillateur INTERNE, imprécis, mais testable sans ajout d'un oscillateur sur PC14 et PC15 !
			RTC_init(TRUE);		//Oscillateur EXTERNE, précis, mais nécessite l'ajout d'un oscillateur sur PC14 et PC15 !

			//	1/04/2015	12h34'56"
			//RTC_TimeTypeDef time = {12, 34, 56};

			RTC_set_alarm(&(RTC_AlarmTypeDef){00, 00, 10}, TRUE, &flag_alarm);

			printf("This demo will print the time every second.\n");
			printf("Commands :\n");
			printf("h : help\n");
			printf("r : reset time & date to default\n");

			break;}
		case RUN:{
			static uint8_t previous_printed_time = 0;
			RTC_TimeTypeDef time;
			RTC_DateTypeDef date;
			RTC_get_time_and_date(&time, &date);
			if(time.Seconds != previous_printed_time)
			{
				printf("%s %2d/%2d/%2d - %2d:%2d:%2d\n", weekday_str[date.WeekDay], date.Date, date.Month, date.Year, time.Hours, time.Minutes, time.Seconds);
				previous_printed_time = time.Seconds;
			}

			uint8_t c;
			if(UART_data_ready(UART2_ID))
			{
				c = UART_getc(UART2_ID);
				switch(c)
				{
					case 'r':
						printf("reset time & date\n");
						RTC_TimeTypeDef time = {00, 00, 00};
						RTC_DateTypeDef date = {RTC_WEEKDAY_MONDAY, RTC_MONTH_APRIL, 1, 20};
						RTC_set_time_and_date(&time, &date);
						break;
					case 'h':
						printf("This demo will print the time every second.\n");
						printf("Commands :\n");
						printf("h : help\n");
						printf("r : reset time & date to default\n");
						break;
				}
			}
			if(flag_alarm)
			{
				printf("alarm occured\n");
				flag_alarm = FALSE;
			}
			if(ask_for_finish)
			{
				state = INIT;
				ret = END_OK;
			}
			break;}
		default:
			break;
	}
	return ret;
}


//______________________________________________



static RTC_HandleTypeDef rtc_handle;
static bool_e initialized = FALSE;
static volatile bool_e * alarm_interrupt_flag = NULL;

/**
 * @brief initialise la RTC
 * @func void RTC_init(bool_e i_installed_an_external_oscillator)
 * @param i_installed_an_external_oscillator :
 * 					pour utiliser la RTC, vous devez relier un quartz de 32768Hz entre PC14 et PC15.
 * 						Si c'est fait, indiquez TRUE
 * 					Sinon, il est possible d'utiliser l'oscillateur interne : indiquez FALSE
 * 					Attention, cet oscillateur est médiocre (pourcentage de déviation de plusieurs pourcents !)
 * 						mais il est pratique pour valider le concept...
 */
void RTC_init(bool_e i_installed_an_external_oscillator)
{
	__HAL_RCC_PWR_CLK_ENABLE();						//Enable the Power Controller (PWR) APB1 interface clock
	HAL_PWR_EnableBkUpAccess();						//Enable access to RTC domain
	if(i_installed_an_external_oscillator)
	{
		uint32_t tickstart;
		__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);

		tickstart = HAL_GetTick();
		while(!__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY))
		{
			if((HAL_GetTick() - tickstart ) > LSE_STARTUP_TIMEOUT)
			{
				printf("LSE is not ready : did you place a 32768 Hz quartz on PC14-PC15?\n");
				return;
			}
		}
		printf("LSE is ready\n");
		__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);	//Select the external RTC clock source
	}
	else
	{
		__HAL_RCC_LSI_ENABLE();
		while(!__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY));
		__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);	//Select the internal RTC clock source
	}
	__HAL_RCC_RTC_ENABLE();							//Enable RTC Clock

	rtc_handle.Instance = RTC;
	rtc_handle.Init.AsynchPrediv = 		256*128;		//128*256 = 32768 => 1Hz !
	rtc_handle.Init.OutPut = 			RTC_OUTPUTSOURCE_NONE;
	HAL_RTC_Init(&rtc_handle);								//Configure the RTC Prescaler (Asynchronous and Synchronous) and RTC hour
	initialized = TRUE;
}


void RTC_set_time_and_date(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate)
{
	HAL_RTC_SetTime(&rtc_handle, sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&rtc_handle, sDate, RTC_FORMAT_BIN);
}

void RTC_set_time(RTC_TimeTypeDef *sTime)
{
	if(initialized)
		HAL_RTC_SetTime(&rtc_handle, sTime, RTC_FORMAT_BIN);
}

void RTC_set_date(RTC_DateTypeDef *sDate)
{
	if(initialized)
		HAL_RTC_SetDate(&rtc_handle, sDate, RTC_FORMAT_BIN);
}

void RTC_get_time_and_date(RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate)
{
	HAL_RTC_GetTime(&rtc_handle, sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&rtc_handle, sDate, RTC_FORMAT_BIN);
}

void RTC_get_time(RTC_TimeTypeDef *sTime)
{
	if(initialized)
		HAL_RTC_GetTime(&rtc_handle, sTime, RTC_FORMAT_BIN);
}

void RTC_get_date(RTC_DateTypeDef *sDate)
{
	if(initialized)
		HAL_RTC_GetDate(&rtc_handle, sDate, RTC_FORMAT_BIN);
}



void RTC_set_alarm(RTC_TimeTypeDef * alarm_time, bool_e enable_interrupt, bool_e * flag)
{
	if(initialized)
	{
		RTC_AlarmTypeDef alarm;
		alarm.Alarm = RTC_ALARM_A;
		alarm.AlarmTime = *alarm_time;
		if(enable_interrupt)
		{
			HAL_RTC_SetAlarm_IT(&rtc_handle, &alarm, RTC_FORMAT_BIN);
			NVIC_EnableIRQ(RTC_IRQn);
			if(flag)
				alarm_interrupt_flag = flag;	//on sauvegarde l'adresse du flag pour qu'il soit levé lors de l'interruption provoquée par l'alarme.
		}
		else
			HAL_RTC_SetAlarm(&rtc_handle, &alarm, RTC_FORMAT_BIN);
	}
}

void RTC_reset_alarm(void)
{
	HAL_RTC_DeactivateAlarm(&rtc_handle, RTC_ALARM_A);
}

void RTC_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&rtc_handle);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(alarm_interrupt_flag)
		*alarm_interrupt_flag = TRUE;
}

void RTC_get_alarm(RTC_AlarmTypeDef *sAlarm, uint32_t Alarm)
{
	if(initialized)
		HAL_RTC_GetAlarm(&rtc_handle, sAlarm, Alarm, RTC_FORMAT_BIN);
}

/*
 * Cette fonction permet de modifier la vitesse de la RTC (à des fins de tests notamment)
 * @param time_acceleration : 1 pour une vitesse normale. 60 pour un écoulement d'une minute par seconde. 5*60 pour 5mn/seconde...
 * @pre		valeur maximale : 32768 secondes par seconde.
 */
void RTC_set_time_acceleration(uint32_t time_acceleration)
{
	if(initialized)
	{
		rtc_handle.Init.AsynchPrediv = 32768/time_acceleration;
		HAL_RTC_Init(&rtc_handle);	//Enable RTC Clock
	}
}

#endif

