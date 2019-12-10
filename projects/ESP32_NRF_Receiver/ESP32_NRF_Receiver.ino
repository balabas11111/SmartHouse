#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "EntityNrf.h"
#include "EntityNrfHolder.h"
#include "EntityNrfManager.h"
#include "EntityApplication.h"

char* pref = (char*) "th";
char* name = (char*)"tankerHeatTemp";
char* description = (char*)"Температура Бойлер акумулятор";
char* tankFields[] = {(char*)"up",(char*)"md",(char*)"dn"};
char* targetTankFields[] = {(char*)"tempUp",(char*)"tempMiddle",(char*)"tempDown"};
char* tankDescr[] = {(char*)"temp Up of tank",(char*)"temp middle of tank",(char*)"temp down of tank"};
char* tankMeasure[] = {(char*)"C",(char*)"C",(char*)"C"};

EntityNrf entity(description, name, targetTankFields, tankDescr, tankMeasure, 3);
EntityNrfHolder holder(&entity, pref, tankFields, 3);

Entity* entities[] = { &entity };
EntityUpdate* updateableEntities[] = {};

EntityNrfHolder* holders = {&holder};

EntityNrfManager manager(&holders, 1);

char msg[16];
const uint64_t pipeReceive = 0xE8E8F0F0E1LL;
RF24 radio(12, 14, 26, 25, 27);

EntityApplication app("ESP32_NRF24", (char*) "Lolin32 NRF dev", entities,
		ARRAY_SIZE(entities), updateableEntities,
		ARRAY_SIZE(updateableEntities), "1F321");

void setup(void){
	//app.begin();
	//manager.init();
	Serial.begin(115200);
	 radio.begin();
	 radio.setChannel(2);
	 radio.setPayloadSize(7);
	 radio.setDataRate(RF24_250KBPS);
	 radio.openReadingPipe(1,pipeReceive);
	 radio.startListening();
	 Serial.println(FPSTR("Radio initialized"));
}

void loop(void){
	//app.loop();
	//manager.loop();
	if (radio.available()){
	     radio.read(msg, 6);
	     Serial.println(msg);
	     delay(10);
	 }
	 else{
	  //Serial.println("No radio available");
	 }
}
