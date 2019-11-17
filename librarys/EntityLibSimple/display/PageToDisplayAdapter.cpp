/*
 * PageRenderer.cpp
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <display/PageToDisplayAdapter.h>

void PageToDisplayAdapter::renderPage(const char* header, JsonVariant pageData) {
	if(this->isPowerOn()) {
		this->clear();
/*
		Serial.print(FPSTR("Display ="));
		Serial.println(header);
*/
		toStartPosition();
		print(header);
		printNextLine();
		printNextLine();

		if (pageData.is<JsonObject>()){
			for( const auto& kv : pageData.as<JsonObject>() ) {
				/*Serial.print(kv.key);
				Serial.print(FPSTR("="));
*/
				String line = "";
				JsonObject& obj = kv.value;

				line += obj["name"].asString();

				if(obj["val"].is<char*>()) {
					line += obj["val"].as<char*>();
				}else if(obj["val"].is<float>()){
					line += obj["val"].as<float>();
				}else if(obj["val"].is<int>()){
					line += obj["val"].as<int>();
				}
				line += obj["msr"].asString();

				//Serial.println(line);

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
	} else {
		this->clear();
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
