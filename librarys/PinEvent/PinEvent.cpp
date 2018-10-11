/*
 * PinEvent.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#include "PinEvent.h"

PinEvent::PinEvent(String _kind,boolean _bubble,uint8_t _pinId,uint8_t _oldVal,uint8_t _val,String _strVal,String _dispatcherName){
	kind=_kind;
	bubble=_bubble;
	pinId=_pinId;
	oldVal=_oldVal;
	val=_val;
	strVal=_strVal;
	dispatcherName=_dispatcherName;

	valid=validate();
}


//kind:bubble:pinId:oldVal:val:strVal:dispatcherName:targetName:
PinEvent::PinEvent(String _pinEventText){
	Serial.println("Construct event "+_pinEventText);

	uint8_t len=7;

	int ind[len];

	ind[0]=_pinEventText.indexOf(":");
	ind[1]=(ind[0]>0)?_pinEventText.indexOf(":",ind[0]+1):-1;
	ind[2]=(ind[1]>0)?_pinEventText.indexOf(":",ind[1]+1):-1;
	ind[3]=(ind[2]>0)?_pinEventText.indexOf(":",ind[2]+1):-1;
	ind[4]=(ind[3]>0)?_pinEventText.indexOf(":",ind[3]+1):-1;
	ind[5]=(ind[4]>0)?_pinEventText.indexOf(":",ind[4]+1):-1;
	ind[6]=(ind[5]>0)?_pinEventText.indexOf(":",ind[5]+1):-1;

	boolean _valid=true;

	for(uint8_t i=0;i<len;i++){
		Serial.print(" ind["+String(i)+"]="+String(ind[i]));
		valid=valid & (ind[i]!=-1);
	}

	kind=_pinEventText.substring(0,ind[0]);
	bubble=_pinEventText.substring(ind[0]+1,ind[1]).toInt();
	pinId=_pinEventText.substring(ind[1]+1,ind[2]).toInt();
	oldVal=(_pinEventText.substring(ind[2]+1,ind[3])).toInt();
	val=(_pinEventText.substring(ind[3]+1)).toInt();
	strVal=_pinEventText.substring(ind[4]+1);
	dispatcherName=_pinEventText.substring(ind[5]+1);
	targetName=_pinEventText.substring(ind[6]+1);

	valid=_valid & validate();
}

String PinEvent::getKind(){
	return kind;
}
boolean PinEvent::isBubble(){
	return bubble;
}
uint8_t PinEvent::getPinId(){
	return pinId;
}
uint8_t PinEvent::getOldVal(){
	return oldVal;
}
uint8_t PinEvent::getVal(){
	return val;
}
String PinEvent::getStrVal(){
	return strVal;
}
String PinEvent::getDispatcherName(){
	return dispatcherName;
}
String PinEvent::getTargetName(){
	return targetName;
}

String PinEvent::getText(){
	return kind+":"+String(bubble)+":"+String(pinId)+":"+String(oldVal)+":"+String(val)+":"+strVal+":"+dispatcherName+":";
}

boolean PinEvent::isValid(){
	return valid;
}
boolean PinEvent::isEventOfKind(String _kind){
	return kind.equals(_kind);
}

boolean PinEvent::validate(){
	return kind.length()>0 && (pinId>-2) && strVal.length()>0 && dispatcherName.length()>0;
}


