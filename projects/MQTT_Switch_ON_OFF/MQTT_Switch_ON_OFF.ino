#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Looper.h"
#include "Loopable.h"
//#include "WiFiHelper.h"

#include "I2Chelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"
#include "PinExternalFuncUint16t.h"

#include "BH1750.h"
#include <Adafruit_BME280.h>

#define FIRMVARE_VERSION "b@l@bas-soft SENSORZ v0.0.5"
//Robot Dyn//vem  // 12E
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp           PIN_WIRE_SCL
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue    PIN_WIRE_SDA
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed
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

int humanNotPresentInterval=20000;
//const char *ssid = "balabasKiev5"; // Имя роутера
//const char *pass = "wuWylKegayg2wu22"; // Пароль роутера

#define BUFFER_SIZE 100

EspSettingsBox espSettingsBox("/values2.txt","",true);

//WiFiClient wclient;
ESP8266WebServer server ( 80 );
//PubSubClient client(mqtt_server, mqtt_port, callback, wclient);

/*
WiFiHelper wiFiHelper(&espSettingsBox,&clearDisplay,displayLineFunction,&ledOnFunction,&ledOffFunction,&setupServerUrlsFunction);
*/
void clearDisplay(){};
void displayLineFunction(String message,int row,int column){};
void ledOnFunction(){};
void ledOffFunction(){};
void setupServerUrlsFunction(ESP8266WebServer server){};

I2Chelper i2cBus(D1_PIN,D2_PIN,false);

BH1750 lightMeter;
Adafruit_BME280 bme;

//PinExternalFuncUint16t luxMeterPin("LuxMeter",10,setupLuxMeter,processEvent,10000,getLuxVal);

String subscribeTopics[3]={"topic/ESP8266Topic""topic/SwitchLeft","topic/SwitchRight"};
//MqttHelper mqttHelper(&espSettingsBox,subscribeTopics,processEvent,processMqttEvent);

TimeTrigger bmeTrigger(0,30000,false,measureBme280);
TimeTrigger luxTrigger(0,30000,false,getLuxVal);
TimeTrigger wiFiTrigger(0,10000,false,checkWiFi);

TimeTrigger humanPresentTrigger(0,humanNotPresentInterval,false,onHumanPresentTrigger);

PinDigital buttonLeft("SwitchLeft",D7_PIN,processEvent);
PinDigital buttonRight("SwitchRight",D6_PIN,processEvent);

PinDigital lampLeft("LampLeft",D5_PIN,OUTPUT,processEvent,CHANGE,HIGH);
PinDigital lampRight("LampRight",D8_PIN,OUTPUT,processEvent,CHANGE,HIGH);

PinDigital signalLed("signalLED",D4_PIN,OUTPUT,processEvent,CHANGE,LOW,HUMAN_NOT_PRESENTED);

PinDigital IrDetector("IrDetector2",D3_PIN,processEvent);

Loopable *loopArray[]={&humanPresentTrigger,&luxTrigger,&bmeTrigger,&wiFiTrigger,&buttonLeft,&buttonRight,&lampLeft,&lampRight,&IrDetector};
Looper looper(loopArray,9);

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Start DeviceId="+espSettingsBox.DeviceId);

  i2cBus.init();
  setupBme();
  setupLuxMeter();

  setupWiFi();
  setupServer();

  looper.displayDetails();
}

void loop() {
 looper.loop();
 //wiFiTrigger.loop();
 server.handleClient();
}

void setupBme(){
	Serial.print("Init BME-280");
	bool status=bme.begin();
	Serial.print(" status="+String(status));
	Serial.println("...done");

	bmeTrigger.setActive(true);
}

void measureBme280(){
	Serial.print("Temperature = ");
	Serial.print(bme.readTemperature());
	Serial.println(" *C");

	Serial.print("Pressure = ");
	Serial.print(bme.readPressure() / 100.0F);
	Serial.println(" hPa");

	Serial.print("Approx. Altitude = ");
	Serial.print(bme.readAltitude(1013.25));
	Serial.println(" m");

	Serial.print("Humidity = ");
	Serial.print(bme.readHumidity());
	Serial.println(" %");

	Serial.println("---------------------------------");
}

