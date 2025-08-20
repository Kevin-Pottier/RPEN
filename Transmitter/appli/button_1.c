#include "button_1.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "stm32f1xx_hal_gpio.h"

/*******************************************************************************
 * @file    button_1.c
 * @author  Nirgal
 * @date    2019
 * @brief   Button handling and debouncing for RPEN transmitter device.
 ******************************************************************************/


#define LONG_PRESS_DURATION_MS  1000U   /**< Long press duration in ms. */


static void Button_ProcessMs(void);

static volatile bool_e g_flag_10ms = FALSE;
static volatile uint32_t g_t1 = 0;
static volatile uint32_t g_t2 = 0;
static volatile uint32_t g_t3 = 0;
static bool_e g_initialized = FALSE;


/**
 * @brief Initialize button 1 (GPIOA, PIN 12).
 */
void BUTTON_1_init(void)
{
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
	Systick_add_callback_function(&Button_ProcessMs);
	g_initialized = TRUE;
}

/**
 * @brief Initialize button 2 (GPIOA, PIN 11).
 */
void BUTTON_2_init(void)
{
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
	Systick_add_callback_function(&Button_ProcessMs);
	g_initialized = TRUE;
}

/**
 * @brief Initialize button 3 (GPIOA, PIN 10).
 */
void BUTTON_3_init(void)
{
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);
	Systick_add_callback_function(&Button_ProcessMs);
	g_initialized = TRUE;
}


/**
 * @brief SysTick callback for button timing and debouncing (every ms).
 */
static void Button_ProcessMs(void)
{
	static uint32_t t10ms = 0;
	t10ms = (t10ms + 1) % 10;
	if (t10ms == 0)
		g_flag_10ms = TRUE;
	if (g_t1)
		g_t1--;
	if (g_t2)
		g_t2--;
	if (g_t3)
		g_t3--;
}


/**
 * @brief State machine for button 1 (GPIOA, PIN 12).
 * @return Button event (short press, long press, or none)
 */
button_event_e BUTTON_1_state_machine(void)
{
	typedef enum {
		STATE_INIT = 0,
		STATE_WAIT_BUTTON,
		STATE_PRESSED,
		STATE_WAIT_RELEASE
	} ButtonState_e;

	static ButtonState_e state = STATE_INIT;
	button_event_e event = BUTTON_EVENT_NONE;
	bool_e current_button;

	if (g_flag_10ms && g_initialized) {
		g_flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
		switch (state) {
			case STATE_INIT:
				state = STATE_WAIT_BUTTON;
				break;
			case STATE_WAIT_BUTTON:
				event = BUTTON_EVENT_NONE;
				if (current_button) {
					g_t1 = LONG_PRESS_DURATION_MS;
					state = STATE_PRESSED;
				}
				break;
			case STATE_PRESSED:
				if (g_t1 == 0) {
					event = BUTTON_EVENT_LONG_PRESS;
					state = STATE_WAIT_RELEASE;
				} else if (!current_button) {
					event = BUTTON_EVENT_SHORT_PRESS;
					state = STATE_WAIT_BUTTON;
				}
				break;
			case STATE_WAIT_RELEASE:
				if (!current_button) {
					state = STATE_WAIT_BUTTON;
				}
				break;
			default:
				state = STATE_INIT;
				break;
		}
	}
	return event;
}

/**
 * @brief State machine for button 2 (GPIOA, PIN 11).
 * @return Button event (short press, long press, or none)
 */
button_event_e BUTTON_2_state_machine(void)
{
	typedef enum {
		STATE_INIT = 0,
		STATE_WAIT_BUTTON,
		STATE_PRESSED,
		STATE_WAIT_RELEASE
	} ButtonState_e;

	static ButtonState_e state = STATE_INIT;
	button_event_e event = BUTTON_EVENT_NONE;
	bool_e current_button;

	if (g_flag_10ms && g_initialized) {
		g_flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
		switch (state) {
			case STATE_INIT:
				state = STATE_WAIT_BUTTON;
				break;
			case STATE_WAIT_BUTTON:
				event = BUTTON_EVENT_NONE;
				if (current_button) {
					g_t2 = LONG_PRESS_DURATION_MS;
					state = STATE_PRESSED;
				}
				break;
			case STATE_PRESSED:
				if (g_t2 == 0) {
					event = BUTTON_EVENT_LONG_PRESS;
					state = STATE_WAIT_RELEASE;
				} else if (!current_button) {
					event = BUTTON_EVENT_SHORT_PRESS;
					state = STATE_WAIT_BUTTON;
				}
				break;
			case STATE_WAIT_RELEASE:
				if (!current_button) {
					state = STATE_WAIT_BUTTON;
				}
				break;
			default:
				state = STATE_INIT;
				break;
		}
	}
	return event;
}

/**
 * @brief State machine for button 3 (GPIOA, PIN 10).
 * @return Button event (short press, long press, or none)
 */
button_event_e BUTTON_3_state_machine(void)
{
	typedef enum {
		STATE_INIT = 0,
		STATE_WAIT_BUTTON,
		STATE_PRESSED,
		STATE_WAIT_RELEASE
	} ButtonState_e;

	static ButtonState_e state = STATE_INIT;
	button_event_e event = BUTTON_EVENT_NONE;
	bool_e current_button;

	if (g_flag_10ms && g_initialized) {
		g_flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
		switch (state) {
			case STATE_INIT:
				state = STATE_WAIT_BUTTON;
				break;
			case STATE_WAIT_BUTTON:
				event = BUTTON_EVENT_NONE;
				if (current_button) {
					g_t3 = LONG_PRESS_DURATION_MS;
					state = STATE_PRESSED;
				}
				break;
			case STATE_PRESSED:
				if (g_t3 == 0) {
					event = BUTTON_EVENT_LONG_PRESS;
					state = STATE_WAIT_RELEASE;
				} else if (!current_button) {
					event = BUTTON_EVENT_SHORT_PRESS;
					state = STATE_WAIT_BUTTON;
				}
				break;
			case STATE_WAIT_RELEASE:
				if (!current_button) {
					state = STATE_WAIT_BUTTON;
				}
				break;
			default:
				state = STATE_INIT;
				break;
		}
	}
	return event;
}
