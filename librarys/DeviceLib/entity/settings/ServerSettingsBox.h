/*
 * ServerSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_
#define LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_

#include "AbstractSettingsBox.h"

class ServerSettingsBox: public AbstractSettingsBox {
public:
	ServerSettingsBox(): AbstractSettingsBox("settings", "ServerSettings", "Wifi Server Device settings", jsonConfig){};
	virtual ~ServerSettingsBox();


protected:
	const char* jsonConfig;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_SERVERSETTINGSBOX_H_ */
