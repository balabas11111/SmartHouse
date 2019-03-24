/*
 * NtpSettingsBox.h
 *
 *  Created on: 24 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_NTPBOX_H_
#define LIBRARIES_ENTITYLIB_NTPBOX_H_

#include <EntityJson.h>
#include "UpdateAble.h"
#include "ObjectUtils.h"

#include "WiFiUdp.h"
#include "NTPClient.h"

#define FIELD_ENABLED  "enabled"
#define FIELD_SERVER   "server"
#define FIELD_OFFSET   "offset"
#define FIELD_INTERVAL "interval"
#define FIELD_timeUnix "timeUnix"

#define _DEFAULT_NTP_DESCRIPTOR "{\"data\":{\
				\"enabled\":1,\
				\"server\":\"europe.pool.ntp.org\",\
				\"offset\":7200,\
				\"interval\":240,\
				\"timeUnix\":0},\
				\"model\":{\"tvar\":[\"*\"],\"set\":[\"server\",\"offset\",\"interval\"]}}"

class NtpBox: public EntityJson, public UpdateAble {
public:
	NtpBox()
		:EntityJson(ROOT_GROUP_SETTINGS, "NtpBox", "NTP time Device settings",_DEFAULT_NTP_DESCRIPTOR){};
	virtual ~NtpBox(){};

	bool enabled(){
		 return this->getModelDataProvider()->getFieldInt(id, FIELD_ENABLED);
	}
	const char* server(){
		return this->getModelDataProvider()->getFieldConstChar(id, FIELD_SERVER);
	}
	int offset(){
		return this->getModelDataProvider()->getFieldInt(id, FIELD_OFFSET);
	}
	int interval(){
		return this->getModelDataProvider()->getFieldInt(id, FIELD_INTERVAL);
	}
	long timeUnix(){
		parseUnixTimeToCache();
		return this->getModelDataProvider()->getFieldLong(id, FIELD_timeUnix);
	}
	int8_t* timeHm(){
		parseTimeToCache();
		return currTimePrepared;
	}

	void startTime(){
		Serial.print(FPSTR("NTP Box init"));
		if(!enabled()){
			return;
		}

		this->timeClient=new NTPClient(ntpUDP,server(),offset(),interval()*1000);
		this->timeClient->begin();
		bool result=timeClient->forceUpdate();

		this->ok=true;

		if(result){
			onNtpTimeReceived();
		}
		Serial.println(FPSTR("...done"));
	}

	void onNtpTimeReceived(){
		timeClient->end();
		Serial.println(FPSTR("On Ntp received"));
	}

	virtual void update() override{
		if(!isOk()){
			return;
		}
		bool result=timeClient->update();
		if(timeClient->udpStarted()){
			onNtpTimeReceived();
		}
		if(result){
			parseUnixTimeToCache();
			parseTimeToCache();
		}
		Serial.print(FPSTR("time = "));
		for(int i=0;i<6;i++){
			Serial.print(currTimePrepared[i]);
			if(i==1 || i==3){
				Serial.print(FPSTR(":"));
			}
		}
		Serial.println();

	}

	bool isOk(){
		return ok;
	}
protected:
	WiFiUDP ntpUDP;
	NTPClient* timeClient;
	bool ok = false;
	int8_t currTimePrepared[6] = {0x00,0x00,0x00,0x00,0x00,0x00};

	void parseUnixTimeToCache(){
		ulong timeUnix=timeClient->getEpochTime();
		this->getModelDataProvider()->setField(id, FIELD_timeUnix,timeUnix);
	}
	void parseTimeToCache(){
		timeClient->update();
		uint8_t h=timeClient->getHours();
		uint8_t m=timeClient->getMinutes();
		uint8_t s=timeClient->getSeconds();

		currTimePrepared[0] = h / 10;
		currTimePrepared[1] = h % 10;
		currTimePrepared[2] = m / 10;
		currTimePrepared[3] = m % 10;
		currTimePrepared[4] = s / 10;
		currTimePrepared[5] = s % 10;
	}
};

#endif /* LIBRARIES_ENTITYLIB_NTPSETTINGSBOX_H_ */
