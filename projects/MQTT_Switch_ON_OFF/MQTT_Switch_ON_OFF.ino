#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Looper.h"
#include "Loopable.h"
#include "PinEventProcessor.h"
//#include "WiFiHelper.h"
#include "FS.h"
#include "I2Chelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"

#include <BME280_Measurer.h>
#include <BH1750_Measurer.h>

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

const int humanNotPresentInterval=30000;
const int sensorsInterval=60000;

//const char *ssid = "balabasKiev5"; // Èìÿ ðîóòåðà
//const char *pass = "wuWylKegayg2wu22"; // Ïàðîëü ðîóòåðà

EspSettingsBox espSettingsBox("/values2.txt","",true);

WiFiClient wclient;
ESP8266WebServer server ( 80 );
//PubSubClient client(mqtt_server, mqtt_port, callback, wclient);

/*
WiFiHelper wiFiHelper(&espSettingsBox,&clearDisplay,displayLineFunction,&ledOnFunction,&ledOffFunction,&setupServerUrlsFunction);

void clearDisplay(){};
void displayLineFunction(String message,int row,int column){};
void ledOnFunction(){};
void ledOffFunction(){};
void setupServerUrlsFunction(ESP8266WebServer server){};
*/
I2Chelper i2cBus(D1_PIN,D2_PIN,false);

String subscribeTopics[]={"topic/ESP8266Topic","topic/SwitchLeft","topic/SwitchRight"};
MqttHelper mqttHelper(&espSettingsBox,subscribeTopics,ARRAY_SIZE(subscribeTopics),callback,wclient);

TimeTrigger sensorsTrigger(0,sensorsInterval,false,measureSensors);
TimeTrigger wiFiTrigger(0,10000,false,checkWiFi);

TimeTrigger humanPresentTrigger(0,humanNotPresentInterval,false,onHumanPresentTrigger);

PinDigital pirDetector(VAR_NAME(pirDetector),D3_PIN,onPirWasChanged);

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7_PIN,processEvent,onButtonChanged);
PinDigital buttonRight(VAR_NAME(buttonRight),D6_PIN,processEvent,onButtonChanged);

PinDigital signalLed(VAR_NAME(signalLED),D4_PIN,OUTPUT,nullptr,CHANGE,LOW,HUMAN_NOT_PRESENTED);

PinDigital lampLeft(VAR_NAME(lampLeft),D5_PIN,OUTPUT,processEvent,CHANGE,HIGH);
PinDigital lampRight(VAR_NAME(lampRight),D8_PIN,OUTPUT,processEvent,CHANGE,HIGH);

Loopable* loopArray[]={&humanPresentTrigger,&sensorsTrigger,&buttonLeft,&buttonRight,&lampLeft,&lampRight,&pirDetector,&mqttHelper};
//Loopable* loopArray[]={&humanPresentTrigger,&buttonLeft,&buttonRight,&pirDetector};

Looper looper(loopArray,ARRAY_SIZE(loopArray));

BME280_Measurer bmeMeasurer("0",VAR_NAME(bmeMeasurer));
BH1750_Measurer luxMeasurer("1",VAR_NAME(luxMeasurer));

HtmlWidget* widgetsArray[]={&bmeMeasurer,&luxMeasurer};

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Start DeviceId="+espSettingsBox.DeviceId);

  SPIFFS.begin();

  i2cBus.init();
  //sensorsTrigger.init();
  bmeMeasurer.init();
  luxMeasurer.init();

  setupWiFi();
  setupServer();

  mqttHelper.init();

  looper.displayDetails();

  Serial.println("var="+String(VAR_NAME(buttonRight)));
}

void loop() {
 looper.loop();
 server.handleClient();
}

//-------------------------------------------------------------
boolean leftButtonPressed=false;
boolean rightButtonPressed=false;
boolean pirDetected=false;

PinEvent onLampLeftChanged(PinEvent event){
	Serial.println("Left lamp changed");
	return PinEvent();
}

