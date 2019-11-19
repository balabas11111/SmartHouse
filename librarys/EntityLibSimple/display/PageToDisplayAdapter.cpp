/*
 * PageRenderer.cpp
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <display/PageToDisplayAdapter.h>

void PageToDisplayAdapter::renderPage(const char* header, JsonVariant pageData) {
	if(!this->isInitialized()) {
		Serial.println(FPSTR("DISPLAY not initialized"));
		return;
	}

	if(this->isPowerOn()) {
		this->clear();

		toStartPosition();
		print(header);
		printNextLine();
		printNextLine();

		if (pageData.is<JsonObject>()){
			for( const auto& kv : pageData.as<JsonObject>() ) {
				JsonObject& obj = kv.value;

				if(obj.containsKey(COL_NAME)) {
					print(obj[COL_NAME].as<char*>());
					printNextLine();
				}

				String line = "     ";

				if(obj.containsKey(COL_VALUE)) {
					if(obj[COL_VALUE].is<char*>()) {
						line += obj[COL_VALUE].as<char*>();
					}else if(obj[COL_VALUE].is<float>()){
						line += obj[COL_VALUE].as<float>();
					}else if(obj[COL_VALUE].is<int>()){
						line += obj[COL_VALUE].as<int>();
					}
				}

				if(obj.containsKey(COL_MEASURE)) {
					line += obj[COL_MEASURE].as<char*>();
				}

				print(line.c_str());
				printNextLine();
			}
		}else if(pageData.is<JsonArray>()){
			for( const auto& value : pageData.as<JsonArray>() ) {
				print(value.as<char*>());
				printNextLine();
			}
		}
	}
}

bool PageToDisplayAdapter::isPowerOn() {
	return this->powerOn;
}

void PageToDisplayAdapter::setPowerOn(bool powerOn) {
	this->powerOn = powerOn;
	if(powerOn){
		this->turnOnTime = millis();
		turnOnHardware();
	} else {
		turnOffHardware();
	}
	Serial.print(FPSTR("setPower "));
	Serial.println(powerOn);
}

void PageToDisplayAdapter::loop() {
	if(this->isPowerOn()){
		if(this->turnOnTime + DISPLAY_TURN_OFF_TIME < millis()){
			setPowerOn(false);
		}
		/*
		Serial.print(FPSTR("turnOnTime="));
		Serial.print(turnOnTime);
		Serial.print(FPSTR(" turnOnExp="));
		Serial.print(DISPLAY_TURN_OFF_TIME);
		Serial.print(FPSTR(" now="));
		Serial.println(millis());
		*/

	}
}

bool PageToDisplayAdapter::isInitialized() {
	return this->initialized;
}

bool PageToDisplayAdapter::init() {
	if(isInitialized()) {
		//Serial.println("Already initialized");
		return true;
	}

	this->initialized = initHardware();

	if(this->isInitialized()) {
		setPowerOn(true);
	}

	return isInitialized();
}
