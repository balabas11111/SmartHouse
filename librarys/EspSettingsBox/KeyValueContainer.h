/*
 * KeyValueContainer
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_KEYVALUECONTAINER_H_
#define LIBRARIES_TIMETRIGGER_KEYVALUECONTAINER_H_

#include "Arduino.h"
#include "KeyValue.h"

#define MAX_SIZE 12

class KeyValueContainer{

public:
	KeyValueContainer(String fileName);
	boolean fileExists();
	boolean loadFromFile();
	boolean saveToFile(boolean recreate);
	int getIndexByKey(String key);
	KeyValue getKeyValueByIndex(uint8_t index);
	KeyValue getKeyValue(String key);
	String getValue(String key);
	void updateKeyValue(String key,String value);
	void putKeyValue(KeyValue keyValue);
	void putKeyValueDetails(String key,String value,uint8_t kind);
	uint8_t getMapLength();
	void printAllKeyValues();
private:
	uint8_t _keyValueLength;
	uint8_t _keyValuePosition;
	KeyValue _keyValueArray[MAX_SIZE];
	String _fileName;
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
