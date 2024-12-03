# Transmitter Module for RPEN System

The **Transmitter Module** is part of the RPEN system designed to capture input movements and transmit graphical parameters to the Receiver Module over UART.

## Features
- Captures movement using an MPU6050 accelerometer.
- Detects button presses for mode selection and parameter adjustments.
- Sends formatted data packets to the Receiver Module via UART.
- Configurable drawing parameters:
  - Movement in X and Y directions.
  - Color selection.
  - Drawing size.
  - Drawing mode.

## How It Works
1. **Input Capture:**
   - Reads accelerometer data from the MPU6050 to calculate movement (dx, dy).
   - Detects button presses to change modes, colors, and sizes.
2. **Data Transmission:**
   - Formats data into a structured packet.
   - Transmits the packet over UART to the Receiver Module.

## File Structure
- **`main.c`**: Implements the main logic, including data capture and transmission.
- **`stm32f1_mpu6050.c`**: Driver for the MPU6050 accelerometer.
- **`button_1.c`**: Handles button states and events.

## Hardware Requirements
- **Microcontroller**: STM32F103RB (Nucleo board).
- **Accelerometer**: MPU6050 for movement detection.
- **Buttons**: 3 buttons for interactive control.
- **Communication**: UART (115200 baud rate).

## Installation and Usage
1. Connect the transmitter hardware:
   - MPU6050 accelerometer to the STM32F103RB.
   - Buttons to designated GPIO pins.
2. Flash the firmware onto the STM32F103RB.
3. Power on the transmitter.
4. Move the device or press buttons to change parameters and send data to the receiver.

## Data Packet Format
- **Start Byte (0x7C)**
- **X-axis Movement (dx)**
- **Y-axis Movement (dy)**
- **Color Index**
- **Size**
- **Mode**
- **End Byte (0x7D)**

## Supported Modes
- **NONE**: Moves the cursor without drawing.
- **WRITE**: Enables drawing with the current parameters.
- **ERASE**: Erases lines by sending white as the color.
- **ERASE_ALL**: Clears the screen on the receiver.
- **CALIBRATE**: Resets the cursor on the receiver to the center.

## Contributing
Contributions to improve the transmitter module are welcome. Please submit pull requests or raise issues for discussion.

---
