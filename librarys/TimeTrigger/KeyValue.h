/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_KEYVALUE_H_
#define LIBRARIES_TIMETRIGGER_KEYVALUE_H_

#include "Arduino.h"

class KeyValue{

public:
	KeyValue();
	KeyValue(String key,String value,int8_t kind);
	void fromLine(boolean removeLastSymbol,String line);
	boolean isValid();
	void setKey(String key);
	void setValue(String value);
	void setKind(int8_t kind);
	void updateKind(int8_t kind);
	String getKey();
	String getValue();
	int8_t getKind();
	String getAsLine();
	String printToSerial(String preffix,uint8_t index);
private:
	String _key;
	String _value;
	int8_t _kind;
	boolean _isValid;
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