PinEvent onLampRightChanged(PinEvent event){
	Serial.println("right lamp changed");
	return PinEvent();
}

PinEvent onButtonLeftChanged(PinEvent event){
	leftButtonPressed=buttonLeft.isOn();
	Serial.println("Left button changed"+String(leftButtonPressed));

	lampLeft.turnOnOff(leftButtonPressed);
	return PinEvent();
}

PinEvent onButtonRightChanged(PinEvent event){
	rightButtonPressed=buttonRight.isOn();
	Serial.println("Right button changed"+String(rightButtonPressed));
	lampRight.turnOnOff(rightButtonPressed);
	return PinEvent();
}

PinEvent onIrChanged(PinEvent event){
	pirDetected=pirDetector.isOn();

	if(pirDetected){
		onMovementDetected();
	} else{
		onNoMovementDetected();
	}

	return PinEvent();
}

void measureSensors(){
/*
	Serial.println("SizeOf items="+String(sizeof(loopArray)));
	Serial.println("SizeOf items[0]="+String(sizeof(loopArray[0])));

	Serial.println(ARRAY_SIZE(loopArray));
*/
	Serial.println("FreeHeap="+String(ESP.getFreeHeap()));
	Serial.println("CpuFreqMHz="+String(ESP.getCpuFreqMHz()));
	Serial.println("FlashChipSize="+String(ESP.getFlashChipSize()));
	Serial.println("FreeSketchSpace="+String(ESP.getFreeSketchSpace()));
	Serial.println("ResetReason="+String(ESP.getResetReason()));
	//Serial.println("CpuFreqMHz="+String(ESP.getCpuFreqMHz()));
	//Serial.println("CpuFreqMHz="+String(ESP.getCpuFreqMHz()));

	Serial.println("----------------------------------------------");
	Serial.println(bmeMeasurer.getJson());
	Serial.println("----------------------------------------------");
	Serial.println(luxMeasurer.getJson());
	Serial.println("----------------------------------------------");
}

//---------------------------------------------------------------------------------------
void setupWiFi(){
	Serial.println("-----Setup WiFi-----");
	//wiFiTrigger.setActive(true);

	const char *ssid = const_cast<char*>(espSettingsBox.ssid.c_str());
	const char *password =const_cast<char*>(espSettingsBox.password.c_str());

	Serial.println("sssid="+String(ssid)+" pass="+String(password));

	Serial.print("Status=");Serial.println(WiFi.status());

	if(isWiFIConnected()){
		Serial.println("Disconnecting wifi");
		WiFi.disconnect(0);
	}

	WiFi.mode(WIFI_STA);
	WiFi.enableSTA(true);
	//WiFi.setAutoReconnect(true);

	//WiFi.config(espSettingsBox.localIp,espSettingsBox.gateIp,espSettingsBox.subnetIp,espSettingsBox.dnsIp,espSettingsBox.dnsIp2);
	WiFi.config(0U,0U,0U,espSettingsBox.dnsIp,espSettingsBox.dnsIp2);
	WiFi.begin(ssid,password);

	signalLed.change();
	while(WiFi.status()!=WL_CONNECTED){
		signalLed.changeAndDelay(500);
		checkWiFi();
	}

	signalLed.turnOff();

	Serial.println("WiFi connected");
	/*
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connect result "+WiFi.status());
		return;
	}
	*/

	Serial.println ( "" );
	Serial.print ( "begun with " );
	Serial.println ( WiFi.SSID() );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );
	Serial.print("status=");
	Serial.println(WiFi.status());
	Serial.print(" Mac=");
	Serial.println(WiFi.macAddress());

	Serial.println("------------------------------------");
	signalLed.changeAndDelay(200, 6);
}

boolean isWiFIConnected(){
	return WiFi.status() == WL_CONNECTED;
}


