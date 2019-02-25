/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_
#define LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_
//network settings
#define SSID "ssid"
#define PASS "pass"

#define IS_AP "isAp"
#define IS_STAT_IP "isStatIp"

#define IP "ip"
#define IP_AP "ipAP"
#define IP_GATEWAY "gateway"
#define IP_SUBNET "subnet"
#define IP_DNS "dns"
#define IP_DNS2 "dns2"
//access settings
#define USER_LOGIN "userLogin"
#define USER_PASS "userPass"
#define ADMIN_LOGIN "adminLogin"
#define ADMIN_PASS "adminPass"
//device info
#define DEVICE_ID "deviceId"
#define DEVICE_FIRMWARE "deviceFirmware"
#define DEVICE_DESCR "deviceDescr"

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

	const char* ssid(){ return getVal<const char*>(SSID);}
	const char* password(){ return getVal<const char*>(PASS);}

	bool isAccessPoint(){return getVal<int>(IS_AP);}
	bool isStaticIp(){return getVal<int>(IS_STAT_IP);}

	IPAddress ipStation(){return IPAddress().fromString(getVal<const char*>(IP));}
	IPAddress ipAccessPoint(){return IPAddress().fromString(getVal<const char*>(IP_AP));}
	IPAddress ipGateway(){return IPAddress().fromString(getVal<const char*>(IP_GATEWAY));}
	IPAddress ipSubnet(){return IPAddress().fromString(getVal<const char*>(IP_SUBNET));}
	IPAddress ipDns(){return IPAddress().fromString(getVal<const char*>(IP_DNS));}
	IPAddress ipDns2(){return IPAddress().fromString(getVal<const char*>(IP_DNS2));}

	const char* userLogin(){ return getVal<const char*>(USER_LOGIN);}
	const char* userPassword(){ return getVal<const char*>(USER_PASS);}

	const char* adminLogin(){ return getVal<const char*>(ADMIN_LOGIN);}
	const char* adminPassword(){ return getVal<const char*>(ADMIN_PASS);}

	const char* deviceId(){ return devId;}
	const char* deviceFirmWare(){ return firmware;}
	const char* deviceDescr(){ return getVal<const char*>(ADMIN_PASS);}

	virtual void initModel(){
		registerVariableField(SSID,"balabasKiev5");
		registerVariableField(PASS,"wuWylKegayg2wu22");
		registerVariableField(IS_AP,0);
		registerVariableField(IS_STAT_IP,0);

		registerVariableField(IP,"192.168.0.120");
		registerVariableField(IP_AP,"192.168.0.4");
		registerVariableField(IP_GATEWAY,"192.168.0.1");
		registerVariableField(IP_SUBNET,"255.255.255.0");
		registerVariableField(IP_DNS,"192.168.0.1");
		registerVariableField(IP_DNS2,"192.168.0.1");
		registerVariableField(USER_LOGIN,"");
		registerVariableField(USER_PASS,"");
		registerVariableField(ADMIN_LOGIN,"admin");
		registerVariableField(ADMIN_PASS,"admin");

		registerStatField(DEVICE_ID, devId);
		registerStatField(DEVICE_FIRMWARE, firmware);

		registerVariableField(DEVICE_DESCR,"Default Device description");

		registerField_loadable(Key_coordinate);

		registerField_saveable(Key_coordinate);
	}
protected:
	const char* devId;
	const char* firmware;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */
