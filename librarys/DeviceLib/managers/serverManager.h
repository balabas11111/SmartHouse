/*
 * serverManager.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_MANAGERS_SERVERMANAGER_H_
#define LIBRARIES_DEVICELIB_MANAGERS_SERVERMANAGER_H_

#include <Arduino.h>

#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

class serverManager {
public:
	serverManager();
	virtual ~serverManager();

protected:

};

#endif /* LIBRARIES_DEVICELIB_MANAGERS_SERVERMANAGER_H_ */
