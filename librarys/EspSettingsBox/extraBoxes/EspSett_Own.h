/*
 * EspSett_Own.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_Own_H_
#define LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_Own_H_


#include "Arduino.h"
#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"

const char OWN_SETT_BOX_DESCR[]         PROGMEM ="Настройки OWN";

enum OWN_SETT_BOX_KEYS_ID: uint8_t {
	OWN_accessUser, OWN_accessPass, OWN_settingsUser, OWN_settingsPass, OWN_password, OWN_refreshInterval,
	OWN_isAccesPoint, OWN_ssidAP, OWN_ssid, OWN_staticIp, OWN_localIp, OWN_apIp, OWN_gateIp, OWN_subnetIp, OWN_dnsIp, OWN_dnsIp2
};

const char* const OWN_SETT_BOX_KEYS_NAME[]          PROGMEM=
{
	"accessUser",
	"accessPass",
	"settingsUser",
	"settingsPass",

	"password",
	"refreshInterval",
	"isAccesPoint",
	"ssidAP",

	"ssid",
	"staticIp",
	"localIp",
	"apIp",
	"gateIp",
	"subnetIp",
	"dnsIp",
	"dnsIp2"
};

const char* const OWN_SETT_BOX_DEFAULTS[]      PROGMEM=
{
	"",
	"",
	"balabas",
	"balabas",
	"wuWylKegayg2wu22",
	"60",
	"false",
	"SENS_",  //+String(ESP.getChipId());;"

	"balabasKiev5",
	"false",
	"192.168.0.120",
	"192.168.4.1",
	"192.168.0.1",
	"255.255.255.0",
	"192.168.0.1",
	"192.168.0.1"
};

class EspSett_Own:public ESPExtraSettingsBox {
public:
	EspSett_Own(){
		this->keySize=ARRAY_SIZE(OWN_SETT_BOX_KEYS_NAME);;
	}
	virtual ~EspSett_Own(){};

	virtual int getId() override{
		return ExtraBox_Own;
	}

	virtual String getName() override{
		return FPSTR(OWN_SETTINGS_BOX_NAME);
	}

	virtual boolean isInMemory() override{
		return false;
	}

	virtual String getDefaultValue(int index) override{
		if(index==OWN_ssidAP){
			String result="SENS_";
					result+=String(ESP.getChipId());
			return result;
		}
		return String(getDefaults()[index]);
	}

	virtual boolean setInternalValueByIndex(int index,String value){
		/*switch(index){
			case OWN_accessUser:{
				accessUser=value;
				return true;
			}
			case OWN_accessPass:{
				accessPass=value;
				return true;
			} case OWN_settingsUser:{
				settingsUser=value;;
				return true;
			} case OWN_settingsPass:{
				settingsPass=value;;
				return true;
			} case OWN_password:{
				password=value;;
				return true;
			} case OWN_refreshInterval:{
				refreshInterval=value;;
				return true;
			}
			case OWN_isAccesPoint:{
				isAccesPoint=value;
				return true;
			}
			case OWN_ssidAP:{
				ssidAP=value;
				return true;
			}
			case OWN_ssid:{
				ssid=value;
				return true;
			}
			case OWN_staticIp:{
				staticIp=value;
				return true;
			}
			case OWN_localIp:{
				localIp=value;
				return true;
			}
			case OWN_apIp:{
				apIp=value;
				return true;
			}
			case OWN_gateIp:{
				gateIp=value;
				return true;
			}
			case OWN_subnetIp:{
				return subnetIp;
				return true;
			}
			case OWN_dnsIp:{
				return dnsIp;
				return true;
			}
			case OWN_dnsIp2:{
				return dnsIp2;
				return true;
			}

		}
*/
		vals[index]=value;
		return true;
	}

	virtual String getInternalValueByIndex(int index,String value){
	/*	switch(index){
			case OWN_accessUser:{
				return accessUser;
				break;
			}
			case OWN_accessPass:{
				return accessPass;
				break;
			} case OWN_settingsUser:{
				return settingsUser;
				break;
			} case OWN_settingsPass:{
				return settingsPass;
				break;
			} case OWN_password:{
				return password;
				break;
			} case OWN_refreshInterval:{
				return refreshInterval;
				break;
			}
			case OWN_isAccesPoint:{
				return isAccesPoint;
				break;
			}
			case OWN_ssidAP:{
				return ssidAP;
				break;
			}
			case OWN_ssid:{
				return ssid;
				break;
			}
			case OWN_staticIp:{
				return staticIp;
				break;
			}
			case OWN_localIp:{
				return localIp;
				break;
			}
			case OWN_apIp:{
				return apIp;
				break;
			}
			case OWN_gateIp:{
				return gateIp;
				break;
			}
			case OWN_subnetIp:{
				return subnetIp;
				break;
			}
			case OWN_dnsIp:{
				return dnsIp;
				break;
			}
			case OWN_dnsIp2:{
				return dnsIp2;
				break;
			}

		}
		*/
		return vals[index];
	}

	virtual const char* const* getDefaults() override{
		return OWN_SETT_BOX_DEFAULTS;
	}
	virtual const char* const* getKeys() override{
		return OWN_SETT_BOX_KEYS_NAME;
	}
	virtual String getDescription() override{
		return FPSTR(OWN_SETT_BOX_DESCR);
	}
	String getKind() override{
		return FPSTR(SETTINGS_KIND_publish);
	}
