/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "AbstractEvent.h"

//PIN_SC:01:001:onOffEvent:Button1   PIN_SC:pinID:pinstate:someStrCommand:dispatcher

AbstractEvent::AbstractEvent(String eventPreffix,uint8_t pin,uint8_t intCOmmand,String strCommand,String dispatcher){
	_eventPreffix=eventPreffix;
	_pin=pin;
	_intCOmmand=intCOmmand;
	_strCommand=strCommand;
	_dispatcher=dispatcher;

	_eventText=_eventPreffix+":"+_pin+":"+_intCOmmand+":"+_strCommand+":"+_dispatcher;

	_valid=validateValues();

	Serial.println("Event parsed "+print());
}

AbstractEvent::AbstractEvent(String eventText){

	Serial.println("Construct event "+eventText);

	uint8_t len=4;

	int ind[len];

	ind[0]=eventText.indexOf(":");
	ind[1]=(ind[0]>0)?eventText.indexOf(":",ind[0]+1):-1;
	ind[2]=(ind[1]>0)?eventText.indexOf(":",ind[1]+1):-1;
	ind[3]=(ind[2]>0)?eventText.indexOf(":",ind[2]+1):-1;

	Serial.println(" len ind="+String(len));
	boolean valid=(len==4);

	for(uint8_t i=0;i<len;i++){
		Serial.print(" ind["+String(i)+"]="+String(ind[i]));
		valid=valid & (ind[i]!=-1);
	}

	Serial.println();
	Serial.println("valid="+String(valid));

	_eventText=eventText;
	_eventPreffix=eventText.substring(0,ind[0]);
	_pin=eventText.substring(ind[0]+1,ind[1]).toInt();
	_intCOmmand=eventText.substring(ind[1]+1,ind[2]).toInt();
	_strCommand=eventText.substring(ind[2]+1,ind[3]);
	_dispatcher=eventText.substring(ind[3]+1);

	_valid=valid & validateValues();

	Serial.println("Event parsed "+print());
}

boolean AbstractEvent::isEventOfKind(String kind){
	return getPreffix().equals(kind);
}

String AbstractEvent::getPreffix(){
	return _eventPreffix;
}
uint8_t AbstractEvent::getPin(){
	return _pin;
}
uint8_t AbstractEvent::getIntCommand(){
	return _intCOmmand;
}
String AbstractEvent::getStrCommand(){
	return _strCommand;
}
String AbstractEvent::getDispatcher(){
	return _dispatcher;
}

void AbstractEvent::setDispatcher(String dispatcher){
	_dispatcher=dispatcher;
}

String AbstractEvent::getEventText(){
	return _eventText;
}

String AbstractEvent::print(){
return "_eventPreffix="+_eventPreffix+"_pin="+_pin+" _intCOmmand="+_intCOmmand+" _strCommand="+_strCommand+" _dispatcher="+_dispatcher+" _valid="+String(isValid()) ;
}

boolean AbstractEvent::isValid(){
	return _valid;
}

boolean AbstractEvent::validateValues(){
	boolean result=_eventText.length()!=0 & _eventPreffix.length()!=0 & _pin>-2 & _intCOmmand>-2 & _strCommand.length()>0
					& _dispatcher.length()>0;

	return result;
}



