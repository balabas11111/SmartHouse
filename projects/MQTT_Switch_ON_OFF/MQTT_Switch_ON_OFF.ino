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
#include <PinDigital_Event.h>
#include "ConfigStorage.h"
#include "ConfigStorageWidget.h"
#include "PinDigitalWidget.h"
#include "EspSettingsBoxWidget.h"
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

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7_PIN,processEvent,onLeftButtonChanged);
PinDigital buttonRight(VAR_NAME(buttonRight),D6_PIN,processEvent,onRightButtonChanged);

PinDigital signalLed(VAR_NAME(signalLED),D4_PIN,OUTPUT,nullptr,CHANGE,LOW,HUMAN_NOT_PRESENTED);

PinDigital lampLeft(VAR_NAME(lampLeft),D5_PIN,OUTPUT,processEvent,CHANGE,HIGH);
PinDigital lampRight(VAR_NAME(lampRight),D8_PIN,OUTPUT,processEvent,CHANGE,HIGH);

PirDetector pirDetector(D3_PIN,&signalLed,processEvent,nullptr);

//NtpTimeClientService timeClient(&espSettingsBox,processTimeClientEvent,60000);

EspSettingsBoxWidget espSettingsBoxWidget("0",&
		espSettingsBox,VAR_NAME(espSettingsBox));
BME280_Measurer bmeMeasurer("1",VAR_NAME(bmeMeasurer));
BH1750_Measurer luxMeasurer("2",VAR_NAME(luxMeasurer));

PinDigitalWidget lampLeftWidget("3",&lampLeft,"./img/OnLamp.png","./img/OffLamp.png");
PinDigitalWidget lampRightWidget("4",&lampRight,"./img/OnLamp.png","./img/OffLamp.png");


PinEventProcessor *eventProcessors[]={&lampLeft,&lampRight,
												&buttonLeft,&buttonRight,
												&bmeMeasurer,&luxMeasurer};

WiFiHelper wifiHelper("WiFiHelper",&espSettingsBox, &displayHelper, &signalLed,	&server,
		postInitWebServer,false, handleHttpWidget, processEvent);

MqttHelper mqttHelper(&espSettingsBox,subscribeTopics,ARRAY_SIZE(subscribeTopics),wclient,eventProcessors,ARRAY_SIZE(eventProcessors),processMqttEvent);
//WebSocketsHelper webSocketHelper(&webSocket,webSocketEvent);

Loopable* loopArray[]={&wifiHelper,&sensorsTrigger,&buttonLeft,&buttonRight,&lampLeft,&lampRight};

Initializable* initializeArray[]={&espSettingsBox,&wifiHelper,&i2cHelper,&bmeMeasurer,&luxMeasurer};

HtmlWidget* widgetsArray[]={&espSettingsBoxWidget,&bmeMeasurer,&luxMeasurer,&lampLeftWidget,&lampRightWidget};

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
/*
	webSocketHelper.loop();
	sensorsTrigger.loop();

	buttonLeft.loop();
	buttonRight.loop();
	lampLeft.loop();
	lampRight.loop();
	*/
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
	//http://192.168.0.100/handleHttpWidget?actionName=getAllWidgetsJson&widgetName=SENS_2252482
	//http://192.168.0.100/handleHttpWidget?actionName=getWidgetJson&widgetName=bmeMeasurer
	deviceHelper.printDeviceDiagnostic();

	int16_t index=-1;

	if(server.arg(FPSTR(PARAM_ACTION_ID))==FPSTR(ACTION_GET_ALL_WIDGET_JSON)
			&& server.arg(FPSTR(PARAM_REMOTE_ID))==espSettingsBox.DeviceId
			){
		//Serial.println("Processing ACTION_GET_ALL_WIDGET_JSON '"+espSettingsBox.DeviceId+"'");

		String result=
				"{\"m.id\":\""+espSettingsBox.DeviceId+"\",\"m.name\":\""+espSettingsBox.DeviceLocation+"\",\"m.val\":\""+espSettingsBox.DeviceId+"\",\"m.kind\":\""+espSettingsBox.DeviceKind+"\",\"m.descr\":\""+espSettingsBox.DeviceDescription+"\",\"itemsCount\":"+String(ARRAY_SIZE(widgetsArray))+",\"time\":\""+String(millis())+"\",\
						\"items\":[";
		uint8_t size=ARRAY_SIZE(widgetsArray);
		for(uint8_t i=0;i<size;i++){
			result+=widgetsArray[i]->executeClientAction(FPSTR(ACTION_GET_WIDGET_JSON), "", "", "", "", "");
			if(i!=size-1){
				result+=",";
			}
		}

		result+="]}";

		server.send(200, "text/html", result);
		//delete &result;
		return;
	}

	for(uint8_t i=0;i<ARRAY_SIZE(widgetsArray);i++){
		//if(widgetsArray[i]->isTargetOfAction(actionName, remoteId, remoteVal, className, childClass, clientData)){
		if(widgetsArray[i]->getName()==server.arg(FPSTR(PARAM_REMOTE_ID))){
			index=i;
			break;
		}
	}

	if(index!=-1){
		String result=widgetsArray[index]->executeClientAction(
						server.arg(FPSTR(PARAM_ACTION_ID)),
						server.arg(FPSTR(PARAM_REMOTE_ID)),
						server.arg(FPSTR(PARAM_REMOTE_VAL)),
						server.arg(FPSTR(PARAM_CLASS_NAME)),
						server.arg(FPSTR(PARAM_CHILD_CLASS)),
						server.arg(FPSTR(PARAM_CLIENT_DATA)));
		//Serial.print("result=");
		//Serial.println(result);

		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), result);
		return;
	}

	server.send(404, FPSTR(CONTENT_TYPE_TEXT_HTML), FPSTR(MESSAGE_STATUS_JSON_WIDGET_NOT_FOUND));
}

