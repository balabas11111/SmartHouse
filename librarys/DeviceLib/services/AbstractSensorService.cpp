/*
 * AbstractSensorService.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <services/AbstractSensorService.h>

#include <ArduinoJson.h>
#include <utils/FileUtils.h>
#include <FS.h>

AbstractSensorService::AbstractSensorService(AbstractSensor** sensors,uint8_t sensorsSize) {
	this->sensors=sensors;
	this->sensorsCount=sensorsSize;
}

AbstractSensorService::~AbstractSensorService(){}

AbstractSensor** AbstractSensorService::getSensors(){
	return sensors;
}

uint8_t AbstractSensorService::getSensorsCount() {
	return sensorsCount;
}

AbstractSensor* AbstractSensorService::getSensorById(uint8_t sensorId){
	if(getSensorsCount()<1 || sensorId>getSensorsCount()-1){
		return NULL;
	}

	return sensors[sensorId];
}

AbstractSensor* AbstractSensorService::getSensorByName(String sensorName){
	if(getSensorsCount()<1){
		return NULL;
	}

	for(uint8_t i=0;i<getSensorsCount();i++){
		if(sensors[i]->getName()==sensorName){
			return sensors[i];
		}
	}

	return NULL;
}

boolean AbstractSensorService::loadSensors() {
	return loadSensorSettingsFromFile(sensors, sensorsCount);
}

boolean AbstractSensorService::saveSensors() {
	return saveSensorSettingsToFile(sensors, sensorsCount);
}

boolean AbstractSensorService::loadSensorSettingsFromFile(AbstractSensor** sensors,uint8_t size) {
	boolean result=true;

	for(uint8_t i=0;i<size;i++){
		Serial.println(sensors[i]->getName());

		result=loadAbstractSensorFromFile(sensors[i]) && result;

		Serial.print(FPSTR("LOADED Sensor="));
		Serial.println(sensors[i]->getJson());
	}

	return result;
}

boolean AbstractSensorService::saveSensorSettingsToFile(AbstractSensor** sensors,uint8_t size) {
	boolean result=true;
	for(uint8_t i=0;i<size;i++){
		result=saveAbstractSensorToFile(sensors[i]) && result;
	}
	return result;
}

StatusMessage AbstractSensorService::putAbstractSensorFromJson(String json,boolean setDescr,boolean setVal,boolean save){
	long start=millis();

	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	if(!root.success()){
		Serial.println(FPSTR("Error parse JSOn"));
		return StatusMessage(STATUS_PARSE_ERROR_INT);
	}
	if(!root.containsKey(FIELD_name)){
		Serial.println(FPSTR("Field name misses in request"));
		return StatusMessage(STATUS_BAD_PARAMS_ERROR_INT);
	}

	AbstractSensor* sensor;

	if(root.containsKey(FIELD_id)){
		uint8_t sId=root[FIELD_id];
		sensor=getSensorById(sId);

		if(sensor==NULL || sensor->getName()!=root[FIELD_name]){
			sensor=getSensorByName(root[FIELD_name]);
		}
	}else{
		sensor=getSensorByName(root[FIELD_name]);
	}

	if(sensor==NULL){
		return StatusMessage(STATUS_NOT_FOUND_INT);
	}

	if(setDescr && root.containsKey(FIELD_descr)){
		sensor->setDescr(root[FIELD_descr]);
	}

	if(setVal && root.containsKey(FIELD_val)){
		sensor->setVal(root[FIELD_val]);
	}

	JsonArray& items=root[FIELD_FIELDS];

	for(uint8_t i=0;i<items.size();i++){

		JsonObject& item=items[i];

		int id=item[FIELD_id];
		String name=item[FIELD_name];

		if(!sensor->isItemIndexValid(id)){
			return StatusMessage(STATUS_BAD_PARAMS_ERROR_INT);
		}

		if(sensor->getName(id)!=name){
			id=sensor->getChildItemIndexByName(name);

			if(id==-1){
				Serial.println(FPSTR("Field name misses in request"));
				return StatusMessage(STATUS_BAD_PARAMS_ERROR_INT);
			}
		}

		if(setDescr){
			sensor->setDescr(id, item[FIELD_descr]);
			sensor->setMinVal(id, item[FIELD_minVal]);
			sensor->setMaxVal(id, item[FIELD_minVal]);
			sensor->setFieldId(id, item[FIELD_fieldId]);
		}

		if(setVal && item.containsKey(FIELD_val)){
			sensor->setVal(item[FIELD_val]);
		}
	}

	if(save){
		saveAbstractSensorToFile(sensor);
	}

	return StatusMessage(STATUS_OK_SAVED_INT);
}

boolean AbstractSensorService::loadAbstractSensorFromFile(AbstractSensor* sensor) {
	if(sensor==NULL){
		Serial.println(FPSTR("Sensor == NULL load impossible"));
		return false;
	}
	String fileName=getAbstractSensorFilePath(sensor);

	File file = SPIFFS.open(fileName, "r");

	if(!file){
		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED));
		saveAbstractSensorToFile(sensor);
		file = SPIFFS.open(fileName, "r");
	}

	  if (!file){
		  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS));
		  return false;
	  } else {
		size_t size = file.size();
		if ( size == 0 ) {
		  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_EMPTY));
		} else {
			DynamicJsonBuffer jsonBuffer;
			delay(1);

			std::unique_ptr<char[]> buf (new char[size]);
			file.readBytes(buf.get(), size);

			JsonObject& root = jsonBuffer.parseObject(buf.get());
			JsonArray& items = root["items"];

			if (!root.success()) {
				Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
			} else {

				sensor->setDescr(root["descr"].as<char*>());

				uint8_t itemCountJson=root["itemCount"];

				if(sensor->getItemCount()>0 && itemCountJson>0){
					for(uint8_t i=0;i<itemCountJson;i++){

						int8_t index=sensor->getChildItemIndexByName(items[i]["name"]);

						if(index>-1){
							sensor->setDescr(index, items[i]["descr"]);
							sensor->setFieldId(index, items[i]["fieldId"]);
							sensor->setMinVal(index, items[i]["minVal"]);
							sensor->setMaxVal(index, items[i]["maxVal"]);
						}else{
							sensor->setNonActiveSensorValue(
									items[i]["name"],
									items[i]["descr"],
									items[i]["fieldId"],
									items[i]["minVal"],
									items[i]["maxVal"]
							);
						}

					}
				}
			}
		}
	  }

	  return true;
}

boolean AbstractSensorService::saveAbstractSensorToFile(AbstractSensor* sensor) {
	String fileName=getAbstractSensorFilePath(sensor);

	return FileUtils::saveStringToFile(fileName, sensor->getJsonForSave());
}

String AbstractSensorService::getAbstractSensorFilePath(AbstractSensor* sensor) {
	return getAbstractSensorFilePreffix(sensor)+FPSTR(TXT);
}

String AbstractSensorService::getAbstractSensorFileFolder(AbstractSensor* sensor) {
	return getAbstractSensorFilePreffix(sensor)+FPSTR(SLASH);
}

void AbstractSensorService::updateSensors(){

	for(uint8_t i=0;i<sensorsCount;i++){
			sensors[i]->update();
			sensors[i]->checkForAlarm();
	}

	Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));Serial.println(ESP.getFreeHeap());
}

String AbstractSensorService::getAbstractSensorFilePreffix(AbstractSensor* sensor){
	return ((sensor->getPath()!=NULL)?sensor->getPath():FPSTR(ABSTRACT_SENSORS_BASE_PATH)) + sensor->getName();
}

void AbstractSensorService::printAbstractSensorServiceDetails(){
	Serial.print(FPSTR("AbstractSensorService sensors="));
	Serial.println(sensorsCount);
}



int AbstractSensorService::getAbstractItems(JsonArray& items, uint8_t pageId) {
	Serial.print(FPSTR("AbstractSensorService::getAbstractItems pageId="));
	Serial.print(pageId);
	Serial.print(FPSTR(" pageName="));
	Serial.println(PAGE_NAME[pageId]);
	long start=millis();

	boolean allFields=false;

	switch(pageId){
		case Page_listVal:{
			allFields=true;
			break;
		}
		case Page_list:{
			allFields=false;
			break;
		}
		default:{
			return HTTP_CODE_NOT_IMPLEMENTED;
		}
	}

	for(uint8_t i=0;i<getSensorsCount();i++){

		JsonObject& item=items.createNestedObject();
		AbstractSensor* sens=getSensorById(i);

		item["id"] = sens->getId();
		item["name"] = sens->getName();
		item["val"] = sens->getVal();

		if(allFields){
			item["descr"] = sens->getDescr();
			item["type"] = sens->getType();
			item["size"] = sens->getSize();
		}

		JsonArray& itemChilds=item.createNestedArray(DEFAULT_CHILDREN_TAG);

		for(uint8_t childId=0;childId<sens->getItemCount();childId++){
			JsonObject& child=itemChilds.createNestedObject();

			child["id"] = sens->getItem(childId)->id;
			child["name"] = sens->getItem(childId)->name;
			child["val"] = sens->getItem(childId)->val;

			if(allFields){
				child["descr"] = sens->getItem(childId)->descr;
				child["type"] = sens->getItem(childId)->type;
				child["size"] = sens->getItem(childId)->size;

				child["minVal"] = sens->getItem(childId)->minVal;
				child["maxVal"] = sens->getItem(childId)->maxVal;
				child["fieldId"] = sens->getItem(childId)->fieldId;
			}
		}
	}

	long total=millis()-start;

	Serial.print(FPSTR("AbstractSensorService::getAbstractItems get="));
	Serial.println(total);

	return HTTP_CODE_OK;
}

int AbstractSensorService::postAbstractItems(JsonArray& items,uint8_t pageId) {

	Serial.print(FPSTR("AbstractSensorService::postAbstractItems pageId="));
	Serial.print(pageId);
	Serial.print(FPSTR(" pageName="));
	Serial.println(PAGE_NAME[pageId]);
	long start=millis();

	boolean allFields=false;

	switch(pageId){
		case Page_save:{
			allFields=true;
			break;
		}
		case Page_saveVal:{
			allFields=false;
			break;
		}
		default:{
			return HTTP_CODE_NOT_IMPLEMENTED;
		}

	}

	for(uint8_t i=0;i<items.size();i++){
		JsonObject& item=items[i].asObject();

		AbstractSensor* sens=getSensorById(item["id"]);

		if(sens!=NULL && sens!=nullptr){
			if(sens->checkName(item["name"])){

				sens->setVal(item["val"]);

				if(allFields){
					sens->setDescr(item["descr"]);
				}

				if(item.containsKey(DEFAULT_CHILDREN_TAG)){
					JsonArray& itemChilds = item[DEFAULT_CHILDREN_TAG];

					for(uint8_t childId=0;childId<itemChilds.size();childId++){

						JsonObject& child=itemChilds[childId].asObject();

						sens->setVal(childId, child["val"]);
						if(allFields){
							sens->setDescr(childId, child["descr"]);
						}

					}

				}

			}
		}

	}

	long total=millis()-start;

	Serial.print(FPSTR("AbstractSensorService::postAbstractItems totalTime="));
	Serial.println(total);

	return HTTP_CODE_OK;
}

int AbstractSensorService::getEntityIdByName(String name) {
	for(uint8_t i=0;i<2;i++){
		if(ENTITY_NAME[i]==name.c_str()){
			return i;
		}
	}
	return -1;
}

uint8_t AbstractSensorService::getEntityId() {
	return Entity_sensors;
}
