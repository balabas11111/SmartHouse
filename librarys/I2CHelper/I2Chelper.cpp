/*
 * I2Chelper.cpp
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */
#include <I2Chelper.h>
#include "Wire.h"

	I2Chelper::I2Chelper(uint8_t _clockPin,uint8_t _dataPin,boolean _active){
		Serial.println("Create I2C Helper");

		devCount=0;
		sda=_dataPin;
		scl=_clockPin;

		active=false;

		if(_active){
			init();
		}
	}

	void I2Chelper::init(){
		if(!active){
			initWire();
			scan();

			active=true;
		}
	}

	void I2Chelper::initWire(){
		Serial.println("---------------");
		Serial.println("Setup I2C bus Wire lib...");
		Wire.begin(sda, scl);
	}

	String I2Chelper::I2Chelper::scan(){
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