void setupLuxMeter(){
	Serial.print("Init luxmeter");
	lightMeter.begin();
	luxTrigger.setActive(true);
	Serial.println("...done");
}
uint16_t getLuxVal(){
	Serial.print("Read LuxMeter val=");
	uint16_t val=lightMeter.readLightLevel();
	Serial.print(val);
	Serial.println(" lux");
	Serial.println("---------------------------------");
	return val;
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

	server.on ( "/runCommand", HTTP_GET, processHttpCommand );
	server.on ( "/test", HTTP_GET, processTestCommand );
	server.begin();

	Serial.println(WiFi.getMode());
	Serial.println(WiFi.getAutoConnect());
	Serial.println(WiFi.status());
	Serial.println("----------------------------");

	Serial.println("Server setup completed");
	Serial.println("---------------------");
}
//---------------------------------------------------------------------------------------
void processHttpCommand(){
	if(server.args()!=0 && server.hasArg("command")){
		String command=server.arg("command");

		PinEvent result=processEvent(PinEvent(command));

		delay(1);
		server.send ( 200, "text/html", "{\"status\":\"Ok\",\"command\":"+command+",\"result\":"+result.getText()+"}" );
	}

	server.send(500,"text/html","{\"status\":\"Failed\",\"command\":\"No command received\"}");
}

void processTestCommand(){
	server.send ( 200, "text/html", "I'm here" );
}
//-------------------------------------------------------------

PinEvent processEvent(PinEvent event){
	/*
	 //kind:bubble:pinId:oldVal:val:strVal:dispatcherName:targetName:

		http://192.168.0.178/runCommand?command=PE_SU:1:14:0:1:PE_SU:SwitchLeft:PE_ANY:
		http://192.168.0.178/runCommand?command=PE_SU:1:14:1:0:PE_SU:SwitchLeft:PE_ANY:
		http://192.168.0.178/runCommand?command=PE_SU:1:15:0:1:PE_SU:SwitchRight:PE_ANY:
		http://192.168.0.178/runCommand?command=PE_SU:1:15:1:0:PE_SU:SwitchRight:PE_ANY:
	*/

	Serial.println(" ProcessEvent "+event.getText());
	PinEvent newEvent=PinEvent();

	if(!event.isValid()){
		return newEvent;
	}

	if(event.isEventOfKind(PIN_EVENT_STATE_GET)){
		Serial.println("-----GET STATE EVENT PROCESS-----");
		if(!newEvent.isValid())
			newEvent=lampLeft.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=lampRight.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=buttonLeft.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=buttonRight.processEventNow(event);
	}

	if(event.isEventOfKind(PIN_EVENT_STATE_SET)){
		Serial.println("-----SET STATE EVENT PROCESS-----");
		if(!newEvent.isValid())
			newEvent=lampLeft.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=lampRight.processEventNow(event);

		newEvent.setIsBubble(false);
	}

	if(event.isEventOfKind(PIN_EVENT_STATE_UPDATED)){
		Serial.println("-----UPDATED STATE EVENT PROCESS-----");
		if(buttonLeft.isDispatcherOfEvent(event)){
			newEvent=lampLeft.constructPinEventSetState(event);
		}
		if(buttonRight.isDispatcherOfEvent(event)){
			newEvent=lampRight.constructPinEventSetState(event);
		}
		if(IrDetector.isDispatcherOfEvent(event)){
			Serial.println("ir change event processed");

			if(event.getVal()==HIGH){
				onMovementDetected();
			} else{
				onNoMovementDetected();
			}
		}

	}

	if(!newEvent.isEmpty()){
		Serial.print("New event ="+newEvent.getText());

		if(newEvent.isValid()){

			if(newEvent.isBubble()){
				Serial.println("...start send");
				//mqttHelper.publish(newEvent.getText());
			}else{
				Serial.println("...not sent");
			}

			processEvent(newEvent);
		}else{
			Serial.println("...not valid");
		}
	}

	return newEvent;
}

boolean humanPresented=false;

void handleHuman(uint8_t val){

	if(val==1){
		signalIrDetection();


	}else{
		humanPresentTrigger.saveTime();
		humanPresentTrigger.setActive(true);

		Serial.println("-------NO MOVEMENT. Start trigger");
	}

	handleLightOnOff();
}

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
}

void handleLightOnOff(){
	signalLed.turnOnOff(humanPresented);
}

void signalIrDetection(){
	lampLeft.changeAndDelay(100, 4);
}

void processMqttEvent(String topic,String message){
	Serial.println("Base process processMqttEvent topic="+topic+" message="+message);
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

const char *mqtt_server = "m23.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 10186; // Порт для подключения к серверу MQTT
const char *mqtt_user = "tpheglmk"; // Логи для подключения к серверу MQTT
const char *mqtt_pass = "QgYRfVzDhQ31"; // Пароль для подключения к серверу MQTT

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

// Функция получения данных от сервера
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
