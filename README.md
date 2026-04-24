# RP2040 Pico 26-Key Keyboard

A simple 26-key direct USB keyboard built with an RP2040 board and TinyUSB.

## Overview

This project turns an RP2040 Pico into a custom USB HID keyboard with 26 individual buttons connected to GPIO pins.  
It uses the Arduino IDE, Raspberry Pi Pico core, and Adafruit TinyUSB.

## Features

- 26 direct input keys.
- USB HID keyboard output.
- TinyUSB support.
- Fast input response.
- Caps Lock LED support.

## Hardware

- RP2040 Pico / RP2040 Zero.
- 26 push buttons.
- USB cable.
- Jumper wires.
- Optional LED for status indication.

## GPIO Mapping

| Button | GPIO |
|---|---:|
| HIDKEYARROWUP | GP0 |
| HIDKEYARROWDOWN | GP1 |
| HIDKEYARROWLEFT | GP2 |
| HIDKEYARROWRIGHT | GP3 |
| HIDKEYCONTROLRIGHT | GP4 |
| HIDKEYSHIFTRIGHT | GP5 |
| HIDKEYENTER | GP6 |
| HIDKEYHOME | GP7 |
| HIDKEYEND | GP8 |
| HIDKEYKEYPAD0 | GP9 |
| HIDKEYKEYPADDECIMAL | GP10 |
| HIDKEYKEYPAD1 | GP11 |
| HIDKEYKEYPAD2 | GP12 |
| HIDKEYKEYPAD3 | GP13 |
| HIDKEYKEYPAD4 | GP14 |
| HIDKEYKEYPAD5 | GP15 |
| HIDKEYSEMICOLON | GP16 |
| HIDKEYKEYPAD7 | GP17 |
| HIDKEYKEYPAD8 | GP18 |
| HIDKEYL | GP19 |
| HIDKEYSPACE | GP20 |
| HIDKEYESCAPE | GP21 |
| HIDKEYF1 | GP22 |
| HIDKEYF2 | GP26 |
| HIDKEYF3 | GP27 |
| HIDKEYKEYPADADD | GP28 |

All buttons share **GND**.

## Software Setup

1. Install Arduino IDE 1.8.19.
2. Add the RP2040 board manager URL.
3. Install the Raspberry Pi Pico core.
4. Install the Adafruit TinyUSB Library.
5. Select the RP2040 board and the correct UF2 port.

## Upload Steps

1. Hold the BOOT button on the RP2040.
2. Connect the board with a USB cable.
3. The board should appear as a storage device.
4. Compile and upload the sketch.
5. The keyboard will reboot and enumerate as a USB HID device.

## Notes

- The current code supports up to 6 simultaneous pressed keys with the standard USB boot keyboard report.
- The project can be extended to NKRO support if needed.
- If the board is not visible in Arduino, reconnect it and restart Arduino IDE.

## License

This project uses TinyUSB and Adafruit-related code. Respect the original license terms included in the source.