void checkWiFi(){
	WiFi.printDiag(Serial);

	Serial.print("Status=");
	Serial.println(WiFi.status());
	Serial.println("------------------------------");
}

void setupServer(){

	Serial.println("SetupServer");

	Serial.print("WiFi.getMode=");
	Serial.print(WiFi.getMode());
	Serial.print(" WiFi.getAutoConnect=");
	Serial.print(WiFi.getAutoConnect());
	Serial.print(" WiFi.status=");
	Serial.println(WiFi.status());
	Serial.println("----------------------------");

	server.onNotFound(handleNotFound);

	server.on ( "/test", HTTP_GET, handleTest);
	server.on ( "/MeasurerWidgetESP", HTTP_GET, handleHttpWidget);
	server.on ( "/SettingsWidgetESP", HTTP_GET, handleEspSettings);
	server.on ( "/processEvent", HTTP_GET, handleHttpEvent );

	initStaticPagesInWebFolder();

	server.begin();

	Serial.println("Server setup completed");
	Serial.println("---------------------");
}
//---------------------------------------------------------------------------------------
void initStaticPagesInWebFolder(){
	String basePath="/web/static";
	//        /js   /css     /html

	Dir base=SPIFFS.openDir(basePath);

	while(base.next()){
		String subFolder=base.fileName();

		String urlPreffix=subFolder+"/";
		String currentFolder=basePath+subFolder;
		Dir dir=SPIFFS.openDir(currentFolder);

		while(dir.next()){
			String fileName=dir.fileName();
			String url=urlPreffix+fileName;
			size_t size=dir.openFile("r").size();

			String path=basePath+"/"+currentFolder+"/"+fileName;
			Serial.print("Found file "+path+" size="+String(size)+" deploying");

			server.serveStatic(url.c_str(), SPIFFS, path.c_str());
			Serial.println("...done");
		}
	}
}

void handleNotFound(){
	server.send(404,"text/html", "notFound");
}

void handleTest(){
	server.send ( 200, "text/html", "I'm here" );
}

void handleEspSettings(){
	//class ="SettingsWidgetESP"
	const String wnParam="widgetName";

	if(server.hasArg(wnParam)){
		String wiName=server.arg(wnParam);
		Serial.println("PorcessEspSetting="+wiName);

		if(wiName.equals("espSettingsBox.DeviceId")){
			server.send(200, "text/html", espSettingsBox.getHtmlVal(wiName));
		}

		server.send(404, "text/html", "Параметр "+wiName+" не найден");
	}

	server.send(400, "text/html", "widgetName parameter missing");
}

void handleHttpWidget(){
	//class ="MeasurerWidgetESP"
	//    /processWidget   param=widgetName
	const String wnParam="widgetName";

	if(server.hasArg(wnParam)){
		String wiName=server.arg(wnParam);
		Serial.println("PorcessWidget="+wiName);

		if(wiName.equals("espSettingsBox.DeviceId")){
			server.send(200, "text/html", espSettingsBox.DeviceId);
		}

		for(uint8_t i=0;i<ARRAY_SIZE(widgetsArray);i++){
			if(widgetsArray[i]->getName().equals(wiName)){
				server.send(200, "text/html", (widgetsArray[i]->getHtml()));
				return;
			}
		}

		server.send(404, "text/html", "Виджет "+wiName+" не найден");
	}

	server.send(400, "text/html", "widgetName parameter missing");
}

void handleHttpEvent(){
	if(server.args()!=0 && server.hasArg("command")){
		String command=server.arg("command");
		Serial.println("Http command received "+command);
		PinEvent result=processEvent(PinEvent(command).setIsBubble(true));

		delay(1);
		server.send ( 200, "text/html", "{\"status\":\"Ok\",\"command\":\""+command+"\",\"result\":\""+result.getText()+"\"}" );
	}

	server.send(500,"text/html","{\"status\":\"Failed\",\"command\":\"No command received\"}");
}



//-----------------------------------------------------------
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

boolean humanPresented=false;

