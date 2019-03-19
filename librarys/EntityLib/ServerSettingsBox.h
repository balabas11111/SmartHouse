/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_WSERVERSETTINGSBOX_H_
#define LIBRARIES_ENTITYLIB_WSERVERSETTINGSBOX_H_
//network settings
#define _SSID "ssid"
#define _PASS "pass"

#define _SSID_AP "ssidAP"
#define _PASS_AP "passAP"

#define _IS_AP "isAp"
#define _IS_STAT_IP "isStatIp"
#define _DISCONNECT_ON_START_IF_CONN "disStIfCon"

#define _IP "ip"
#define _IP_AP "ipAP"
#define _IP_GATEWAY "gateway"
#define _IP_SUBNET "subnet"
#define _IP_DNS "dns"
#define _IP_DNS2 "dns2"
//access settings
#define _USER_LOGIN "userLogin"
#define _USER_PASS "userPass"
#define _ADMIN_LOGIN "adminLogin"
#define _ADMIN_PASS "adminPass"
//device info
#define _DEVICE_ID "deviceId"
#define _DEVICE_FIRMWARE "deviceFirmware"
#define _DEVICE_DESCR "deviceDescr"
// AP default settings
#define _AP_CHANNEL  "apCH"
#define _AP_HIDDEN   "apH"
#define _AP_MAXCONN  "apMC"
#define _DEFAULT_AP_PASSWORD ""
#define _DEFAULT_AP_CHANNEL  1
#define _DEFAULT_AP_HIDDEN   0
#define _DEFAULT_AP_MAXCONN  4

#define _DEFAULT_SERVER_DESCRIPTOR "{\"data\":{\
				\"ssid\":\"balabasKiev5\",\
				\"pass\":\"wuWylKegayg2wu22\",\
				\"passAp\":\"\",\
				\"isAp\":0,\
				\"isStatIp\":0,\
				\"disStIfCon\":0,\
				\"ip\":\"192.168.0.120\",\
				\"ipAP\":\"192.168.0.4\",\
				\"gateway\":\"192.168.0.1\",\
				\"subnet\":\"255.255.255.0\",\
				\"dns\":\"192.168.0.1\",\
				\"dns2\":\"192.168.0.1\",\
				\"userLogin\":\"\",\
				\"userPass\":\"\",\
				\"adminLogin\":\"admin\",\
				\"adminPass\":\"admin\",\
				\"deviceId\":\"devId\",\
				\"deviceFirmware\":\"firmware\",\
				\"deviceDescr\":\"Default Device description\"},\
				\"model\":{\"tvar\":[\"*\"]}}"

#include <EntityJson.h>
#include <IPAddress.h>
#include <sstream>
#include "ObjectUtils.h"

class ServerSettingsBox: public EntityJson {
public:
	ServerSettingsBox(const char* firmware):EntityJson("settings", "ServerSettings", "Wifi Server Device settings",_DEFAULT_SERVER_DESCRIPTOR){
		this->firmware=firmware;
	};
	virtual ~ServerSettingsBox(){};

	void resetToAp(){
		this->getModelDataProvider()->setField(id, _IS_AP, 1);
		this->getModelDataProvider()->setField(id, _IS_STAT_IP, 1);
	}

