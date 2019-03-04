/*
 * I2Chelper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_I2C_utils_I2C_utils_H_
#define LIBRARIES_I2C_utils_I2C_utils_H_

#include <Arduino.h>
#include "ConstUtil.h"

#include "Wire.h"

class I2C_utils{

public:
	/*
	I2C device found at address 0x23  BH1750
	I2C device found at address 0x76  BME280
	I2C device found at address 0x27  PCF8574_for_LCD
	*/

	I2C_utils(bool init=false){
		if(init){
			initDefault();
		}
	}

	virtual ~I2C_utils(){};

	static uint8_t initDefault(){
		return initStatic(SCL,SDA);
	}

	static uint8_t initStatic(uint8_t clockPin,uint8_t dataPin){
		Wire.begin(dataPin, clockPin);
		return scan();
	}

	static uint8_t scan(){
		Serial.println(FPSTR("----------------------------------"));
		Serial.println(FPSTR("I2C scanning..."));
		  byte error, address;
		  int nDevices;

		  nDevices = 0;
		  for(address = 1; address < 127; address++ )
		  {

			Wire.beginTransmission(address);
			error = Wire.endTransmission();

			if (error == 0)
			{
			  Serial.print(FPSTR("got address="));
			  if (address<16){
				Serial.print(0);
			  }
			  Serial.println(address,HEX);

			  nDevices++;
			}
			else if (error==4)
			{


			   Serial.print(FPSTR("ERROR"));
			  if (address<16)
				Serial.print(0);
			  Serial.println(address,HEX);

			}
		  }

		  if (nDevices == 0){
			Serial.println(FPSTR("NO I2C devices found"));
		  }else{
			  Serial.print(FPSTR("Total ="));
			  Serial.println(nDevices);
		  }
		  Serial.println(FPSTR("----------------------------------"));

		  return nDevices;
	}
};


#endif /* LIBRARIES_I2CHELPER_I2CHELPER_H_ */