void onMovementDetected(){
	Serial.println("-------handleMovementDetected");
	signalLed.changeAndDelay(50, 2);

	humanPresented=true;
	humanPresentTrigger.setActive(false);
	handleLightOnOff();
}

void onNoMovementDetected(){
	Serial.println("-------handle NO MovementDetected");
	humanPresentTrigger.saveTime();
	humanPresentTrigger.setActive(true);
	handleLightOnOff();
}

void onHumanPresentTrigger(){
	Serial.println("-------no movement during period");

	humanPresented=false;
	humanPresentTrigger.setActive(false);

	handleLightOnOff();

	processEvent(pirDetector.constructEvent(PIN_EVENT_STATE_UPDATED,true));
}

void handleLightOnOff(){
	signalLed.turnOnOff(humanPresented);
}

PinEvent onPirWasChanged(PinEvent event){
	Serial.println("//On pir changed");
	//On pir changed
	boolean dispatch=!humanPresented;

	if(event.getVal()==HIGH){
		onMovementDetected();
	} else{
		onNoMovementDetected();
	}

	processEvent(event.setIsBubble(dispatch));

	return PinEvent();
}

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
	Serial.println("-----------------------------------------");
	Serial.println(" ProcessEvent "+event.getText());
	PinEvent newEvent=PinEvent();

	if(!event.isValid() || !event.isNotEmpty()){
		return newEvent;
	}

	if(event.isBubble()){
		publishEvent(event);
		event.setIsBubble(false);
	}

		PinEventProcessor *eventProcessors[]={&lampLeft,&lampRight,
											&buttonLeft,&buttonRight,
											&bmeMeasurer,&luxMeasurer};

		boolean processed=false;

		for(uint8_t i=0;i<ARRAY_SIZE(eventProcessors);i++){
			newEvent=eventProcessors[i]->processEventNow(event);
			if(newEvent.isNotEmpty()){
				Serial.println(eventProcessors[i]->printProcessInfo(event)+" new->"+newEvent.getText());
				processed=true;
				break;
			}
		}

	if(newEvent.isNotEmpty()){
		//Serial.print("New event ="+newEvent.getText());
		if(newEvent.isValid()){

			if(newEvent.isBubble()){
				publishEvent(newEvent);
				newEvent.setIsBubble(false);
			}else{
				//Serial.println("...not sent");
			}

			processEvent(newEvent);
		}else{
			//Serial.println("...not valid");
		}
	}
	if(!processed){
		//Serial.println("NO PROCESSOR FOUND "+newEvent.getText());
	}

	return newEvent;
}

void publishEvent(PinEvent event){
	event.setSendDevice(espSettingsBox.DeviceId);

	String mess=event.getText();
	Serial.print(mess+"...sending");
	boolean res=mqttHelper.publish(mess);

	Serial.println("..."+res?"OK":"FAILED");
}

void signalIrDetection(){
	lampLeft.changeAndDelay(100, 4);
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
}

void callback(char* topic, uint8_t* payload, unsigned int length) {

	String payloadIn = (char*)payload;
	String topicIn=String(topic);

	String msg;
		for(int i=0;i<length;i++){
			msg+=(char)payload[i];
		}

	String messageIn="Message received payload="+msg+" topic="+topic +" length="+length;

	Serial.println(messageIn);
	PinEvent result=processEvent(PinEvent(msg));
}

