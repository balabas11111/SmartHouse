/*
 * Url_Registerer.cpp
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <EntityLib/Url_Registerer.h>

Url_Registerer::Url_Registerer(ESP8266WebServer* server) {
	this->server=server;
}

Url_Registerer::~Url_Registerer() {}

void Url_Registerer::on(const String& uri, HTTPMethod method,
		ESP8266WebServer::THandlerFunction fn) {
	server->on(uri, method,fn);
}

boolean Url_Registerer::hasParameter(String name) {
	return server->hasArg(name);
}

String Url_Registerer::getParameter(String name) {
	return server->arg(name);
}

void Url_Registerer::send(int code, const String& content_type,
		const String& content) {
	server->send(code, content_type, content);
}
