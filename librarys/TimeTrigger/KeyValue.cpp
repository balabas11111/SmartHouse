/*
 * ButtonB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "KeyValue.h"
const char DIVIDER='=';
const char DIVIDER2=';';

KeyValue::KeyValue(){
	_key="";
	_value="";
	_isValid=false;
	_kind=0;
};

KeyValue::KeyValue(String key,String value,int8_t kind){
	_key=key;
	_value=value;
	_kind=kind;
	isValid();
};

void KeyValue::fromLine(boolean removeLastSymbol,String line){

	if(removeLastSymbol){
	int nli=line.indexOf('\n');
		int endIndex=line.length()-1;

		if(nli>0){
			//Serial.println("Next line found");
			line=line.substring(0,nli-1);
		}else{
			line=line.substring(0, endIndex);
			//Serial.println("Next line not found");
		}
	}
	int ind=line.indexOf(DIVIDER);
	int ind1=line.indexOf(DIVIDER2);

	if(ind!=0){
		String key=line.substring(0,ind);
		String val=line.substring(ind+1,ind1);
		uint8_t kind=(line.substring(ind1+1)).toInt();

		/*Serial.print("key=");
		Serial.print(key);
		Serial.print("; val=");
		Serial.println(val);
*/
		_key=key;
		_value=val;
		_kind=kind;
		_isValid=(_key && _value && _key.length()!=0 && _value.length()!=0);

	}else{
		_isValid=false;
	}

};
boolean KeyValue::isValid(){
	_isValid=(_key && _key.length()!=0);
	return _isValid;
};
void KeyValue::setKey(String key){
	_key=key;
};
void KeyValue::setValue(String value){
	_value=value;
};

String KeyValue::getKey(){
	return _key;
};
String KeyValue::getValue(){
	return _value;
};
int8_t KeyValue::getKind(){
	return _kind;
}
String KeyValue::getAsLine(){
	return _key+DIVIDER+_value+DIVIDER2+String(_kind);
};
String KeyValue::printToSerial(String preffix,uint8_t index){
	String result=preffix+getAsLine()+"; ind="+String(index)+";";

	Serial.println(result);

	return result;
}
void KeyValue::updateKind(int8_t kind){
	_kind=kind;

}




