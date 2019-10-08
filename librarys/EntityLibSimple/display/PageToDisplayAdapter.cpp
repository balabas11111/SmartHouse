/*
 * PageRenderer.cpp
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <display/PageToDisplayAdapter.h>

void PageToDisplayAdapter::renderPage(const char* header, JsonVariant pageData) {
	this->clear();
	this->setPowerOn();

	Serial.print(FPSTR("Display ="));
	Serial.println(header);

	if (pageData.is<JsonObject>()){
		for( const auto& kv : pageData.as<JsonObject>() ) {
		    Serial.println(kv.key);
		    Serial.println(kv.value.as<char*>());
		}
	}else if(pageData.is<JsonArray>()){
		for( const auto& value : pageData.as<JsonArray>() ) {
		    Serial.println(value.as<char*>());
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
	}
}

void PageToDisplayAdapter::loop() {
	if(this->isPowerOn()){
		if(this->turnOnTime + DISPLAY_TURN_OFF_TIME < millis()){
			setPowerOn(false);
		}
	}
}
