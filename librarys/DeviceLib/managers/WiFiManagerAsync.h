/*
 * WiFiManagerAsync.h
 *
 *  Created on: Feb 25, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_MANAGERS_WIFIMANAGERASYNC_H_
#define LIBRARIES_DEVICELIB_MANAGERS_WIFIMANAGERASYNC_H_

#include <entity/settings/ServerSettingsBox.h>

class WiFiManagerAsync {
public:
	WiFiManagerAsync(ServerSettingsBox* conf){
		this->conf=conf;
	}
	virtual ~WiFiManagerAsync(){};

	void init();
	bool isConnected();

protected:
	void initWiFi();

	void waitForConnect();

	void startAsAP();
	void startAsSta();

	void startWebServer();

	void deployTemplates();
	void deployStaticFiles();

	ServerSettingsBox* conf;
};

#endif /* LIBRARIES_DEVICELIB_MANAGERS_WIFIMANAGERASYNC_H_ */
