/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_ABSTRACTEVENT_H_
#define LIBRARIES_TIMETRIGGER_ABSTRACTEVENT_H_

#define EVENT_SEPARATOR ":"

#include "Arduino.h"

class AbstractEvent{

public:
	//PIN_SC:01:001:onOffEvent:Button1   PIN_SC:pinID:pinstate:someStrCommand:dispatcher

	AbstractEvent(String eventPreffix,uint8_t pin,uint8_t intCOmmand,String strCommand,String dispatcher);
	AbstractEvent(String eventText);

	boolean isEventOfKind(String eventKind);

	String getPreffix();
	uint8_t getPin();
	uint8_t getIntCommand();
	String getStrCommand();
	String getDispatcher();
	void setDispatcher(String dispatcher);
	String getEventText();
	String print();
	boolean isValid();

private:
	String _eventText;
	String _eventPreffix;
	uint8_t _pin;
	uint8_t _intCOmmand;
	String _strCommand;
	String _dispatcher;
	boolean _valid;

	boolean validateValues();
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
