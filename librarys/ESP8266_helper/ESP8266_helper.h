/*
 * ESP8266_helper.h
 *
 *  Created on: 14 џэт. 2018 у.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESP8266_HELPER_ESP8266_HELPER_H_
#define LIBRARIES_ESP8266_HELPER_ESP8266_HELPER_H_

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "LedB.h"

class ESP8266_helper{
	public:
		ESP8266_helper(SoftwareSerial* softwareSerial,uint8_t baudRate,
				boolean doReset,uint8_t cwMode,String NetworkSSID,String NetworkPASS,
				uint8_t cipMode,uint8_t cipMux,uint8_t cipServerMode,uint8_t cipServerPort,
				uint8_t cipSto,LedB* statusLed);
		String executeAtCommand(String AT_Command, int wait);
		SoftwareSerial getSerialPort();
	private:
		void connectWiFiCommand(String NetworkSSID,String NetworkPASS);
		SoftwareSerial _softwareSerial;
};



#endif /* LIBRARIES_ESP8266_HELPER_ESP8266_HELPER_H_ */
