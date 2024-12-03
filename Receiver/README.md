# Receiver Module for RPEN System

The **Receiver Module** is part of the RPEN system designed to process and display graphical data transmitted from the Transmitter Module. It features real-time updates on an ILI9341 TFT screen and controls an LED matrix for enhanced visual feedback.

## Features
- Receives data over UART to control drawing operations.
- Displays graphics on a 320x240 ILI9341 TFT screen.
- Controls an LED matrix for color indication.
- Supports multiple drawing modes:
  - **WRITE**: Draws lines with specified color and size.
  - **ERASE**: Erases lines using white color.
  - **ERASE_ALL**: Clears the entire screen.
  - **CALIBRATE**: Resets the cursor position to the center of the screen.
- Processes button inputs for interactive control.

## How It Works
1. **UART Communication:**
   - Receives formatted data packets from the transmitter.
   - Updates drawing parameters based on received values.
2. **Graphical Processing:**
   - Draws lines or erases parts of the screen using the ILI9341 TFT driver.
3. **LED Matrix Feedback:**
   - Provides visual feedback for the selected color.

## File Structure
- **`main.c`**: Implements the main logic, including UART processing and drawing functions.
- **`stm32f1_ili9341.c`**: Driver for the ILI9341 TFT screen.
- **`WS2812S.c`**: Driver for controlling the LED matrix.

## Hardware Requirements
- **Microcontroller**: STM32F103RB (Nucleo board).
- **Screen**: ILI9341 TFT (320x240 resolution).
- **LED Matrix**: WS2812S-compatible LEDs.
- **Communication**: UART (115200 baud rate).

## Installation and Usage
1. Connect the receiver hardware:
   - ILI9341 TFT screen to the STM32F103RB.
   - WS2812S LED matrix to the designated GPIO pins.
2. Flash the firmware onto the STM32F103RB.
3. Power on the system and ensure the transmitter is operational.
4. Observe graphical updates and LED feedback as the system runs.

## Supported Modes
- **NONE**: Moves the cursor without drawing.
- **WRITE**: Draws lines with the specified parameters.
- **ERASE**: Erases lines by drawing white over them.
- **ERASE_ALL**: Clears the entire screen.
- **CALIBRATE**: Resets the cursor to the center.

## Contributing
Contributions to improve the receiver module are welcome. Please submit pull requests or raise issues for discussion.

---

