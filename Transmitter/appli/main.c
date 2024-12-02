/**
  ******************************************************************************
  * @file    main.c
  * @author  Juickar & JegHeterKevin
  * @date    2022
  * @brief   Main function implementing a state machine for a connected device.
  ******************************************************************************
*/

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "stm32f1_mpu6050.h"
#include "systick.h"
#include "button_1.h"
#include "led.h"

#define MULTIPLE_PRESS_DURATION 3000 /**< Duration in milliseconds for handling multiple button presses. */

MPU6050_t datas; /**< Structure for storing MPU6050 sensor data. */


/**
 * @brief State machine implementation.
 */
static void state_machine(void);

/**
 * @brief Timer processing function called every millisecond.
 */
static void process_ms(void);

int8_t size = 5;      /**< Size of the drawing or operation. */
int8_t num_color = 1; /**< Current color index. */
int8_t mode = 1;      /**< Current mode of operation. */

/**
 * @brief Turns the green LED on or off.
 * @param b Boolean value (true to turn on, false to turn off).
 */
void writeLED(bool_e b) {
    HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

/**
 * @brief Reads the state of the blue button.
 * @return Boolean value (true if pressed, false otherwise).
 */
bool_e readButton(void) {
    return !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
}

static volatile uint32_t t = 0; /**< Timer variable for periodic tasks. */

/**
 * @brief Decrements the timer variable every millisecond if it's greater than zero.
 */
static void process_ms(void) {
    if (t)
        t--;
}

/**
 * @brief Main function to initialize peripherals and run the state machine.
 */
int main(void) {
    HAL_Init(); /**< Initialize the Hardware Abstraction Layer (HAL). */

    // Initialize UART2 at 115200 baud (92 kbits/s). Pins PA2: Tx, PA3: Rx.
    // Note: Pins PA2 and PA3 are redirected to the debugger probe and not directly available.
    UART_init(UART2_ID, 115200);

    // Redirect printf outputs to UART2.
    SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

    // Initialize the GPIO port for the green LED.
    BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

    // Initialize the GPIO port for the blue button.
    BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);

    // Add process_ms to the list of functions called every millisecond by the SysTick interrupt routine.
    Systick_add_callback_function(&process_ms);

    // Initialize the MPU6050 sensor.
    MPU6050_Init(&datas, GPIOB, GPIO_PIN_5, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s);

    int8_t dx = 0; /**< X-axis data for movement. */
    int8_t dy = 0; /**< Y-axis data for movement. */

    // Infinite loop to process tasks.
    while (1) {
        state_machine(); /**< Run the state machine. */

        // If timer reaches zero, perform periodic tasks.
        if (!t) {
            t = 50; /**< Reset the timer. */

            // Read data from the MPU6050 sensor.
            MPU6050_ReadAll(&datas);

            dx = (int8_t)(datas.Accelerometer_X / 410); /**< Scale the X-axis data. */
            dy = (int8_t)(datas.Accelerometer_Y / 410); /**< Scale the Y-axis data. */

            // Send formatted data through UART.
            UART_putc(UART2_ID, 0x7C); /**< Start byte. */
            UART_putc(UART2_ID, dx);   /**< X-axis data. */
            UART_putc(UART2_ID, dy);   /**< Y-axis data. */
            UART_putc(UART2_ID, num_color); /**< Current color index. */
            UART_putc(UART2_ID, size);      /**< Current size. */
            UART_putc(UART2_ID, mode);      /**< Current mode. */
            UART_putc(UART2_ID, 0x7D); /**< End byte. */
        }
    }
}

/**
 * @brief Implements a state machine to handle button events and modes.
 */
static void state_machine(void) {
    typedef enum {
        INIT = 0,          /**< Initialization state. */
        DEFAULT_STATE,     /**< Default state with no active actions. */
        DEFAULT_STATE_1_PRESS, /**< State triggered by one button press. */
        DEFAULT_STATE_2_PRESS, /**< State triggered by two button presses. */
        DEFAULT_STATE_3_PRESS, /**< State triggered by three button presses. */
        CALIBRATE,         /**< Calibration state. */
        COLOR,             /**< Color selection state. */
        CHANGE_SIZE,       /**< Size adjustment state. */
        WRITE,             /**< Write or draw mode. */
        ERASE,             /**< Erase mode. */
        ERASE_ALL          /**< Erase all mode. */
    } state_e;

    static state_e etat = INIT; /**< Current state of the machine. */

    // Variables to hold button events.
    button_event_e button_1 = BUTTON_1_state_machine();
    button_event_e button_2 = BUTTON_2_state_machine();
    button_event_e button_3 = BUTTON_3_state_machine();

    switch (etat) {
        case INIT:
            LED_init();
            BUTTON_1_init();
            BUTTON_2_init();
            BUTTON_3_init();
            etat = DEFAULT_STATE;
            break;

        case DEFAULT_STATE:
            mode = 0;
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE_1_PRESS;
            } else if (button_2 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE_2_PRESS;
            } else if (button_3 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE_3_PRESS;
            }
            break;

        case DEFAULT_STATE_1_PRESS:
            if (button_2 == BUTTON_EVENT_SHORT_PRESS) {
                etat = CALIBRATE;
            } else if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = WRITE;
            } else if (button_3 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case DEFAULT_STATE_2_PRESS:
            if (button_2 == BUTTON_EVENT_SHORT_PRESS) {
                etat = COLOR;
            } else if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = CHANGE_SIZE;
            } else if (button_3 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case DEFAULT_STATE_3_PRESS:
            if (button_2 == BUTTON_EVENT_SHORT_PRESS) {
                etat = ERASE;
            } else if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = ERASE_ALL;
            } else if (button_3 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case CALIBRATE:
            mode = 4;
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case COLOR:
            if (button_2 == BUTTON_EVENT_SHORT_PRESS) {
                num_color = (num_color + 1) % 4; /**< Cycle through colors. */
            }
            if (button_3 == BUTTON_EVENT_SHORT_PRESS) {
                num_color = (num_color - 1 + 4) % 4; /**< Cycle backwards through colors. */
            }
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case CHANGE_SIZE:
            if (button_2 == BUTTON_EVENT_SHORT_PRESS) {
                size = (size + 5 > 50) ? 50 : size + 5; /**< Increment size with a limit. */
            }
            if (button_3 == BUTTON_EVENT_SHORT_PRESS) {
                size = (size - 5 < 5) ? 5 : size - 5; /**< Decrement size with a limit. */
            }
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case WRITE:
            mode = 1;
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case ERASE:
            mode = 2;
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        case ERASE_ALL:
            mode = 3;
            if (button_1 == BUTTON_EVENT_SHORT_PRESS) {
                etat = DEFAULT_STATE;
            }
            break;

        default:
            break;
    }
}
