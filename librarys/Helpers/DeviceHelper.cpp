/*
 * Looper.cpp
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#include <DeviceHelper.h>

DeviceHelper::DeviceHelper(Loopable** _loopItems,uint8_t _loopItemsSize){
	loopItems=_loopItems;
	loopItemsSize=_loopItemsSize;

	displayDetails();
	Serial.println();
}

String DeviceHelper::displayDetails(){
	Serial.println("----------------------");

	String result="DeviceHelper loop size="+String(loopItemsSize);

	Serial.println(result);

	Serial.println();
	for(uint8_t i=0;i<loopItemsSize;i++){
		loopItems[i]->displayDetails();
	}

	Serial.println("----------------------");

	return result;
}


boolean DeviceHelper::loop(){
	boolean result=false;

	for(uint8_t i=0;i<loopItemsSize;i++){
		result=result || loopItems[i]->loop();
	}
	#ifdef DISPLAY_LOOPS
		Serial.println("DeviceHelper loop="+String(result));
	#endif

	return result;
}

void DeviceHelper::startDevice(String deviceId){
  Serial.begin(115200);
  delay(100);

  Serial.println("-------------Device helper-----------");
  Serial.println("Start DeviceId="+deviceId);
  Serial.println("-------------------------------------");
}

boolean DeviceHelper::init(Initializable** initItems,uint8_t initItemsSize){
	Serial.println("-------------Device helper-----------");
	Serial.println("Init started count="+String(initItemsSize));
	uint8_t initOk=0;

	for(uint8_t i=0;i<initItemsSize;i++){
		boolean init=initItems[i]->init();
		if(init){
			initOk++;
		}
	}

	Serial.println("Initialized count="+String(initOk));
	Serial.println("-------------Device helper COMPLETE-----------");
	displayDetails();

	return initOk==initItemsSize;
}

void DeviceHelper::printDeviceDiagnostic(){
	Serial.println("--------------Device diagnostic-------------------");
	Serial.println("FreeHeap="+String(ESP.getFreeHeap()));
	Serial.println("CpuFreqMHz="+String(ESP.getCpuFreqMHz()));
	Serial.println("FlashChipSize="+String(ESP.getFlashChipSize()));
	Serial.println("FreeSketchSpace="+String(ESP.getFreeSketchSpace()));
	Serial.println("ResetReason="+String(ESP.getResetReason()));
	Serial.println("--------------------------------------------------");
}



