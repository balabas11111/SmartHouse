/*
 * Bme280sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_WS2811LED_H_
#define LIBRARIES_ENTITYLIBSENSORS_WS2811LED_H_

#include <Adafruit_NeoPixel.h>

//--------------------------------

class WS2811Led {
public:
	WS2811Led(uint8_t pin, uint8_t ledCount =1) {
		this->pin = pin;
		this->ledCount = ledCount;

		this->strip = new Adafruit_NeoPixel(this->ledCount, this->pin, NEO_GRB + NEO_KHZ400);
		strip->begin();
		strip->show();
	}

	virtual ~WS2811Led() {
	}

	void setRed(uint16_t i = 0) {
		//Serial.println(FPSTR("led  red"));
		setColor(i, strip->Color(255, 0, 0));
	}

	void setGreen(uint16_t i = 0) {
		//Serial.println(FPSTR("led  green"));
		setColor(i, strip->Color(0, 255, 0));
	}

	void setBlue(uint16_t i = 0) {
		//Serial.println(FPSTR("led  blue"));
		setColor(i, strip->Color(0, 0, 255));
	}

	void setYellow(uint16_t i = 0) {
		setColor(i, strip->Color(255, 255, 0));
	}

	void setVoid(uint16_t i = 0) {
		setColor(i, strip->Color(0, 0, 0));
	}

protected:
	Adafruit_NeoPixel* strip;
	uint8_t pin;
	uint8_t ledCount;
private:
	void setColor(uint16_t i, uint32_t c) {
		strip->setPixelColor(i, c);
		strip->show();
	}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_BME280SENSOR_H_ */