/*

void connectWifiIfNotConnected(){
	Serial.println("connectWifiIfNotConnected");

	while(!isWiFIConnected()){
		connectWiFi();

		if(!isWiFIConnected()){
			delay(5000);
		}
	}
}

void connectWiFi(){
	if (!isWiFIConnected()) {
	    Serial.print("Connecting to ");
	    Serial.print(ssid);
	    Serial.println("...");
	    WiFi.begin(ssid, pass);

	    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
	    Serial.println("WiFi connected");

		server.on ( "/runCommand", HTTP_GET, processHttpCommand );
		server.begin();

		Serial.println(WiFi.localIP());
	  }
}

const char *mqtt_server = "m23.cloudmqtt.com"; // Èìÿ ñåðâåðà MQTT
const int mqtt_port = 10186; // Ïîðò äëÿ ïîäêëþ÷åíèÿ ê ñåðâåðó MQTT
const char *mqtt_user = "tpheglmk"; // Ëîãè äëÿ ïîäêëþ÷åíèÿ ê ñåðâåðó MQTT
const char *mqtt_pass = "QgYRfVzDhQ31"; // Ïàðîëü äëÿ ïîäêëþ÷åíèÿ ê ñåðâåðó MQTT

const char *baseTopic="topic/";
const char *topic_sub_s1="topic/switch1";
const char *topic_sub_s2="topic/switch2";

const char *topicBasePath="topic";

void sendEventToTopic(PinEvent event){

	if(event.isValid()){
		String topic=String(baseTopic)+event.getDispatcherName();
		String msg=event.getText();

		publishToTopic((char*) topic.c_str(),msg);
	}
}

void sendData() {
	static int counter = 0;

	  String payload = "{\"micros\":";
	  payload += micros();
	  payload += ",\"counter\":";
	  payload += counter;
	  payload += "}";

	  publishToTopic(baseTopic,payload);

	  ++counter;
	  delay(5000);

  delay(1);
}

boolean publishToTopic(const char *topic, String payload){

	boolean result=false;

	if (client.connected()){
		Serial.print("Sending payload: ");
		Serial.print(payload);

		result=client.publish(topic, (char*) payload.c_str());

		if (result) {
		  Serial.println("...ok");
		}
		else {
		  Serial.println("...failed");
		}
    }

	return result;
}

// Ôóíêöèÿ ïîëó÷åíèÿ äàííûõ îò ñåðâåðà
void callback(char* topic, uint8_t* payload, unsigned int length) {

	String payloadIn = (char*)payload;
	String topicIn=String(topic);

	String msg;
		for(int i=0;i<length;i++){
			msg+=(char)payload[i];
		}

	String messageIn="Message received payload="+msg+" topic="+topic +" length="+length;

	Serial.println(messageIn);
	PinEvent result=processEvent(PinEvent(msg));
}


void processMqtt(){
	if (client.connected()){
	  client.loop();
	  //sendData();
	}
}

boolean isMqttConnected(){
	return client.connected();
}

void connectMqttIfNotConnected(){
	String clState=String(client.state());
	String clConnected=String(client.connected());
	String wifiState=String(wclient.status());
	String wifiConnected=String(wclient.connected());

	//Serial.println("Client State="+clState+" clConnected="+clConnected+" wifiState="+wifiState+" wifiConnected"+wifiConnected);

	if (!client.connected()) {

	    	  String clientName;
	    	  clientName += "esp8266 "+millis();

	    	if (client.connect((char*) clientName.c_str(),mqtt_user,mqtt_pass)) {
	    	    Serial.println("Connected to MQTT broker");
	    	    Serial.print("Base Publish Topic is: ");
	    	    Serial.println(baseTopic);

	    	    if (client.publish(baseTopic, "hello from ESP8266")) {
	    	      Serial.println("Publish ok");
	    	    }
	    	    else {
	    	      Serial.println("Publish failed");
	    	    }

	    	    subscribeToTopic(topic_sub_s1);
	    	    subscribeToTopic(topic_sub_s2);
	    	  }
	    	  else {
	    	    Serial.println("MQTT connect failed");
	    	    Serial.println("Will reset and try again...");
	    	    delay(5000);
	    	    //abort();
	    	  }

	    	Serial.println("IP="+WiFi.localIP().toString());
	    }
}

void subscribeToTopic(const char *topicIn){
	Serial.print("Subscribing to "+String(topicIn));
	client.subscribe(topicIn);
	Serial.println("   ...Subscribed");
}
*/
