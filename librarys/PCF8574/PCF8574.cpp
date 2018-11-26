/*
 * See header file for details
 *
 *  This program is free software: you can redistribute it and/or modify\n
 *  it under the terms of the GNU General Public License as published by\n
 *  the Free Software Foundation, either version 3 of the License, or\n
 *  (at your option) any later version.\n
 * 
 *  This program is distributed in the hope that it will be useful,\n
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 *  GNU General Public License for more details.\n
 * 
 *  You should have received a copy of the GNU General Public License\n
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
 */

/* Dependencies */
#include <Wire.h>
#include "PCF8574.h"
#include "FunctionalInterrupt.h"

#ifdef PCF8574_INTERRUPT_SUPPORT
//#include "PCint.h"
#endif

PCF8574::PCF8574() :
		_PORT(0), _PIN(0), _DDR(0), _address(0)
#ifdef PCF8574_INTERRUPT_SUPPORT
		, _isrIgnore(0), _pcintPin(0)//, _oldPIN(0), _intMode(), _intCallback()
#endif
{
}

void PCF8574::begin(uint8_t address) {
	_address = address;
	Wire.begin();
	readGPIO();

	Serial.print(FPSTR("address="));
	Serial.print(_address);
	Serial.print(FPSTR(" pin values="));
	Serial.println(_PIN);
	Serial.println(FPSTR("-------------------------"));

}

void PCF8574::pinMode(uint8_t pin, uint8_t mode) {

	/* Switch according mode */
	switch (mode) {
	case INPUT:
		_DDR &= ~(1 << pin);
		_PORT &= ~(1 << pin);
		break;

	case INPUT_PULLUP:
		_DDR &= ~(1 << pin);
		_PORT |= (1 << pin);
		break;

	case OUTPUT:
		_DDR |= (1 << pin);
		_PORT &= ~(1 << pin);
		break;

	default:
		break;
	}

	/* Update GPIO values */
	updateGPIO();
}

void PCF8574::digitalWrite(uint8_t pin, uint8_t value) {
	if (value)
		_PORT |= (1 << pin);
	else
		_PORT &= ~(1 << pin);

	updateGPIO();
}

uint8_t PCF8574::digitalRead(uint8_t pin) {
	readGPIO();
	Serial.println();
	uint8_t res= (_PIN & (1 << pin)) ? HIGH : LOW;
	return res;
}

void PCF8574::write(uint8_t value) {
	_PORT = value;
	updateGPIO();
}

uint8_t PCF8574::read() {
	readGPIO();
	return _PIN;
}

void PCF8574::pullUp(uint8_t pin) {
	pinMode(pin, INPUT_PULLUP); // /!\ pinMode form THE LIBRARY
}

void PCF8574::pullDown(uint8_t pin) {
	pinMode(pin, INPUT); // /!\ pinMode form THE LIBRARY
}

void PCF8574::clear() {
	write(0x00);
}

void PCF8574::set() {
	write(0xFF);
}

void PCF8574::toggle(uint8_t pin) {
	_PORT ^= (1 << pin);
	updateGPIO();
}

void PCF8574::blink(uint8_t pin, uint16_t count, uint32_t duration) {
	duration /= count * 2;
	while (count--) {
		toggle(pin);
		delay(duration);
		toggle(pin);
		delay(duration);
	}
}

#ifdef PCF8574_INTERRUPT_SUPPORT

void PCF8574::enableInterrupt(uint8_t pin, std::function<void(void)> func){
	_pcintPin=pin;

	#if ARDUINO >= 100
		::pinMode(_pcintPin, INPUT_PULLUP); // /!\ pinMode form THE ARDUINO CORE
	#else
		::pinMode(pin, INPUT); // /!\ pinMode form THE ARDUINO CORE
		::digitalWrite(pin, HIGH); // /!\ digitalWrite form THE ARDUINO CORE
	#endif
	attachInterrupt(_pcintPin, func, FALLING);
}

#endif

void PCF8574::readGPIO() {
	Wire.requestFrom(_address, (uint8_t) 0x01);
	while (Wire.available() < 1)
		;
	_PIN = Wire.read();

	Serial.print("read PCF8574 val=");
	Serial.println(_PIN);
}

void PCF8574::updateGPIO() {

	//uint8_t value = ((_PIN & ~_DDR) & ~(~_DDR & _PORT)) | _PORT; // Experimental
	uint8_t value = (_PIN & ~_DDR) | _PORT;

	Wire.beginTransmission(_address);
	Wire.write(value);
	Wire.endTransmission();

	Serial.print("write PCF8574 val=");
	Serial.println(value);
}
