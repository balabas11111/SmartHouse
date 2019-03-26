/*
 * DallasTemperatureMock.h
 *
 *  Created on: Mar 26, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_DALLASTEMPERATUREMOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_DALLASTEMPERATUREMOCK_H_

#include "Arduino.h"
#include "AMock.h"
#include "ObjectUtils.h"

#include "OneWireMock.h"
#include "DallasTemperature.h"


class DallasTemperatureMock: public AMock {
public:
	DallasTemperatureMock(OneWireMock* oneWire){
		owm = oneWire;
	};
	virtual ~DallasTemperatureMock(){};

	int getDeviceCount(){
		return this->deviceC;
	}
	bool getAddress(DeviceAddress deviceAddress,uint8_t index){
		UNUSED(deviceAddress);
		uint8_t addr[8]={0,0,0,0,0,0,0,index};
		deviceAddress=addr;
		Serial.println(FPSTR("getAddress"));
		ObjectUtils::printInt8Arr(addr);
		return true;
	}
	void requestTemperatures(){
		Serial.println(FPSTR("DallasTemperatureMock->requestTemperatures"));
	}
	float getTempCByIndex(int i){
		return 1+i;
	}
protected:
	OneWireMock* owm;
	int deviceC=3;
};

#endif /* LIBRARIES_ENTITYLIBMOCK_DALLASTEMPERATUREMOCK_H_ */
