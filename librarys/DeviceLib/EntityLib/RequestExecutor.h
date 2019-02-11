/*
 * RequestExecutor.h
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITYLIB_REQUESTEXECUTOR_H_
#define LIBRARIES_DEVICELIB_ENTITYLIB_REQUESTEXECUTOR_H_

#include <Arduino.h>
#include <EntityLib/Request.h>

#include <interfaces/Identifiable.h>
#include <interfaces/Nameable.h>

class RequestExecutor: public Identifiable, public Nameable {
public:
	RequestExecutor();
	virtual ~RequestExecutor();

	virtual uint8_t getId();

	virtual Request executePageRequest(uint8_t pageId,String json);
};

#endif /* LIBRARIES_DEVICELIB_ENTITYLIB_REQUESTEXECUTOR_H_ */
