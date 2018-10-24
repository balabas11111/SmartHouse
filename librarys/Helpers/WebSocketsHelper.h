/*
 * WebSocketHelper.h
 *
 *  Created on: Oct 24, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_WEBSOCKETSHELPER_H_
#define LIBRARIES_HELPERS_WEBSOCKETSHELPER_H_

#include "WebSocketsServer.h"
#include <Initializable.h>
#include <Loopable.h>

class WebSocketsHelper: public Initializable, public Loopable {
public:

	WebSocketsHelper(WebSocketsServer* _webSocket,std::function<void(uint8_t num, WStype_t type, uint8_t * payload, size_t length)> _funcEvent){
		webSocket=_webSocket;
		funcEvent=_funcEvent;
	}
	virtual ~WebSocketsHelper();

	virtual boolean initialize(boolean _init){
		if(_init){
			webSocket->begin();
			webSocket->onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length){
				if(funcEvent!=nullptr)
					funcEvent(num,type,payload,length);
			});
		}

		initialized=_init;
		return initialized;
	};

	virtual boolean loop(){
		webSocket->loop();
		return initialized;
	}

	virtual String displayDetails(){
	   Serial.println("-----------Web socket details-----");
	   int clientsCount=(webSocket->connectedClients(true));
	   Serial.print("clients=");Serial.println(clientsCount);

	   for(uint8_t i=0;i<clientsCount;i++){
		   Serial.print("=");Serial.println(webSocket->remoteIP(i));
	   }

	   Serial.println("-----------------------------------");

	   return "";
	}

	boolean sendMessageToAll(String message){
		Serial.print("sendMessages ");
		Serial.print(message);
		return webSocket->broadcastTXT(message);
	}


private:

	std::function<void(uint8_t num, WStype_t type, uint8_t * payload, size_t length)> funcEvent;

	WebSocketsServer* webSocket;

};


#endif /* LIBRARIES_HELPERS_WEBSOCKETHELPER_H_ */