/*private:
	String accessUser = "";
	String accessPass = "";

	String settingsUser = "balabas";
	String settingsPass = "balabas";

	String password = "wuWylKegayg2wu22";//wuWylKegayg2wu22

	uint16_t refreshInterval=60;

	boolean isAccesPoint=false;
	#ifdef ESP8266
		String ssidAP="SENS_"+String(ESP.getChipId());;
	#endif
	#ifdef ESP32
		String ssidAP="SENS_"+String(ESP.getChipRevision());;
	#endif

	String ssid = "balabasKiev5";//balabasKiev5
	boolean staticIp=false;
	IPAddress localIp=IPAddress(192, 168, 0, 120);
	IPAddress apIp=IPAddress(192, 168, 4, 1);
	IPAddress gateIp=IPAddress(192, 168, 0, 1);
	IPAddress subnetIp=IPAddress(255, 255, 255, 0);
	IPAddress dnsIp=IPAddress(192, 168, 0, 1);
	IPAddress dnsIp2=IPAddress(192, 168, 0, 1);
		if(fieldName==FPSTR(ESBOX_refreshInterval)){
		long val=fieldValue.toInt();
		if(refreshInterval!=val){
			refreshInterval=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessUser)){
		if(accessUser!=fieldValue){
			accessUser=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessPass)){
		if(fieldValue!=FPSTR(ESBOX_STARS) && accessPass!=fieldValue){
			accessPass=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_accessPassConfirm)
		|| fieldName==FPSTR(ESBOX_settingsPassConfirm)){
		return true;
	}
	if(fieldName==FPSTR(ESBOX_settingsUser)){
		if(settingsUser!=fieldValue){
			settingsUser=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_settingsPass)){
		if(fieldValue!=FPSTR(ESBOX_STARS) && settingsPass!=fieldValue){
			settingsPass=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_isAccesPoint)){
		boolean val=EspSettingsUtil::stringToBoolean(fieldValue);
		if(isAccesPoint!=val){
			isAccesPoint=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_ssidAP)){
		if(ssidAP!=fieldValue){
			ssidAP=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_ssid)){
		if(ssid!=fieldValue){
			ssid=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_password)){
		if(fieldValue!=FPSTR(ESBOX_STARS) && password!=fieldValue){
			password=fieldValue;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_passwordConfirm)){
		return true;
	}
	if(fieldName==FPSTR(ESBOX_staticIp)){
		boolean val=EspSettingsUtil::stringToBoolean(fieldValue);
		if(staticIp!=val){
			staticIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_localIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(localIp!=val){
			localIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_apIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(apIp!=val){
			apIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_gateIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(gateIp!=val){
			gateIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_subnetIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(subnetIp!=val){
			subnetIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_dnsIp)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(dnsIp!=val){
			dnsIp=val;
			saveRequired=true;
		}
		return true;
	}
	if(fieldName==FPSTR(ESBOX_dnsIp2)){
		IPAddress val=EspSettingsUtil::stringToIp(fieldValue);
		if(dnsIp2!=val){
			dnsIp2=val;
			saveRequired=true;
		}
		return true;
	}

	*/
};

#endif /* LIBRARIES_ESPSETTINGSBOX_EXTRABOXES_ESPSETT_Own_H_ */
