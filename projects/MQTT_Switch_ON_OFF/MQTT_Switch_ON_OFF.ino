#include <Arduino.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Loopable.h"
#include "PinEventProcessor.h"
#include "FS.h"
#include "I2Chelper.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"

#include <BME280_Measurer.h>
#include <BH1750_Measurer.h>
#include <DeviceHelper.h>
#include <PinDigital.h>
#include "ConfigStorage.h"
#include "PirDetector.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

#define DISPLAY_LOOPS 1

#define FIRMVARE_VERSION "b@l@bas-soft ONOFF v0.0.5"

//Robot Dyn//vem  // 12E
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp           PIN_WIRE_SCL
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue    PIN_WIRE_SDA
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed               INTERNAL_LED_PIN
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define RX_PIN 3
#define TX_PIN 1

#define SD3_PIN 10
#define SD2_PIN 9

#define HUMAN_PRESENTED LOW
#define HUMAN_NOT_PRESENTED HIGH

const int sensorsInterval=60000;

WiFiClient wclient;
ESP8266WebServer server ( 80 );
//WebSocketsServer webSocket = WebSocketsServer(8081);

EspSettingsBox espSettingsBox("/settings.txt","",true,true);

I2Chelper i2cHelper(D1_PIN,D2_PIN,false);
DisplayHelper displayHelper(true);

String subscribeTopics[]={"topic/ESP8266Topic","topic/SwitchLeft","topic/SwitchRight"};

TimeTrigger sensorsTrigger(0,sensorsInterval,true,measureSensors);

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7_PIN,onLeftButtonChanged,0,"");
PinDigital buttonRight(VAR_NAME(buttonRight),D6_PIN,onRightButtonChanged,0,"");

PinDigital signalLed(VAR_NAME(signalLED),D4_PIN,nullptr,OUTPUT,CHANGE,LOW,HUMAN_NOT_PRESENTED,0,"");

PinDigital lampLeft(VAR_NAME(lampLeft),D5_PIN,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW,0,"");
PinDigital lampRight(VAR_NAME(lampRight),D8_PIN,onRightLampChanged,OUTPUT,CHANGE,HIGH,LOW,0,"");

PirDetector pirDetector(VAR_NAME(pirDetector),D3_PIN,&signalLed,nullptr,0,"");

//NtpTimeClientService timeClient(&espSettingsBox,processTimeClientEvent,60000);

BME280_Measurer bmeMeasurer(D1_PIN,VAR_NAME(bmeMeasurer),0,"");
BH1750_Measurer luxMeasurer(D1_PIN,VAR_NAME(luxMeasurer),0,"");

WiFiHelper wifiHelper("WiFiHelper",&espSettingsBox, &displayHelper, &signalLed,	&server,postInitWebServer,false);

MqttHelper mqttHelper(&espSettingsBox,subscribeTopics,ARRAY_SIZE(subscribeTopics),wclient,processMqttEvent);

Loopable* loopArray[]={&wifiHelper,&sensorsTrigger,&buttonLeft,&buttonRight,&pirDetector};

Initializable* initializeArray[]={&espSettingsBox,&wifiHelper,&i2cHelper,&bmeMeasurer,&luxMeasurer};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray));

void setup() {
  deviceHelper.startDevice(espSettingsBox.DeviceId);
  deviceHelper.printDeviceDiagnostic();

  espSettingsBox.printSettingsFile();
  espSettingsBox.printSpiffsInfo();

  deviceHelper.init(initializeArray, ARRAY_SIZE(initializeArray));
  deviceHelper.printDeviceDiagnostic();
  deviceHelper.displayDetails();

}

void loop() {
	deviceHelper.loop();

	buttonLeft.loop();
	buttonRight.loop();
}

void postInitWebServer(){

	server.on("/espSettingsBox/getJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), espSettingsBox.getSimpleJson());
	});

	server.on("/buttonLeft/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonLeft.getSimpleJson());
	});
	server.on("/buttonRight/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonRight.getSimpleJson());
	});

	server.on("/lampLeft/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampLeft.getSimpleJson());
	});
	server.on("/lampRight/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampRight.getSimpleJson());
	});
	server.on("/lampLeft/setValue", HTTP_ANY, [](){
		handleLampChange(&lampLeft);
	});
	server.on("/lampRight/setValue", HTTP_ANY, [](){
		handleLampChange(&lampRight);
	});

	server.on("/pirDetector/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), pirDetector.getSimpleJson());
	});

	server.on("/bmeMeasurer/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), bmeMeasurer.getSimpleJson());
	});
	server.on("/luxMeasurer/getSimpleJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), luxMeasurer.getSimpleJson());
	});

	server.on("/getI2C", [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), i2cHelper.scan());
	});
}

//---------------------------------------------------------------------
//button handling
void onLeftButtonChanged(){
	Serial.println("Left button changed");
	bool isOn=buttonLeft.isOn();
	lampLeft.setVal(isOn);
}

void onRightButtonChanged(){
	Serial.println("right button changed");
	bool isOn=buttonRight.isOn();
	lampRight.setVal(isOn);
}
//---------------------------------------------------------------------
//handle lamp events
void handleLampChange(PinDigital* lamp){
	wifiHelper.checkAuthentication();
	int8_t on=server.arg("val").toInt();
	if(on==-1){
		lamp->change();
	}
	lamp->setVal(on);
	server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lamp->getSimpleJson());
}

void onLeftLampChanged(){
	Serial.println("* Left lamp changed");
}

void onRightLampChanged(){
	Serial.println("* Right lamp changed");
}
//---------------------------------------------------------------------
//handle pirDetector events
void onPirDetectorChanged(){
	Serial.println("* Right lamp changed");
}

//---------------------------------------------------------------------
//event and mqtt processing

void measureSensors(){
	Serial.println("---Measure sensors---");
	bmeMeasurer.measure();
	luxMeasurer.measure();
	deviceHelper.printDeviceDiagnostic();
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
}
