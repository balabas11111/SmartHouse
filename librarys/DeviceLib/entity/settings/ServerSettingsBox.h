/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_
#define LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_

#define SSID "ssid"
#define PASS "pass"
#define PORT "port"

#include "AbstractSettingsBox.h"

class ServerSettingsBox: public AbstractSettingsBox {
public:
	ServerSettingsBox(): AbstractSettingsBox("settings", "ServerSettings", "Wifi Server Device settings", serverConfig){};
	virtual ~ServerSettingsBox(){};

	const char* ssid(){ return getVar_char(SSID);}
	const char* password(){ return getVar_char(PASS);}

	const int port(){return getVar_int(PORT);}

protected:
	const char* serverConfig;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */
