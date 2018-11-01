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
	I2Chelper(uint8_t _clockPin,uint8_t _dataPin,boolean _active){
		Serial.println("Create I2C Helper");

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
		Serial.println("Begin initialize of I2CHelper");
		if(init){
			initWire();
			scan();
		}

		initialized=init;

		return init;
	}

	String scan(){
			Serial.println("---------------");
			Serial.println("Setup I2C bus...");
			  byte error, address;
			  int nDevices;
			  String result="";

			  Serial.println("Scanning I2C...");

			  nDevices = 0;
			  for(address = 1; address < 127; address++ )
			  {

				Wire.beginTransmission(address);
				error = Wire.endTransmission();

				if (error == 0)
				{
				  Serial.print("I2C device found at address 0x");
				  if (address<16){
					Serial.print("0");
					result.concat("0");
				  }
				  Serial.print(address,HEX);

				  result.concat(String(address));
				  result.concat(";");

				  Serial.println("  !");

				  nDevices++;
				}
				else if (error==4)
				{
				  Serial.print("Unknow error at address 0x");
				  if (address<16)
					Serial.print("0");
				  Serial.println(address,HEX);
				}
			  }

			  result.concat(" count="+String(nDevices));

			  if (nDevices == 0)
				Serial.println("No I2C devices found\n");
			  else
				Serial.println("done\n");
			  Serial.println("---------------");

			  devCount=nDevices;

			  return result;
		}

private:
	uint8_t sda;
	uint8_t scl;
	uint8_t devCount;

	void initWire(){
		Serial.println("---------------");
		Serial.println("Setup I2C bus Wire lib...");
		Wire.begin(sda, scl);
	}
};


#endif /* LIBRARIES_I2CHELPER_I2CHELPER_H_ */
