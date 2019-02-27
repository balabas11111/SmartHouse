/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_
#define LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_
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
#define _DEFAULT_AP_PASSWORD ""
#define _DEFAULT_AP_CHANNEL  1
#define _DEFAULT_AP_HIDDEN   0
#define _DEFAULT_AP_MAXCONN  4

#include <entity/model/Entity.h>
#include <IPAddress.h>
#include <sstream>

class ServerSettingsBox: public Entity {
public:
	ServerSettingsBox(const char* firmware):Entity("settings", "ServerSettings", "Wifi Server Device settings"){
		this->firmware=firmware;

		std::ostringstream sstream;
		sstream << "ESP_D_" << ESP.getChipId() ;
		this->devId=sstream.str().c_str();
		sstream.clear();
	};
	virtual ~ServerSettingsBox(){};

	void resetToAp(){
		this->setVar(_IS_AP, 1);
		this->setVar(_IS_STAT_IP, 1);
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

	const char* ssid(){ return getVal<const char*>(_SSID);}
	const char* password(){ return getVal<const char*>(_PASS);}

	const char* ssidAP(){ return deviceId();}
	const char* passwordAP(){ return _DEFAULT_AP_PASSWORD;}
	const int channelAP(){ return _DEFAULT_AP_CHANNEL;}
	const int hiddenAP(){ return _DEFAULT_AP_HIDDEN;}
	const int maxConnAP(){ return _DEFAULT_AP_MAXCONN;}

	bool isAccessPoint(){return getVal<int>(_IS_AP);}
	bool isStaticIp(){return getVal<int>(_IS_STAT_IP);}
	bool isDisconnectOnStartIfConn(){return getVal<int>(_DISCONNECT_ON_START_IF_CONN);}

	IPAddress IPstation(){return IPAddress().fromString(getVal<const char*>(_IP));}
	IPAddress IPaccessPoint(){return IPAddress().fromString(getVal<const char*>(_IP_AP));}
	IPAddress gateway(){return IPAddress().fromString(getVal<const char*>(_IP_GATEWAY));}
	IPAddress subnet(){return IPAddress().fromString(getVal<const char*>(_IP_SUBNET));}
	IPAddress dns(){return IPAddress().fromString(getVal<const char*>(_IP_DNS));}
	IPAddress dns2(){return IPAddress().fromString(getVal<const char*>(_IP_DNS2));}

	const char* userLogin(){ return getVal<const char*>(_USER_LOGIN);}
	const char* userPassword(){ return getVal<const char*>(_USER_PASS);}

	const char* adminLogin(){ return getVal<const char*>(_ADMIN_LOGIN);}
	const char* adminPassword(){ return getVal<const char*>(_ADMIN_PASS);}

	const char* deviceId(){ return devId;}
	const char* deviceFirmWare(){ return firmware;}
	const char* deviceDescr(){ return getVal<const char*>(_ADMIN_PASS);}

	virtual void initModel(){
		registerVariableField(_SSID,"balabasKiev5",true,true);
		registerVariableField(_PASS,"wuWylKegayg2wu22",true,true);

		registerStatField(_PASS_AP, _DEFAULT_AP_PASSWORD);

		registerVariableField(_IS_AP,0,true,true);
		registerVariableField(_IS_STAT_IP,0,true,true);
		registerVariableField(_DISCONNECT_ON_START_IF_CONN,0,true,true);

		registerVariableField(_IP,"192.168.0.120",true,true);
		registerVariableField(_IP_AP,"192.168.0.4",true,true);
		registerVariableField(_IP_GATEWAY,"192.168.0.1",true,true);
		registerVariableField(_IP_SUBNET,"255.255.255.0",true,true);
		registerVariableField(_IP_DNS,"192.168.0.1",true,true);
		registerVariableField(_IP_DNS2,"192.168.0.1",true,true);
		registerVariableField(_USER_LOGIN,"",true,true);
		registerVariableField(_USER_PASS,"",true,true);
		registerVariableField(_ADMIN_LOGIN,"admin",true,true);
		registerVariableField(_ADMIN_PASS,"admin",true,true);

		registerStatField(_DEVICE_ID, devId);
		registerStatField(_DEVICE_FIRMWARE, firmware);
		registerVariableField(_DEVICE_DESCR,"Default Device description",true,true);
	}
protected:
	const char* devId;
	const char* firmware;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */
