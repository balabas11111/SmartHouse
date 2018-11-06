#include <Arduino.h>
#include <BH1750_Sensor.h>
#include <BME280_Sensor.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Loopable.h"
#include "FS.h"
#include "I2Chelper.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include "TimeTrigger.h"

#include <DeviceHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include "ConfigStorage.h"
#include "BH1750_Sensor.h"
#include "BME280_Sensor.h"
#include "DHT22_Sensor.h"
#include "DS18D20_Sensor.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

#define DISPLAY_LOOPS 1

#define FIRMVARE_VERSION "b@l@bas-soft ONOFF v0.0.5"

//Robot Dyn//vem  // 12E
/*
#define A0_PIN 17
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp           PIN_WIRE_SCL
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue    PIN_WIRE_SDA
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed               INTERNAL_LED_PIN
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define SD3_PIN 10
#define SD2_PIN 9
*/
#define RX_PIN 3
#define TX_PIN 1


#define HUMAN_PRESENTED LOW
#define HUMAN_NOT_PRESENTED HIGH

const int sensorsInterval=60000;

WiFiClient wclient;
ESP8266WebServer server ( 80 );
//WebSocketsServer webSocket = WebSocketsServer(8081);

EspSettingsBox espSettingsBox("",true,true);

I2Chelper i2cHelper(D1,D2,false);
DisplayHelper displayHelper(true);

String subscribeTopics[]={"topic/ESP8266Topic","topic/SwitchLeft","topic/SwitchRight"};

TimeTrigger sensorsTrigger(0,sensorsInterval,true,measureSensors);

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7,onLeftButtonChanged);
PinDigital buttonRight(VAR_NAME(buttonRight),D6,onRightButtonChanged);

//PinDigital signalLed(VAR_NAME(signalLED),D4,nullptr,OUTPUT,CHANGE,LOW,HUMAN_NOT_PRESENTED,0,"");

PinDigital lampLeft(VAR_NAME(lampLeft),D5,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW);
PinDigital lampRight(VAR_NAME(lampRight),D8,onRightLampChanged,OUTPUT,CHANGE,HIGH,LOW);

Pir_Sensor pirDetector(VAR_NAME(pirDetector),A0,onPirDetectorChanged);

//NtpTimeClientService timeClient(&espSettingsBox,processTimeClientEvent,60000);

BME280_Sensor bmeMeasurer(20,VAR_NAME(bmeMeasurer));
BH1750_Sensor luxMeasurer(21,VAR_NAME(luxMeasurer));

DHT22_Sensor dhtMeasurer(VAR_NAME(dhtSensor), D0, 22);

DS18D20_Sensor ds18d20Measurer(VAR_NAME(ds18d20Measurer), D3);

WiFiHelper wifiHelper("WiFiHelper",&espSettingsBox, &displayHelper, /*nullptr,*/&server,postInitWebServer,false);

MqttHelper mqttHelper(&espSettingsBox,subscribeTopics,ARRAY_SIZE(subscribeTopics),wclient,processMqttEvent);

Loopable* loopArray[]={&wifiHelper,&sensorsTrigger,&buttonLeft,&buttonRight,&pirDetector};
Initializable* initializeArray[]={&espSettingsBox,&wifiHelper,&i2cHelper,&bmeMeasurer,&luxMeasurer,&dhtMeasurer,&ds18d20Measurer};

Measurable* measurableArray[]={&bmeMeasurer,&luxMeasurer,&dhtMeasurer,&ds18d20Measurer};
AbstractItem* minMaxValues[]={&lampLeft,&lampRight,&bmeMeasurer,&luxMeasurer,&dhtMeasurer,&ds18d20Measurer};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray));

void setup() {
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.printSettingsFile();
  espSettingsBox.printSpiffsInfo();

  deviceHelper.init(initializeArray, ARRAY_SIZE(initializeArray));
  deviceHelper.printDeviceDiagnostic();
  deviceHelper.displayDetails();

  loadSensors();
  measureSensors();
}

void loop() {
	deviceHelper.loop();
}

void postInitWebServer(){
	server.on(espSettingsBox.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), espSettingsBox.getJson());
	});
	server.on("/submitAllSensorsJson", HTTP_POST, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), setAllSensorsJson());
	});
	server.on("/getAllSensorsJson", HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), getAllSensorsJson());
	});
	server.on(espSettingsBox.getSetValueUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), setEspSettingsBoxValues());
	});

	server.on(buttonLeft.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonLeft.getJson());
	});
	server.on(buttonRight.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonRight.getJson());
	});

	server.on(lampLeft.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampLeft.getJson());
	});
	server.on(lampRight.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampRight.getJson());
	});
	server.on(lampLeft.getSetValueUrl(), HTTP_ANY, [](){
		handleLampChange(&lampLeft);
	});
	server.on(lampRight.getSetValueUrl(), HTTP_ANY, [](){
		handleLampChange(&lampRight);
	});

	server.on(pirDetector.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), pirDetector.getJson());
	});

	server.on(bmeMeasurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), bmeMeasurer.getJson());
	});
	server.on(luxMeasurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), luxMeasurer.getJson());
	});
	server.on(dhtMeasurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), dhtMeasurer.getJson());
	});
	server.on(ds18d20Measurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), ds18d20Measurer.getJson());
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
	Serial.print(" Lamp on=");
	Serial.println(on);

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
//----------espSettings save-------------------------------------------
String setEspSettingsBoxValues(){
	wifiHelper.checkAuthentication();

	return "";
}
//---------------------------------------------------------------------
//handle pirDetector events
void onPirDetectorChanged(){
	Serial.println("* Pir changed");
}

//---------------------------------------------------------------------
//event and mqtt processing

void loadSensors(){
	espSettingsBox.loadAbstractItemsFromFile(minMaxValues, ARRAY_SIZE(minMaxValues));
}

void saveSensors(){
	espSettingsBox.saveAbstractItemsToFile(minMaxValues, ARRAY_SIZE(minMaxValues));
}

String setAllSensorsJson(){
	wifiHelper.checkAuthentication();
	deviceHelper.processAbstractitemsSettings(minMaxValues, ARRAY_SIZE(minMaxValues),&server);

	return getAllSensorsJson();
}

String getAllSensorsJson(){
	wifiHelper.checkAuthentication();
	return deviceHelper.getJson(minMaxValues, ARRAY_SIZE(minMaxValues));

}

void measureSensors(){
	deviceHelper.update(measurableArray, ARRAY_SIZE(measurableArray));
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
}
