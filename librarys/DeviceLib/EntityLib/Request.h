/*
 * Request.h
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYLIB_REQUEST_H_
#define LIBRARIES_DEVICELIB_ENTITYLIB_REQUEST_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include <AbstractSensor.h>

enum RequestKind: uint8_t {
	Request_Kind, Response_Kind,Response_MessageIn,Response_MessageOut
};

enum RequestStatus: uint8_t{
	RequestStatus_ReceivedByDevice,RequestStatus_ProcessedByDevice,RequestStatus_SentToClient,RequestFailed
};

class Request{
public:
	Request();
	virtual ~Request();

	virtual uint16_t httpStatus();

	virtual Request buildFromString(String str);

	virtual String toString();

	virtual String execute();
};

#endif /* LIBRARIES_DEVICELIB_ENTITYLIB_REQUEST_H_ */
