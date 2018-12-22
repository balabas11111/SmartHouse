/*
 * I2Chelper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_I2CHELPER_I2CHELPER_H_
#define LIBRARIES_I2CHELPER_I2CHELPER_H_

#include <Arduino.h>
#include "Initializable.h"
#include "Wire.h"

class I2Chelper: public Initializable{

public:
	/*
	I2C device found at address 0x23  BH1750
	I2C device found at address 0x76  BME280
	I2C device found at address 0x27  PCF8574_for_LCD
	*/

	I2Chelper(uint8_t _clockPin,uint8_t _dataPin,boolean _active){

		devCount=0;
		sda=_dataPin;
		scl=_clockPin;

		initialize(_active);
	}

	virtual ~I2Chelper(){};

	virtual bool init(){
		return initialize(true);
	}

	virtual boolean initialize(boolean init){
		Serial.println(FPSTR(MESSAGE_I2CHELPER_BEGIN_INIT));
		if(init){
			initWire();
			scan();
		}

		initialized=init;

		return init;
	}

	void scan(){
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		Serial.println(FPSTR(MESSAGE_I2CHELPER_SETUP_I2C_BUS));
		  byte error, address;
		  int nDevices;

		  Serial.println(FPSTR(MESSAGE_I2CHELPER_SCANNING));

		  nDevices = 0;
		  for(address = 1; address < 127; address++ )
		  {

			Wire.beginTransmission(address);
			error = Wire.endTransmission();

			if (error == 0)
			{
			  Serial.print(FPSTR(MESSAGE_I2CHELPER_DEVICE_FOUND));
			  if (address<16){
				Serial.print(0);
			  }
			  Serial.println(address,HEX);

			  nDevices++;
			}
			else if (error==4)
			{


			   Serial.print(FPSTR(MESSAGE_I2CHELPER_UNKNOWN_ERROR));
			  if (address<16)
				Serial.print(0);
			  Serial.println(address,HEX);

			}
		  }

		  if (nDevices == 0){
			Serial.println(FPSTR(MESSAGE_I2CHELPER_NO_DEVICES_FOUND));
		  }else{
			  Serial.print(FPSTR("Total devices="));
			  Serial.println(nDevices);
		  }
		  Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		  devCount=nDevices;
	}

	uint8_t getDevCount(){
		return devCount;
	}

private:
	uint8_t sda;
	uint8_t scl;
	uint8_t devCount;

	void initWire(){
		Wire.begin(sda, scl);
	}
};


#endif /* LIBRARIES_I2CHELPER_I2CHELPER_H_ */
