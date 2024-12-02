# RPEN - Connected Pen

RPEN (Remote Pen) is an innovative connected pen project developed as part of the DEEP (Digital Embedded Electronics Project) program at ESEO. This pen allows users to write on a screen remotely via Bluetooth, providing a practical and interactive solution for teaching or creative applications.

## Features

- **Remote Writing:** The pen uses position sensors to capture movements and transcribe them onto a TFT screen.
- **Interactive Modes:**
  - **Writing Mode:** Draw with customizable colors and sizes.
  - **Calibration Mode:** Re-center the cursor on the screen.
  - **Erase Mode:** Remove drawings by overlaying white lines.
- **Style Customization:**
  - Modify drawing size and color via dedicated buttons.
- **Interactive Screen:** The pen pairs with a TFT screen controlled by a microcontroller receiving data via UART Bluetooth.

## Technical Specifications

### Hardware
- **Connected Pen:**
  - Microcontroller based on the Bluepill board.
  - MPU6050 sensor for motion detection.
  - WS2812S RGB LED for indicating the current color.
  - Buttons for functionality control.
- **TFT Screen:**
  - Controlled via SPI.
  - UART Bluetooth interface for receiving data.

### Software
- **Data Transmission:**
  - Motion data and parameters (color, size, mode) are sent as Bluetooth frames.
- **Core Algorithm:**
  - The pen stores motion and parameters in variables.
  - The screen interprets the data to draw or perform the corresponding actions.

## Usage

### Pen Controls
- **Button 1:** Writing mode.
- **Button 2:** Change size or color.
- **Button 3:** Erase the screen or activate erase-by-drawing mode.

To navigate the menus, tilt the pen in the desired direction, similar to a Wii controller.

### Tests
The project underwent various tests:
- Verification of the microcontroller’s power supply.
- Display testing on the TFT screen.
- Changing colors and sizes using buttons.
- Bluetooth functionality for data transmission.

## Contributions

### Developers
- **[Kevin POTTIER](https://github.com/JegHeterKevin1)**:
- **[Julien RACKI](https://github.com/juickar)**:

## Potential Improvements
- Reduce noise from the MPU6050 sensor data using low-pass filters.
- Optimize Bluetooth frame processing.
- Improve motion precision for smoother control.

## License

This project is open for educational and experimental purposes. Any reuse must include proper attribution.

---

**Note:** Associated files, including the PCB schematic and the CAD design of the pen case, are available upon request.
