/*******************************************************************************
 * @file    main.c
 * @author  Juickar & JegHeterKevin
 * @date    2022
 * @brief   Main entry point and state machine for RPEN transmitter device.
 ******************************************************************************/

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "stm32f1_mpu6050.h"
#include "systick.h"
#include "Core/Inc/button_1.h"
#include "Core/Inc/led.h"
#include "Config/config.h"

#define MULTIPLE_PRESS_DURATION_MS 3000U  /**< Duration in ms for multi-press detection. */

static MPU6050_t mpu6050_data; /**< MPU6050 sensor data structure. */

// Drawing parameters
static int8_t g_size = 5;      /**< Drawing size. */
static int8_t g_color = 1;     /**< Color index. */
static int8_t g_mode = 1;      /**< Operation mode. */

// Timer for periodic tasks
static volatile uint32_t g_timer_ms = 0;

// Function prototypes
static void StateMachine(void);
static void ProcessMs(void);
static void SendUartPacket(int8_t dx, int8_t dy, int8_t color, int8_t size, int8_t mode);

/**
 * @brief Decrement timer every millisecond (called by SysTick interrupt).
 */
static void ProcessMs(void) {
    if (g_timer_ms > 0) {
        g_timer_ms--;
    }
}

/**
 * @brief Main entry point. Initializes peripherals and runs the main loop.
 */
int main(void) {
    HAL_Init();

    // UART2: 115200 baud, PA2 (Tx), PA3 (Rx) - redirected to debugger probe
    UART_init(UART2_ID, 115200);
    SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

    // GPIO: Green LED output, Blue button input
    BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
    BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);

    // SysTick callback for ms timer
    Systick_add_callback_function(&ProcessMs);

    // MPU6050 sensor initialization
    MPU6050_Init(&mpu6050_data, GPIOB, GPIO_PIN_5, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s);

    int8_t dx = 0, dy = 0;

    while (1) {
        StateMachine();

        if (g_timer_ms == 0) {
            g_timer_ms = 50; // 50ms periodic task

            MPU6050_ReadAll(&mpu6050_data);
            dx = (int8_t)(mpu6050_data.Accelerometer_X / 410);
            dy = (int8_t)(mpu6050_data.Accelerometer_Y / 410);

            SendUartPacket(dx, dy, g_color, g_size, g_mode);
        }
    }
}

/**
 * @brief State machine for button events and device modes.
 */
static void StateMachine(void) {
    typedef enum {
        STATE_INIT = 0,
        STATE_DEFAULT,
        STATE_1_PRESS,
        STATE_2_PRESS,
        STATE_3_PRESS,
        STATE_CALIBRATE,
        STATE_COLOR,
        STATE_SIZE,
        STATE_WRITE,
        STATE_ERASE,
        STATE_ERASE_ALL
    } State_e;

    static State_e state = STATE_INIT;

    button_event_e btn1 = BUTTON_1_state_machine();
    button_event_e btn2 = BUTTON_2_state_machine();
    button_event_e btn3 = BUTTON_3_state_machine();

    switch (state) {
        case STATE_INIT:
            LED_init();
            BUTTON_1_init();
            BUTTON_2_init();
            BUTTON_3_init();
            state = STATE_DEFAULT;
            break;

        case STATE_DEFAULT:
            g_mode = 0;
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_1_PRESS;
            } else if (btn2 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_2_PRESS;
            } else if (btn3 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_3_PRESS;
            }
            break;

        case STATE_1_PRESS:
            if (btn2 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_CALIBRATE;
            } else if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_WRITE;
            } else if (btn3 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_2_PRESS:
            if (btn2 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_COLOR;
            } else if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_SIZE;
            } else if (btn3 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_3_PRESS:
            if (btn2 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_ERASE;
            } else if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_ERASE_ALL;
            } else if (btn3 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_CALIBRATE:
            g_mode = 4;
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_COLOR:
            if (btn2 == BUTTON_EVENT_SHORT_PRESS) {
                g_color = (g_color + 1) % 4;
            }
            if (btn3 == BUTTON_EVENT_SHORT_PRESS) {
                g_color = (g_color - 1 + 4) % 4;
            }
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_SIZE:
            if (btn2 == BUTTON_EVENT_SHORT_PRESS) {
                g_size = (g_size + 5 > 50) ? 50 : g_size + 5;
            }
            if (btn3 == BUTTON_EVENT_SHORT_PRESS) {
                g_size = (g_size - 5 < 5) ? 5 : g_size - 5;
            }
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_WRITE:
            g_mode = 1;
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_ERASE:
            g_mode = 2;
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        case STATE_ERASE_ALL:
            g_mode = 3;
            if (btn1 == BUTTON_EVENT_SHORT_PRESS) {
                state = STATE_DEFAULT;
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Send formatted data packet via UART.
 * @param dx X-axis movement
 * @param dy Y-axis movement
 * @param color Color index
 * @param size Drawing size
 * @param mode Operation mode
 */
static void SendUartPacket(int8_t dx, int8_t dy, int8_t color, int8_t size, int8_t mode) {
    UART_putc(UART2_ID, 0x7C); // Start byte
    UART_putc(UART2_ID, dx);
    UART_putc(UART2_ID, dy);
    UART_putc(UART2_ID, color);
    UART_putc(UART2_ID, size);
    UART_putc(UART2_ID, mode);
    UART_putc(UART2_ID, 0x7D); // End byte
}
