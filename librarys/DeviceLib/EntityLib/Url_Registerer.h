/*
 * Url_Registerer.h
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYLIB_URL_REGISTERER_H_
#define LIBRARIES_DEVICELIB_ENTITYLIB_URL_REGISTERER_H_

#include "ESP8266WebServer.h"

class Url_Registerer {
public:
	Url_Registerer(ESP8266WebServer* server);
	virtual ~Url_Registerer();

	void on(const String &uri, HTTPMethod method, ESP8266WebServer::THandlerFunction fn);
	void send(int code, const String& content_type, const String& content);

	boolean hasParameter(String name);
	String getParameter(String name);

private:
	ESP8266WebServer* server;
};

#endif /* LIBRARIES_DEVICELIB_ENTITYLIB_URL_REGISTERER_H_ */
