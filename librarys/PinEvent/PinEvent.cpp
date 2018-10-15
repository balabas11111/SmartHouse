/*
 * PinEvent.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#include "PinEvent.h"

PinEvent::PinEvent(){
	valid=false;
	notEmpty=false;
}

PinEvent::PinEvent(String _kind,boolean _bubble,uint8_t _pinId,uint16_t _oldVal,uint16_t _val,String _strVal,String _dispatcherName,String _targetName){
	kind=_kind;
	bubble=_bubble;
	pinId=_pinId;
	oldVal=_oldVal;
	val=_val;
	strVal=_strVal;
	dispatcherName=_dispatcherName;
	targetName=_targetName;

	valid=validate();
	notEmpty=true;
}


//kind:bubble:pinId:oldVal:val:strVal:dispatcherName:targetName:
PinEvent::PinEvent(String _pinEventText){
	//Serial.print("Con event text="+_pinEventText);

	uint8_t len=9;

	int ind[len];
	String strs[len];

	ind[0]=_pinEventText.indexOf(":");
	strs[0]=_pinEventText.substring(0,ind[0]);
	boolean _valid=(ind[0]!=-1);

	//Serial.print(" ind["+String(0)+"]="+String(ind[0])+" = "+strs[0]+" str="+_pinEventText.substring(0,ind[0]));

	for(uint8_t i=1;i<len;i++){
		ind[i]=(ind[i-1]>0)?_pinEventText.indexOf(":",ind[i-1]+1):-1;
		String str=_pinEventText.substring(ind[i-1]+1,ind[i]);
		strs[i]=_pinEventText.substring(ind[i-1]+1,ind[i]);
		valid=valid & (ind[i]!=-1);

		//Serial.print(" ind["+String(i)+"]="+String(ind[i])+" = "+strs[i]+" str="+str);
	}

	kind=strs[0];
	bubble=strs[1].toInt();
	pinId=strs[2].toInt();
	oldVal=strs[3].toInt();
	val=strs[4].toInt();
	strVal=strs[5];
	dispatcherName=strs[6];
	targetName=strs[7];
	sendDevice=strs[8];

	valid=_valid & validate();
	notEmpty=true;

	Serial.println("String->PinEvent("+getText()+")");

/*
	ind[1]=(ind[0]>0)?_pinEventText.indexOf(":",ind[0]+1):-1;
	ind[2]=(ind[1]>0)?_pinEventText.indexOf(":",ind[1]+1):-1;
	ind[3]=(ind[2]>0)?_pinEventText.indexOf(":",ind[2]+1):-1;
	ind[4]=(ind[3]>0)?_pinEventText.indexOf(":",ind[3]+1):-1;
	ind[5]=(ind[4]>0)?_pinEventText.indexOf(":",ind[4]+1):-1;
	ind[6]=(ind[5]>0)?_pinEventText.indexOf(":",ind[5]+1):-1;
	ind[7]=(ind[6]>0)?_pinEventText.indexOf(":",ind[6]+1):-1;

	kind=_pinEventText.substring(0,ind[0]);
	bubble=_pinEventText.substring(ind[0]+1,ind[1]).toInt();
	pinId=_pinEventText.substring(ind[1]+1,ind[2]).toInt();
	oldVal=(_pinEventText.substring(ind[2]+1,ind[3])).toInt();
	val=(_pinEventText.substring(ind[3]+1,ind[4])).toInt();
	strVal=_pinEventText.substring(ind[4]+1,ind[5]);
	dispatcherName=_pinEventText.substring(ind[5]+1,ind[6]);
	targetName=_pinEventText.substring(ind[6]+1);
*/
}

PinEvent PinEvent::setIsBubble(boolean _bubble){
	bubble=_bubble;
	return *(this);
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
uint16_t PinEvent::getOldVal(){
	return oldVal;
}
uint16_t PinEvent::getVal(){
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
	return kind+":"+String(bubble)+":"+String(pinId)+":"+String(oldVal)+":"+String(val)+":"+strVal+":"+dispatcherName+":"+targetName+":"+sendDevice+":"+String(valid)+":";
}

boolean PinEvent::isValid(){
	return valid;
}
boolean PinEvent::isEventOfKind(String _kind){
	return kind.equals(_kind);
}

PinEvent PinEvent::setIsNotEmpty(boolean _isNotEmpty){
	notEmpty=_isNotEmpty;
	return *(this);
}

boolean PinEvent::isNotEmpty(){
	return notEmpty;
}

boolean PinEvent::validate(){
	return kind.length()>0 && strVal.length()>0 && dispatcherName.length()>0;
}

void PinEvent::setSendDevice(String _sendDevice){
	sendDevice=_sendDevice;
}


