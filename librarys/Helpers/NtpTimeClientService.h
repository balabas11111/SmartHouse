/*
 * NtpTimeClientService.h
 *
 *  Created on: 28 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_
#define LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_

#include "Initializable.h"
#include "Loopable.h"
#include "EspSettingsBox.h"
#include "ESP_Consts.h"
#include "TimeTrigger.h"
#include "WiFiUdp.h"
#include "NTPClient.h"
#include "FunctionalInterrupt.h"

class NtpTimeClientService: public Initializable, public Loopable {
public:

	NtpTimeClientService(EspSettingsBox* espSettingsBox,std::function<void(int8_t*)> _externalFunction, ulong externalFunctionInterval){
			Serial.println(FPSTR("Create time client"));

			this->timeClient=new NTPClient(ntpUDP, espSettingsBox->NTP_poolServerName.c_str(), espSettingsBox->NTP_timeOffset, espSettingsBox->NTP_updateInterval);
			this->timeClientTrigger=new TimeTrigger(0,1000,false,[this](){processTimeClient();});
			this->externalFunction=_externalFunction;
			this->externalFunctionInterval=externalFunctionInterval;
		}

	NtpTimeClientService(EspSettingsBox* espSettingsBox,std::function<void(int8_t*)> _externalFunction){
		Serial.println("Init time client");

		timeClient=new NTPClient(ntpUDP, espSettingsBox->NTP_poolServerName.c_str(), espSettingsBox->NTP_timeOffset, espSettingsBox->NTP_updateInterval);
		timeClientTrigger=new TimeTrigger(0,espSettingsBox->NTP_timeTriggerInterval,false,[this](){processTimeClient();});
		externalFunction=_externalFunction;
	}

	~NtpTimeClientService(){};

	virtual boolean initialize(boolean _init){
		if(_init){

			Serial.println("-----Init Ntp Client-----");
			timeClient->begin();
			timeClient->forceUpdate();
			timeClientTrigger->init();

			Serial.println(FPSTR("TimeUpdateInterval="));
			Serial.println(externalFunctionInterval);

			Serial.print(" currentTime= ");
			Serial.println(getCurrentTimeAsString('.'));
		}
		initialized=_init;

		return initialized;
	}

	virtual boolean loop(){
		if(initialized){
			timeClientTrigger->loop();
			timeClient->update();
			return true;
		}

		return false;
	}

	virtual void displayDetails(){
		Serial.println(getCurrentTimeAsString(':'));
	}

	boolean updateCurrentTime(){
		boolean update=false;

		uint8_t h=timeClient->getHours();
		uint8_t m=timeClient->getMinutes();
		uint8_t s=timeClient->getSeconds();

		int8_t tmp[6] = {0x00,0x00,0x00,0x00,0x00,0x00};

		tmp[0] = h / 10;
		tmp[1] = h % 10;
		tmp[2] = m / 10;
		tmp[3] = m % 10;
		tmp[4] = s / 10;
		tmp[5] = s % 10;

		for(uint8_t i=0;i<6;i++){
			if(tmp[i]!=currentTime[i]){
				update=true;
				currentTime[i]=tmp[i];
			}
		}

		if(!updated && update){
			timeClientTrigger->setInterval(externalFunctionInterval);
			updated=true;

			Serial.print(FPSTR("-First Time received new interval="));
			Serial.println(timeClientTrigger->getInterval());
		}

		return updated;
	}

	ulong getCurrentTimeAsLong(){
		return timeClient->getEpochTime();
	}

	String getCurrentTimeAsString(const char divider){
		String result=String(currentTime[0])
				+String(currentTime[1])
				+String(divider)
				+String(currentTime[2])
				+String(currentTime[3])
				+String(divider)
				+String(currentTime[4])
				+String(currentTime[5]);

		return result;
	}

	String getCurrentTimeNoSecondsAsString(const char divider){
			String result=String(currentTime[0])
					+String(currentTime[1])
					+String(divider)
					+String(currentTime[2])
					+String(currentTime[3]);

			return result;
		}

	int8_t* getCurrentTime(){
		return currentTime;
	}

private:
	WiFiUDP ntpUDP;
	int8_t currentTime[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	NTPClient* timeClient;
	TimeTrigger* timeClientTrigger;
	std::function<void(int8_t*)> externalFunction;
	ulong externalFunctionInterval;
	boolean updated=false;


	void processTimeClient(){
		updateCurrentTime();

		if(updated && externalFunction!=nullptr){
			externalFunction(currentTime);
		}
	}
};

#endif /* LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_ */
