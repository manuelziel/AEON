# README

This is a project for a remaining life display based on the RP2040 microcontroller. The focus of the project is to display the remaining lifespan in days. The display also shows the current date and time.

## Hardware

The project uses the following components:

- RP2040 microcontroller (16MB noFS / 8MB-8MB FS, Generic SPI/2, 133MHz, C++ Exceptions Enabled, Pico SDK)
- NOR Flash W25Q128JWPIQ (SPI) (128 Mbit - 16MB)
- DS3231SN RTC (I2C)
- EA OLEDM128-6LWA or EA OLEDL128-6LWA display (SSD1306, 128x64 pixels with I2C)

## Dependencies

The following external package was used:

- https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

## License

This project is licensed under the MIT License. Feel free to modify and distribute the code.

## Installation

1. Download and install the Arduino IDE from https://www.arduino.cc/en/software/.
2. Add the following URL to the Additional Board Manager URLs in the Arduino IDE settings: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
3. Install the RP2040 boards package using the Board Manager in the Arduino IDE.
4. Connect the DS3231SN RTC to the I2C pins on the RP2040 board (SDA, SCL).
5. Connect the EA OLEDM128-6LWA display to the I2C pins on the RP2040 board (SDA, SCL).
6. Open the "AEON.ino" sketch in the Arduino IDE.
7. Compile and upload the sketch to the RP2040 board.
8. The display will now show the remaining lifespan, date and time.

## Usage

The display will show the remaining lifespan in days based on the birthdate of the user, which can be set with the buttons. The remaining lifespan is calculated using actuarial life tables, which provide an estimate of the average remaining lifespan for a given age and gender. The actuarial life tables used in this project are based on data from the United States Social Security Administration.

The display also shows the current date and time, which is obtained from the DS3231SN RTC.

## Credits

This project was created by Manuel Ziel as a hobby project. The code for the RTC and display were adapted from the following libraries:

- Adafruit_SSD1306 (https://github.com/adafruit/Adafruit_SSD1306)
- Adafruit_GFX (https://github.com/adafruit/Adafruit-GFX-Library)
- DS3231 / RTClib (https://github.com/adafruit/RTClib)

## Contribution

Contributions to this project are welcome. If you find a bug or have an idea for an improvement, feel free to submit a pull request or open an issue on GitHub.
