/*
 * ESP8266_helper.cpp
 *
 *  Created on: 14 џэт. 2018 у.
 *      Author: Vitaliy
 */
#include "ESP8266_helper.h"

ESP8266_helper::ESP8266_helper(SoftwareSerial *softwareSerial,uint8_t baudRate,
				boolean doReset,uint8_t cwMode,String NetworkSSID,String NetworkPASS,
				uint8_t cipMode,uint8_t cipMux,uint8_t cipServerMode,uint8_t cipServerPort,
				uint8_t cipSto,LedB* statusLed){

	_softwareSerial=&softwareSerial;

	statusLed->turnOn();
	_softwareSerial.begin(baudRate);
		statusLed->changeStateForDefaultInterval();
		//Serial.println(GetResponse("AT+IPR=9600",3400));
		Serial.println("RESET 3,5 sek");
		statusLed->changeStateForDefaultInterval();
		executeAtCommand("AT+RST",3400);
		statusLed->changeStateForDefaultInterval();
		Serial.println("--AT version");
		executeAtCommand("AT+GMR",3400);
		statusLed->changeStateForDefaultInterval();
		//Serial.println(GetResponse("AT+CIOBAUD=9600",3400));
		//Serial.println(GetResponse("AT+CIOBAUD=?",3400));
		Serial.println("baud rate");
		statusLed->changeStateForDefaultInterval();
		//Serial.println(GetResponse("AT+UART?",1500));
		//Serial.println(GetResponse("AT+IPR=9600",3400));
		//Serial.println(GetResponse("AT+CIOBAUD=?",3400));
		//-->//Serial.println(GetResponse("AT+UART=9600,8,1,0,0",3400));
		//Serial.println(GetResponse("AT+UART=9600,8,1,0,0",3400));

		executeAtCommand("AT+CWMODE=1",300);
		statusLed->changeStateForDefaultInterval();
		connectWiFiCommand("balabasKiev5","wuWylKegayg2wu22");
		statusLed->changeStateForDefaultInterval();
		executeAtCommand("AT+CIPMODE=0",300);
		statusLed->changeStateForDefaultInterval();
		executeAtCommand("AT+CIPMUX=1",300);
		statusLed->changeStateForDefaultInterval();
		Serial.print("Start TCP-server: ");
		executeAtCommand("AT+CIPSERVER=1,88", 300);
		statusLed->changeStateForDefaultInterval();
		executeAtCommand("AT+CIPSTO=5", 300);
		statusLed->changeStateForDefaultInterval();
		executeAtCommand("AT+CIFSR", 300);
		statusLed->changeStateForDefaultInterval();
		executeAtCommand("AT+CIPSTATUS", 300);
		statusLed->changeStateForDefaultInterval();
		Serial.println("-----");
		statusLed->changeStateForDefaultInterval();

		statusLed->turnOff();
};

SoftwareSerial ESP8266_helper::getSerialPort(){
	return _softwareSerial;
};

String ESP8266_helper::executeAtCommand(String AT_Command, int wait){
	 Serial.print("Executed=");
	  Serial.println(AT_Command);

	  String tmpData;

	  _softwareSerial.println(AT_Command);
	  delay(wait);
	  while (_softwareSerial.available() >0 )
	   {
	    char c = _softwareSerial.read();
	    tmpData += c;

	    if ( tmpData.indexOf(AT_Command) > -1 )
	      tmpData = "";
	    else
	      tmpData.trim();

	   }
	  Serial.print("result=");
	  Serial.println(tmpData);
	  Serial.println("---------------------------------------------------");

	  return tmpData;
};

void ESP8266_helper::connectWiFiCommand(String NetworkSSID,String NetworkPASS){
	 String cmd = "AT+CWJAP=\"";
	  cmd += NetworkSSID;
	  cmd += "\",\"";
	  cmd += NetworkPASS;
	  cmd += "\"";
	  //Serial.println(cmd);
	  executeAtCommand(cmd,6500);
};




