#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Loopable.h"
#include "PinEventProcessor.h"
//#include "WiFiHelper.h"
#include "FS.h"
#include "I2Chelper.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"

#include <BME280_Measurer.h>
#include <BH1750_Measurer.h>
#include <DeviceHelper.h>
#include "WidgetHelper.h"

#include "PirDetector.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

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

EspSettingsBox espSettingsBox("/values2.txt","",true,true);

WiFiClient wclient;
ESP8266WebServer server ( 80 );

I2Chelper i2cBus(D1_PIN,D2_PIN,false);
DisplayHelper displayHelper(true);

String subscribeTopics[]={"topic/ESP8266Topic","topic/SwitchLeft","topic/SwitchRight"};

//TimeTrigger sensorsTrigger(0,sensorsInterval,false,[](){Serial.println(getWidgetsHtlm())});

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7_PIN,processEvent,onButtonChanged);
PinDigital buttonRight(VAR_NAME(buttonRight),D6_PIN,processEvent,onButtonChanged);

PinDigital signalLed(VAR_NAME(signalLED),D4_PIN,OUTPUT,nullptr,CHANGE,LOW,HUMAN_NOT_PRESENTED);

PinDigital lampLeft(VAR_NAME(lampLeft),D5_PIN,OUTPUT,processEvent,CHANGE,HIGH);
PinDigital lampRight(VAR_NAME(lampRight),D8_PIN,OUTPUT,processEvent,CHANGE,HIGH);

PirDetector pirDetector(D3_PIN,&signalLed,processEvent);

BME280_Measurer bmeMeasurer("0",VAR_NAME(bmeMeasurer));
BH1750_Measurer luxMeasurer("1",VAR_NAME(luxMeasurer));

PinEventProcessor *eventProcessors[]={&lampLeft,&lampRight,
												&buttonLeft,&buttonRight,
												&bmeMeasurer,&luxMeasurer};

WiFiHelper wifiHelper("WiFiHelper",&espSettingsBox, &displayHelper, &signalLed,	&server,
		postInitWebServer,false, handleHttpWidget, processEvent);

MqttHelper mqttHelper(&espSettingsBox,subscribeTopics,ARRAY_SIZE(subscribeTopics),wclient,eventProcessors,ARRAY_SIZE(eventProcessors),processMqttEvent);

Loopable* loopArray[]={&buttonLeft,&buttonRight,
							&lampLeft,&lampRight/*,&mqttHelper*/};
Initializable* initializeArray[]={&espSettingsBox,&wifiHelper,&i2cBus,&bmeMeasurer,&luxMeasurer};

HtmlWidget* widgetsArray[]={&bmeMeasurer,&luxMeasurer};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray));


void setup() {
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.printSpiffsInfo();

  deviceHelper.init(initializeArray, ARRAY_SIZE(initializeArray));
}

void loop() {
 deviceHelper.loop();
 server.handleClient();
}

void postInitWebServer(){
	server.on("/getWidgets", [](){
		server.send(200, "text/html", WidgetHelper::getWidgetsJson(&espSettingsBox, widgetsArray, ARRAY_SIZE(widgetsArray)));
	});
}

void handleHttpWidget(){
	//class ="MeasurerWidgetESP"
	//    /processWidget   param=widgetName
	const String wnParam="widgetName";

	if(server.hasArg(wnParam)){
		String wiName=server.arg(wnParam);
		Serial.println("PorcessWidget="+wiName);

		String html=WidgetHelper::getHtmlWidgetHtml(&espSettingsBox, wiName, widgetsArray, ARRAY_SIZE(widgetsArray));

		if(!html.equals("")){
			server.send(200, "text/html", html);
		}

		server.send(404, "text/html", "Виджет "+wiName+" не найден");
	}

	server.send(400, "text/html", "widgetName parameter missing");
}

String getWidgetsHtlm(){
	return WidgetHelper::getWidgetsJson(&espSettingsBox, widgetsArray, ARRAY_SIZE(widgetsArray));
}

//---------------------------------------------------------------------
//button handling
PinEvent onButtonChanged(PinEvent event){
	//On button changed
	Serial.println("//On button changed");
	if(buttonLeft.isDispatcherOfEvent(event)){
		return lampLeft.constructPinEventSetState(event);
	}
	if(buttonRight.isDispatcherOfEvent(event)){
		return lampRight.constructPinEventSetState(event);
	}
	return PinEvent();
}
//---------------------------------------------------------------------
//event and mqtt processing
PinEvent processEvent(PinEvent event){
	/*
	 //kind:bubble:pinId:oldVal:val:strVal:dispatcherName:targetName:

	UPDATESTate command

		http://192.168.0.100/runCommand?command=PE_SS:1:14:0:1:PE_SS:http:lampLeft:
		http://192.168.0.100/runCommand?command=PE_SS:1:14:1:0:PE_SS:http:lampLeft:
		http://192.168.0.100/runCommand?command=PE_SS:1:15:0:1:PE_SS:http:lampRight:
		http://192.168.0.100/runCommand?command=PE_SS:1:15:1:0:PE_SS:http:lampRight:

	GETState measurers command

		http://192.168.0.100/runCommand?command=PE_SG:1:0:0:0:PE_SG:http:bmeMeasurer:
		http://192.168.0.100/runCommand?command=PE_SG:1:0:0:0:PE_SG:http:luxMeasurer:
	*/
	if(!event.isValid() || !event.isNotEmpty()){
		return PinEvent();
	}

	return mqttHelper.processEvent(event);
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
}

//---------------------------------------------------