//---------------------------------------------------------------------
//button handling
PinEvent onLeftButtonChanged(PinEvent event){
	Serial.println("Left button changed");
	return lampLeft.constructPinEventSetState(event);
}

PinEvent onRightButtonChanged(PinEvent event){
	Serial.println("right button changed");
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
	//Serial.println("processEvent handled "+event.getText());
	if(!event.isValid() || !event.isNotEmpty()){
		return PinEvent();
	}
	/*
	if(lampLeft.isDispatcherOfEvent(event) && event.getKind()==PIN_EVENT_STATE_UPDATED){
		webSocketHelper.sendMessageToAll(lampLeftWidget.getWsText());
	}

	if(lampRight.isDispatcherOfEvent(event) && event.getKind()==PIN_EVENT_STATE_UPDATED){
		webSocketHelper.sendMessageToAll(lampRightWidget.getWsText());
	}
*/
	deviceHelper.printDeviceDiagnostic();

	return mqttHelper.processEvent(event);
}

void measureSensors(){
	Serial.println("---Measure sensors---");
	bmeMeasurer.measure();
	luxMeasurer.measure();
/*
	webSocketHelper.sendMessageToAll(bmeMeasurer.getWsText());
	webSocketHelper.sendMessageToAll(luxMeasurer.getWsText());
*/
	deviceHelper.printDeviceDiagnostic();
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
}

void sendInitDataToClient(){
	//Serial.println("New client connected");
	//webSocketHelper.sendMessageToAll(timeClient.getWsText(false,':'));
	//webSocketHelper.sendMessageToAll(lampLeftWidget.getWsText());
	//webSocketHelper.sendMessageToAll(lampRightWidget.getWsText());
	//webSocketHelper.sendMessageToAll(bmeMeasurer.getWsText());
	//webSocketHelper.sendMessageToAll(luxMeasurer.getWsText());
}

//---------------------------------------------------
/*
 void processTimeClientEvent(){
	webSocketHelper.sendMessageToAll(timeClient.getWsText(false,':'));
}
//--------------------------websockets----------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  //Serial.println("num="+String(num)+" type="+String(type)+" length="+String(length));
	Serial.println("-----------Web socket event-----");
  if(type==WStype_CONNECTED){
	  Serial.println("Connected");
	  sendInitDataToClient();
  }
  if(type==WStype_DISCONNECTED){
  	  Serial.println("Disconnected");
    }

  if (type == WStype_TEXT){
   Serial.print("num=");Serial.print(num);
   Serial.print("type=");Serial.print(type);
   Serial.print("length=");Serial.print(length);
   Serial.println();

   String message="";

   for(size_t i = 0; i < length; i++){
	   Serial.print((char) payload[i]);
	   message+=(char) payload[i];
   }


   //if(message=="__ping__"){
   //webSocketHelper.sendMessageToAll("__pong__");
   //}
   Serial.println();
  }
}

 */
