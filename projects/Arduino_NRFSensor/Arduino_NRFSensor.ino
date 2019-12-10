/*
 #include "Arduino.h"
 #include  <SPI.h>
 #include "OneWire.h"
 #include "DallasTemperature.h"

 #include "nRF24L01.h"
 #include "RF24.h"

 #define BASE_PREF "1:th"
 #define PREF_UP     "up:"
 #define PREF_MIDDLE "ml:"
 #define PREF_DOWN   "dn:"

 #define PIN_CE             7
 #define PIN_CS             8

 #define PIN_DS18D20_UP     10
 #define PIN_DS18D20_MIDDLE 9
 #define PIN_DS18D20_DOWN   6

 #define PIN_BUTTON         5

 RF24 radio(PIN_CE, PIN_CS);

 OneWire oneWire(PIN_DS18D20_UP);
 OneWire oneWire2(PIN_DS18D20_MIDDLE);

 DallasTemperature temp1(&oneWire);
 DallasTemperature temp2(&oneWire2);

 float temperatures[2];

 const uint64_t pipe = 0xE8E8F0F0E1LL;

 char msg[16] = "hello world is mine";

 void setup(void) {
 Serial.begin(115200);

 temp1.begin();
 temp2.begin();

 Serial.print("temp1.Count=");
 Serial.println(temp1.getDeviceCount());
 Serial.print("temp1.Count=");
 Serial.println(temp2.getDeviceCount());

 initRadio();

 pinMode(PIN_BUTTON, INPUT_PULLUP);
 attachInterrupt(PIN_BUTTON, onButtonPressed, FALLING );
 }

 void initRadio() {
 radio.begin();
 radio.setChannel(2);
 radio.setPayloadSize(17);
 radio.setDataRate(RF24_250KBPS);
 radio.openWritingPipe(pipe);
 }

 void readSendTemperatures(uint8_t targetId, char* in, DallasTemperature* temp) {
 for(int i =0; i< temp->getDeviceCount(); i++) {
 uint8_t* addr;
 String str = BASE_PREF;
 str += in;
 temp->getAddress(addr, i);
 //getDeviceAddress(str, temp, addr, i);
 Serial.print("id=");
 Serial.print(i);
 //Serial.println(addr);
 float t = temp->getTempC(addr);
 Serial.print(" t=");
 Serial.println(t);
 temperatures[targetId] = t;
 str += t;
 str += ":";
 Serial.println(str);
 sendRadio(str);
 }
 }

 void getDeviceAddress(String result, DallasTemperature* temp, DeviceAddress deviceAddress, uint8_t index) {
 temp->getAddress(deviceAddress, index);

 for (uint8_t i = 0; i < 8; i++)
 {
 if (deviceAddress[i] < 16 ) result=result+"0";
 result=result+String(deviceAddress[i], HEX);
 }
 }

 void sendRadio(String& str) {
 Serial.print("sendRadio str=");
 Serial.print(str);
 Serial.print(" len=");
 Serial.println(str.length());

 //radio.write(str.c_str(), str.length());
 //radio.write(msg, 16);
 Serial.println("sent");
 }

 void loop(void) {
 Serial.println("send ...");

 readSendTemperatures(0, (char*)PREF_UP, &temp1);
 readSendTemperatures(1, (char*)PREF_MIDDLE, &temp2);

 delay(5000);
 }

 void onButtonPressed(){
 Serial.println("btn");
 }
 */
#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#define PIN_DS18D20_UP     10
#define PIN_DS18D20_MIDDLE 9
#define PIN_DS18D20_DOWN   6

#define PIN_BUTTON         5

uint8_t bus_count = 3;

OneWire oneWire(PIN_DS18D20_UP);
OneWire oneWire2(PIN_DS18D20_MIDDLE);
OneWire oneWire3(PIN_DS18D20_DOWN);

DallasTemperature temp1(&oneWire);
DallasTemperature temp2(&oneWire2);
DallasTemperature temp3(&oneWire3);

RF24 radio(7, 8);
const uint64_t pipe = 0xE8E8F0F0E1LL;

float tempVal[] = {-127, -127, -127};
char* pref[] = {"th:up:", "th:md:", "th:dn:"};

void setup(void) {
	Serial.begin(115200);
	Serial.println("device begin");
	temp1.begin();
	temp2.begin();

	Serial.println("temp begin");
	Serial.print("temp1 =");
	Serial.print(temp1.getDeviceCount());
	Serial.print("temp2 =");
	Serial.println(temp2.getDeviceCount());

	radio.begin();
	radio.setChannel(2);
	radio.setPayloadSize(17);
	radio.setDataRate(RF24_250KBPS);
	radio.openWritingPipe(pipe);

	Serial.println("radio begin");
	//temp1.begin();
	//temp2.begin();
}
void loop(void) {
	readTemperatures();
	sendTemperatures();

	delay(15000);
}

void readTemperatures() {
	//tempVal[0] = temp2.getTempCByIndex(0);
	tempVal[1] = temp2.getTempCByIndex(0);
	//tempVal[2] = temp3.getTempCByIndex(0);

}

void sendTemperatures() {
	for(int i=0; i<bus_count; i++){
		String str=pref[i];
		str+=tempVal[i];
		str+=":";
		radio.write(str.c_str(), str.length());

		Serial.println(str);
	}
}

