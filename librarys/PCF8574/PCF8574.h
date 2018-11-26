/**
 * @brief PCF8574 arduino library
 * @author SkyWodd <skywodd@gmail.com>
 * @version 2.0
 * @link http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This class is designed to allow user to use PCF8574 gpio expander like any standard arduino pins.\n
 * This class provided standards arduino functions like pinMode, digitalWrite, digitalRead, ...\n
 * This new version is fully optimized and documented.\n
 * \n
 * Please report bug to <skywodd at gmail.com>
 *
 * @section license_sec License
 *  This program is free software: you can redistribute it and/or modify\n
 *  it under the terms of the GNU General Public License as published by\n
 *  the Free Software Foundation, either version 3 of the License, or\n
 *  (at your option) any later version.\n
 * \n
 *  This program is distributed in the hope that it will be useful,\n
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 *  GNU General Public License for more details.\n
 * \n
 *  You should have received a copy of the GNU General Public License\n
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
 *
 * @section other_sec Others notes and compatibility warning
 * Compatible with arduino 1.0.x and >=0023\n
 * Retro-compatible with the previous library version
 */
#ifndef PCF8574_H
#define PCF8574_H

/** Comment this define to disable interrupt support */
#define PCF8574_INTERRUPT_SUPPORT
#define PCF8574_IMPORT

/* Retro-compatibility with arduino 0023 and previous version */
#if ARDUINO >= 100
#include "Arduino.h"
#include "FunctionalInterrupt.h"
#define I2CWRITE(x) Wire.write(x)
#define I2CREAD() Wire.read()
#else
#include "WProgram.h"
#define I2CWRITE(x) Wire.send(x)
#define I2CREAD() Wire.receive()
#define INPUT_PULLUP 2
#endif

/**
 * @brief PCF8574 Arduino class
 */
class PCF8574 {
public:
	PCF8574();

	void begin(uint8_t address = 0x27);

	void pinMode(uint8_t pin, uint8_t mode);

	void digitalWrite(uint8_t pin, uint8_t value);

	uint8_t digitalRead(uint8_t pin);

	void write(uint8_t value);

	uint8_t read();
	void clear();

	void set();

	void toggle(uint8_t pin);

	void pullUp(uint8_t pin);

	void pullDown(uint8_t pin);
	void blink(uint8_t pin, uint16_t count, uint32_t duration);

#ifdef PCF8574_INTERRUPT_SUPPORT
	void enableInterrupt(uint8_t pin,std::function<void(void)> func);
#endif

protected:
	/** Output pins values */
	volatile uint8_t _PORT;
	/** Current input pins values */
	volatile uint8_t _PIN;
	/** Pins modes values (OUTPUT or INPUT) */
	volatile uint8_t _DDR;

	/** PCF8574 I2C address */
	uint8_t _address;

#ifdef PCF8574_INTERRUPT_SUPPORT
	/** ISR ignore flag */
	volatile uint8_t _isrIgnore;

	/** PCINT pin used for "INT" pin handling */
	uint8_t _pcintPin;

#endif

	void readGPIO();
	void updateGPIO();
};

#endif
