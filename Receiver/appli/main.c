/**
  ******************************************************************************
  * @file    main.c
  * @author  Juickar & JegHeterKevin
  * @date    2022
  * @brief   Main function implementing graphical operations and LED handling.
  ******************************************************************************
*/

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "MatrixLed/WS2812S.h"
#include <stdio.h>
#include <string.h>


#define COLOR_BLUE  0x000010 /**< Color code for blue. */
#define COLOR_RED   0x100000 /**< Color code for red. */
#define COLOR_GREEN 0x001000 /**< Color code for green. */

int16_t x = 160; /**< Initial x-coordinate. */
int16_t y = 120; /**< Initial y-coordinate. */
int8_t dx, dy, color, size, mode; /**< Variables for movement, color, size, and mode. */

/**
 * @brief Controls the green LED state.
 * @param b Boolean value (true for on, false for off).
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
void process_ms(void) {
    if (t)
        t--;
}

/**
 * @brief Processes received UART data to update drawing parameters.
 */
void process_uart_rx(void) {
    static uint8_t buffer[8];
    static uint16_t index = 0;
    uint8_t c;

    if (UART_data_ready(UART2_ID)) {
        c = UART_getc(UART2_ID);
        buffer[index] = c;

        if (c == 0x7D) { // End of frame
            if (buffer[0] == 0x7C && index == 6) { // Valid frame
                dx = (int8_t)buffer[1];
                dy = (int8_t)buffer[2];
                color = (int8_t)buffer[3];
                size = (int8_t)buffer[4];
                mode = (int8_t)buffer[5];

                if (dx > -20 && dx < 20 && dy > -20 && dy < 20) {
                    trace();
                    LED_MATRIX_color(color);
                }
            }
            index = 0; // Reset buffer index
        } else if (index < 7) {
            if (index > 0 || c == 0x7C)
                index++;
        }
    }
}

/**
 * @brief Handles graphical operations based on the current mode.
 */
void trace(void) {
    typedef enum {
        NONE = 0,
        WRITE,
        ERASE,
        ERASE_ALL,
        CALIBRATE,
    } state_e;

    if (mode == NONE) { // Update position only
        x -= dx;
        y -= dy;
        if (x < 0) x = 0;
        if (x > 320) x = 320;
        if (y < 0) y = 0;
        if (y > 240) y = 240;
    }

    if (mode == WRITE) {
        draw(dx, dy, color, size);
    }

    if (mode == ERASE) {
        draw(dx, dy, 4, size); // Use white to erase
    }

    if (mode == ERASE_ALL) {
        ILI9341_Fill(ILI9341_COLOR_WHITE); // Clear the screen
    }

    if (mode == CALIBRATE) {
        x = 160;
        y = 120; // Reset to center
    }
}

/**
 * @brief Draws a line or shapes based on the specified parameters.
 * @param dx X-axis delta.
 * @param dy Y-axis delta.
 * @param color Drawing color.
 * @param size Line thickness.
 */
void draw(int dx, int dy, int color, int size) {
    int colors[] = {ILI9341_COLOR_BLACK, ILI9341_COLOR_RED, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE};
    int x0, y0, x1, y1;

    int new_x = x - dx;
    int new_y = y - dy;

    // Boundary check
    if (new_x < 0) new_x = 0;
    if (new_x > 320) new_x = 320;
    if (new_y < 0) new_y = 0;
    if (new_y > 240) new_y = 240;

    for (int i = 0; i < size; i++) {
        x0 = x - i;
        x1 = new_x - i;
        y0 = y - i;
        y1 = new_y - i;

        // Boundary check for each point
        if (x0 < 0) x0 = 0;
        if (x0 > 320) x0 = 320;
        if (y0 < 0) y0 = 0;
        if (y0 > 240) y0 = 240;
        if (x1 < 0) x1 = 0;
        if (x1 > 320) x1 = 320;
        if (y1 < 0) y1 = 0;
        if (y1 > 240) y1 = 240;

        // Draw lines
        ILI9341_DrawLine(x0, y, x1, new_y, colors[color]);
        ILI9341_DrawLine(x, y0, new_x, y1, colors[color]);
    }

    x -= dx; // Update position
    y -= dy;

    // Final boundary check
    if (x < 0) x = 0;
    if (x > 320) x = 320;
    if (y < 0) y = 0;
    if (y > 240) y = 240;
}

/**
 * @brief Main function to initialize peripherals and handle tasks.
 */
int main(void) {
    HAL_Init(); // Initialize the HAL layer.

    // Initialize UART2 at 115200 baud (92 kbits/s). PA2: Tx, PA3: Rx.
    UART_init(UART2_ID, 115200);

    // Redirect printf outputs to UART2.
    SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);
    printf("Starting...\n");

    // Initialize the GPIO port for the green LED.
    BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

    // Initialize the GPIO port for the blue button.
    BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH);

    // Add process_ms to the list of SysTick callbacks.
    Systick_add_callback_function(&process_ms);

    // Initialize the ILI9341 screen.
    ILI9341_Init();
    ILI9341_Fill(ILI9341_COLOR_WHITE); // Clear the screen

    // Initialize the LED matrix.
    LED_MATRIX_init();

    // Main loop
    while (1) {
        process_uart_rx(); // Handle incoming UART data
    }
}
