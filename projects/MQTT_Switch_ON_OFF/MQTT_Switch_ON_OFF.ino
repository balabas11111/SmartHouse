#include <Arduino.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Loopable.h"
#include "PinEventProcessor.h"
#include "FS.h"
#include "I2Chelper.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include <Wire.h>
#include <WebSocketsServer.h>
#include <WebSocketsHelper.h>

#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"

#include <BME280_Measurer.h>
#include <BH1750_Measurer.h>
#include <DeviceHelper.h>
#include "ConfigStorage.h"
#include "ConfigStorageWidget.h"
#include "PinDigitalWidget.h"

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

WiFiClient wclient;
ESP8266WebServer server ( 80 );
//WebSocketsServer webSocket = WebSocketsServer(81);

ConfigStorage configStorage;
EspSettingsBox espSettingsBox("/settings.txt","",true,true);

I2Chelper i2cHelper(D1_PIN,D2_PIN,false);
DisplayHelper displayHelper(true);

String subscribeTopics[]={"topic/ESP8266Topic","topic/SwitchLeft","topic/SwitchRight"};

//TimeTrigger sensorsTrigger(0,sensorsInterval,false,[](){Serial.println(getWidgetsHtlm())});

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7_PIN,processEvent,onLeftButtonChanged);
PinDigital buttonRight(VAR_NAME(buttonRight),D6_PIN,processEvent,onRightButtonChanged);

PinDigital signalLed(VAR_NAME(signalLED),D4_PIN,OUTPUT,nullptr,CHANGE,LOW,HUMAN_NOT_PRESENTED);

PinDigital lampLeft(VAR_NAME(lampLeft),D5_PIN,OUTPUT,processEvent,CHANGE,HIGH);
PinDigital lampRight(VAR_NAME(lampRight),D8_PIN,OUTPUT,processEvent,CHANGE,HIGH);

PirDetector pirDetector(D3_PIN,&signalLed,processEvent,nullptr);

BME280_Measurer bmeMeasurer("0",VAR_NAME(bmeMeasurer));
BH1750_Measurer luxMeasurer("1",VAR_NAME(luxMeasurer));

ConfigStorageWidget configStorageWidget(&configStorage,VAR_NAME(configStorageWidget));
PinDigitalWidget lampLeftWidget(&lampLeft,"./img/OnLamp.png","./img/OnLamp.png");
PinDigitalWidget lampRightWidget(&lampRight,"./img/OnLamp.png","./img/OnLamp.png");

PinEventProcessor *eventProcessors[]={&lampLeft,&lampRight,
												&buttonLeft,&buttonRight,
												&bmeMeasurer,&luxMeasurer};

WiFiHelper wifiHelper("WiFiHelper",&espSettingsBox, &displayHelper, &signalLed,	&server,
		postInitWebServer,false, handleHttpWidget, processEvent);

//MqttHelper mqttHelper(&configStorage,&espSettingsBox,subscribeTopics,ARRAY_SIZE(subscribeTopics),wclient,eventProcessors,ARRAY_SIZE(eventProcessors),processMqttEvent);

//WebSocketsHelper webSocketHelper(&webSocket,webSocketEvent);

Loopable* loopArray[]={&buttonLeft,&buttonRight,
							&lampLeft,&lampRight,&wifiHelper/*,&mqttHelper*/};
Initializable* initializeArray[]={&configStorage,&espSettingsBox,&wifiHelper,&i2cHelper,&bmeMeasurer,&luxMeasurer};

HtmlWidget* widgetsArray[]={&bmeMeasurer,&luxMeasurer,&configStorageWidget,&lampLeftWidget,&lampRightWidget};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray));


void setup() {
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  deviceHelper.printDeviceDiagnostic();

  espSettingsBox.printSettingsFile();
  espSettingsBox.printSpiffsInfo();
/*
  deviceHelper.printDeviceDiagnostic();

  espSettingsBox.webDefPage="/web/index.htm";
  espSettingsBox.webDepExt="html,htm,js,css,jpeg,png,jpg,bmp";
  espSettingsBox.webRoot="/web/";

  Serial.println("webDefPage="+espSettingsBox.webDefPage);
  Serial.println("webDepExt="+espSettingsBox.webDepExt);
  Serial.println("webRoot="+espSettingsBox.webRoot);

  espSettingsBox.getSettingsFromMemory(false);
  Serial.println("------------------------");

  String str="";
  (espSettingsBox.getSettingsFromMemory(false)).printTo(str);

  Serial.println("str="+str);
  Serial.println("------------------------");
  deviceHelper.printDeviceDiagnostic();
  (espSettingsBox.getSettingsFromMemory(true));
  deviceHelper.printDeviceDiagnostic();
  //espSettingsBox.saveSettingsJson();
  espSettingsBox.loadSettingsJson();
  deviceHelper.printDeviceDiagnostic();
  Serial.println("sid="+espSettingsBox.getParamVal("sid"));
  deviceHelper.printDeviceDiagnostic();
  espSettingsBox.loadSettingsJson();
  deviceHelper.printDeviceDiagnostic();
  espSettingsBox.getSettingsFromMemory(false);
  deviceHelper.printDeviceDiagnostic();
  */
  deviceHelper.init(initializeArray, ARRAY_SIZE(initializeArray));
  deviceHelper.printDeviceDiagnostic();
}

void loop() {
 deviceHelper.loop();
}

void postInitWebServer(){
	/*
	server.on("/getWidgets", [](){
		server.send(200, "text/html", WidgetHelper::getWidgetsJson(&espSettingsBox, widgetsArray, ARRAY_SIZE(widgetsArray)));
	});
	*/
	server.on("/getI2C", [](){
		server.send(200, "text/html", i2cHelper.scan());
	});
}

void handleHttpWidget(){

	if(!server.hasArg(FPSTR(PARAM_ACTION_ID))
			||!server.hasArg(FPSTR(PARAM_REMOTE_ID))
			||!server.hasArg(FPSTR(PARAM_REMOTE_VAL))
			||!server.hasArg(FPSTR(PARAM_CLASS_NAME))
			||!server.hasArg(FPSTR(PARAM_CHILD_CLASS))
			||!server.hasArg(FPSTR(PARAM_CLIENT_DATA))){

		server.send(400, "text/html", "Miss one of params "
				+String(FPSTR(PARAM_ACTION_ID))
				+String(FPSTR(PARAM_REMOTE_ID))
				+String(FPSTR(PARAM_REMOTE_VAL))
				+String(FPSTR(PARAM_CLASS_NAME))
				+String(FPSTR(PARAM_CHILD_CLASS))
				+String(FPSTR(PARAM_CLIENT_DATA)));
	}

	if(server.arg(FPSTR(PARAM_ACTION_ID)).equals("")
				||server.arg(FPSTR(PARAM_REMOTE_ID)).equals("")
				||server.arg(FPSTR(PARAM_CLASS_NAME)).equals("")
				){

			server.send(400, "text/html", "Empty one of params "
					+String(FPSTR(PARAM_ACTION_ID))
					+String(FPSTR(PARAM_REMOTE_ID))
					+String(FPSTR(PARAM_CLASS_NAME))
					);
		}
	String actionName=server.arg(FPSTR(PARAM_ACTION_ID));
	String remoteId=server.arg(FPSTR(PARAM_REMOTE_ID));
	String remoteVal=server.arg(FPSTR(PARAM_REMOTE_VAL));
	String className=server.arg(FPSTR(PARAM_CLASS_NAME));
	String childClass=server.arg(FPSTR(PARAM_CHILD_CLASS));
	String clientData=server.arg(FPSTR(PARAM_CLIENT_DATA));

	int16_t index=-1;

	for(uint8_t i=0;i<ARRAY_SIZE(widgetsArray);i++){
		if(widgetsArray[i]->isTargetOfAction(actionName, remoteId, remoteVal, className, childClass, clientData)){
			index=i;
			break;
		}
	}

	if(index!=-1){
		server.send(200, "text/html",
			widgetsArray[index]->
			executeClientAction(actionName, remoteId, remoteVal, className, childClass, clientData)
			);
	}

	server.send(404, "text/html", "Виджет "+remoteId+" не найден");
}

//--------------------------websockets----------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if (type == WStype_TEXT){
   Serial.println("-----------Web socket event-----");
   Serial.print("num=");Serial.print(num);
   Serial.print("type=");Serial.print(type);
   Serial.print("length=");Serial.print(length);
   Serial.println();

   for(size_t i = 0; i < length; i++) Serial.print((char) payload[i]);
   Serial.println();
  }
}
//---------------------------------------------------------------------
//button handling
PinEvent onLeftButtonChanged(PinEvent event){
	//webSocketHelper.sendMessageToAll(lampLeftWidget.getWsText());
	return lampLeft.constructPinEventSetState(event);
}

PinEvent onRightButtonChanged(PinEvent event){
	//webSocketHelper.sendMessageToAll(lampRightWidget.getWsText());
	return lampRight.constructPinEventSetState(event);
}
//---------------------------------------------------------------------
//event and mqtt processing
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

PinEvent processEvent(PinEvent event){
		if(!event.isValid() || !event.isNotEmpty()){
		return PinEvent();
	}

	//return mqttHelper.processEvent(event);
	return PinEvent();
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
}

//---------------------------------------------------
