/*
 * ButtonB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "KeyValueContainer.h"

#include "FS.h"


KeyValueContainer::KeyValueContainer(String fileName){
	_fileName=fileName;
	_keyValuePosition=0;
	_keyValueLength=MAX_SIZE;
};
boolean KeyValueContainer::fileExists(){
	boolean fileExists=SPIFFS.exists(_fileName);

	return fileExists;
};

boolean KeyValueContainer::loadFromFile(){
	File f = SPIFFS.open(_fileName, "r");
	Serial.println("File exists. Reading it");

	boolean result=false;

	  if (f) {		    // we could open the file
		while(f.available()) {
			delay(1);
		  String line = f.readStringUntil('\n');
		  if(line.length()>3){
			  KeyValue kv=KeyValue();
			  kv.fromLine(true, line);
			  putKeyValue(kv);
		  }
		}

		f.close();
		result=true;
	  }

	  return result;
};
boolean KeyValueContainer::saveToFile(boolean recreate){

	boolean executeSave=false;
	boolean fe=fileExists();

	Serial.print("file=");
	Serial.print(_fileName);
	Serial.print("; exists=");
	Serial.print(fe);
	Serial.print("; recreate=");
	Serial.println(recreate);

	if(!recreate){
		executeSave=!fe;
	}else{
		if(fe){
			bool result=SPIFFS.remove(_fileName);

			Serial.print("file=");
			Serial.print(_fileName);
			Serial.print("; removed=");
			Serial.println(result);

		}

		executeSave=true;
	}

	if (executeSave) {
	    Serial.println("save to file to be executed");

	    File f = SPIFFS.open(_fileName, "w");
	    if (!f) {
	      Serial.println("file creation failed");
	    }

	    uint8_t len=getMapLength();

	    for(uint8_t i=0;i<len;i++){
	    	if(_keyValueArray[i].isValid()){
				String str=_keyValueArray[i].getAsLine();
				Serial.print("-----------------------");
				Serial.println(str);
				Serial.println("-----------------------");

				f.println(str);
	    	}
	    }

	    f.close();
	  } else {
		  Serial.println("Save to file is not requested");
	  }

	 Serial.println("--------------------");

	 return executeSave;
};
int KeyValueContainer::getIndexByKey(String key){
	for(uint8_t i=0;i<_keyValuePosition;i++){
				if(key==_keyValueArray[i].getKey()){
					return i;
				}
			}

		return -1;
};

KeyValue KeyValueContainer::getKeyValueByIndex(uint8_t index){
	return _keyValueArray[index];
};

void KeyValueContainer::printAllKeyValues(){
	Serial.println("--------------------");
	for(uint8_t i=0;i<_keyValuePosition;i++){
		Serial.println(_keyValueArray[i].getAsLine());
	}
	Serial.println("--------------------");
}

KeyValue KeyValueContainer::getKeyValue(String key){
	for(uint8_t i=0;i<_keyValuePosition;i++){

	/*
			Serial.print("key=");
			Serial.print(key);
			Serial.print(" keyArray[i]=");
			Serial.print(keyArray[i]);
			Serial.println(";");
	*/
			if(key==_keyValueArray[i].getKey()){
				KeyValue keyValue=_keyValueArray[i];

				//keyValue.printToSerial("got ", i);

				return keyValue;
			}
		}

		Serial.print("key=");
		Serial.print(key);
		Serial.println(" not found");

		return KeyValue();

};
String KeyValueContainer::getValue(String key){
	return getKeyValue(key).getValue();
};
void KeyValueContainer::updateKeyValue(String key,String value){
	int index=getIndexByKey(key);
	if(index!=-1){
		_keyValuePosition++;

		_keyValueArray[index].setValue(value);
	}else{
		Serial.println("Invalid key =");
		Serial.println(key);
	}
};

void KeyValueContainer::putKeyValueDetails(String key,String value,uint8_t kind){
	KeyValue keyValue=KeyValue(key,value,kind);
	putKeyValue(keyValue);
};
uint8_t KeyValueContainer::getMapLength(){
	return _keyValuePosition;
};

void KeyValueContainer::putKeyValue(KeyValue keyValue){
	if(keyValue.isValid()){
		int index=getIndexByKey(keyValue.getKey());
		if(index==-1){
			index=_keyValuePosition;
			_keyValuePosition++;
			_keyValueArray[index]=keyValue;
		}else{
			//_keyValueArray[index].setValue(keyValue.getValue());
			_keyValueArray[index]=keyValue;
		}

	}else{
		Serial.println(keyValue.getAsLine());
		Serial.println("Invalid key value");
	}
};