	void printApConf(){
		Serial.print(FPSTR(" ssid="));  Serial.print(ssidAP());
		Serial.print(FPSTR(" pass=")); Serial.print(passwordAP());
		Serial.print(FPSTR(" chann="));  Serial.print(channelAP());
		Serial.print(FPSTR(" hidden=")); Serial.print(hiddenAP());
		Serial.print(FPSTR(" maxCon=")); Serial.print(maxConnAP());
		Serial.print(FPSTR(" ipAp="));  Serial.print(IPaccessPoint());
		Serial.print(FPSTR(" ipStatic="));  Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));  Serial.print(gateway());
		Serial.print(FPSTR(" subnet="));  Serial.print(subnet());
		Serial.print(FPSTR(" dns="));  Serial.print(dns());
		Serial.print(FPSTR(" dns2="));  Serial.print(dns2());
		Serial.println();
	}

	void printStaConf(){
		Serial.print(FPSTR(" ssid="));  Serial.print(ssid());
		Serial.print(FPSTR(" pass=")); Serial.print(password());
		Serial.print(FPSTR(" ip="));  Serial.print(IPstation());
		Serial.print(FPSTR(" ipStatic="));  Serial.print(isStaticIp());
		Serial.print(FPSTR(" gateway="));  Serial.print(gateway());
		Serial.print(FPSTR(" subnet="));  Serial.print(subnet());
		Serial.print(FPSTR(" dns="));  Serial.print(dns());
		Serial.print(FPSTR(" dns2="));  Serial.print(dns2());
		Serial.println();
	}

	const char* ssid(){ return this->getModelDataProvider()->getFieldConstChar(id, _SSID);}
	const char* password(){ return this->getModelDataProvider()->getFieldConstChar(id, _PASS);}

	const char* ssidAP(){ return deviceId();}
	const char* passwordAP(){ return this->getModelDataProvider()->getFieldConstChar(id, _PASS_AP);}
	int channelAP(){ return this->getModelDataProvider()->getFieldInt(id, _AP_CHANNEL);}
	int hiddenAP(){ return this->getModelDataProvider()->getFieldInt(id, _AP_HIDDEN);}
	int maxConnAP(){ return this->getModelDataProvider()->getFieldInt(id, _AP_MAXCONN);}

	bool isAccessPoint(){return this->getModelDataProvider()->getFieldInt(id, _IS_AP);}
	bool isStaticIp(){return this->getModelDataProvider()->getFieldInt(id, _IS_STAT_IP);}
	bool isDisconnectOnStartIfConn(){return this->getModelDataProvider()->getFieldInt(id, _DISCONNECT_ON_START_IF_CONN);}

	IPAddress IPstation(){
		const char* ip=this->getModelDataProvider()->getFieldConstChar(id, _IP);
		return ObjectUtils::stringToIp(ip);
	}
	IPAddress IPaccessPoint(){
		const char* ip=this->getModelDataProvider()->getFieldConstChar(id, _IP_AP);
		return ObjectUtils::stringToIp(ip);
	}
	IPAddress gateway(){
		const char* ip=this->getModelDataProvider()->getFieldConstChar(id, _IP_GATEWAY);
		return ObjectUtils::stringToIp(ip);
	}
	IPAddress subnet(){
		const char* ip=this->getModelDataProvider()->getFieldConstChar(id, _IP_SUBNET);
		return ObjectUtils::stringToIp(ip);
	}
	IPAddress dns(){
		const char* ip=this->getModelDataProvider()->getFieldConstChar(id, _IP_DNS);
		return ObjectUtils::stringToIp(ip);
	}
	IPAddress dns2(){
		const char* ip=this->getModelDataProvider()->getFieldConstChar(id, _IP_DNS2);
		return ObjectUtils::stringToIp(ip);
	}

	const char* userLogin(){ return this->getModelDataProvider()->getFieldConstChar(id, _USER_LOGIN);}
	const char* userPassword(){ return this->getModelDataProvider()->getFieldConstChar(id, _USER_PASS);}

	const char* adminLogin(){ return this->getModelDataProvider()->getFieldConstChar(id, _ADMIN_LOGIN);}
	const char* adminPassword(){ return this->getModelDataProvider()->getFieldConstChar(id, _ADMIN_PASS);}

	const char* deviceId(){ return devId;}
	const char* deviceFirmWare(){ return firmware;}
	const char* deviceDescr(){ return this->getModelDataProvider()->getFieldConstChar(id, _ADMIN_PASS);}

	virtual void init() override{
		//String devIdTmp="ESP_"+String(ESP.getChipId());
		//this->devId=strdup(devIdTmp.c_str());
		this->devId="ESP_Dev_ID";
	}

	virtual void postModelDataInit() override{
		this->getModelDataProvider()->setField(id, _DEVICE_ID,devId);
		this->getModelDataProvider()->setField(id, _DEVICE_FIRMWARE,firmware);
	}

protected:
	const char* devId;
	const char* firmware;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */
