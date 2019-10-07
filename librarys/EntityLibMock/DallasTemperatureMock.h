/*
 * DallasTemperatureMock.h
 *
 *  Created on: Mar 26, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_DALLASTEMPERATUREMOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_DALLASTEMPERATUREMOCK_H_

#include <DeviceUtils.h>
#include "Arduino.h"
#include "AMock.h"
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
	String getAddress(DeviceAddress deviceAddress,uint8_t index){
		UNUSED(deviceAddress);
		uint8_t addrLoc[8]={0,0,0,0,0,0,0,index};

		/*Serial.println(FPSTR("getAddress"));
		DeviceUtils::printInt8Arr(addr);
		*/
		return "0000000"+String(index);
	}
	void requestTemperatures(){
		//Serial.println(FPSTR("DallasTemperatureMock->requestTemperatures"));
	}
	float getTempCByIndex(int i){
		return 20+random(0, 20);
	}

protected:
	uint8_t* addr[8];
	OneWireMock* owm;
	int deviceC=3;

};

#endif /* LIBRARIES_ENTITYLIBMOCK_DALLASTEMPERATUREMOCK_H_ */
