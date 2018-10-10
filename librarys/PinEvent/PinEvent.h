/*
 * PinEvent.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENT_PINEVENT_H_
#define LIBRARIES_PINEVENT_PINEVENT_H_

#include "Arduino.h"

#define PIN_EVENT_STATE_UPDATED "PE_SU"
#define PIN_EVENT_STATE_SET "PE_SS"
#define PIN_EVENT_STATE_GET "PE_SG"
#define PIN_EVENT_STATE_CURRENT "PE_SC"

class PinEvent{

public:
	PinEvent(String kind,boolean bubble,uint8_t pinId,uint8_t oldVal,uint8_t val,	String strVal,String dispatcherName);
	PinEvent(String pinEventText);

	String getKind();
	boolean isBubble();
	uint8_t getPinId();
	uint8_t getOldVal();
	uint8_t getVal();
	String getStrVal();
	String getDispatcherName();

	String getText();

	boolean isValid();
	boolean isEventOfKind(String kind);

private:

	String kind;
	boolean bubble;
	uint8_t pinId;
	uint8_t oldVal;
	uint8_t val;
	String strVal;
	String dispatcherName;

	boolean valid;

	boolean validate();

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
