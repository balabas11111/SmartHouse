#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include "TM1637.h"
#include "TimeTriggerB.h"
#include "Arduino.h"

#define DEBUG_NTPClient 1

//Robot Dyn//vem  // 12E
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp
#define D2_PIN 4  //GPIO04  //OLED
#define D3_PIN 0  //GPIO00  //OLED
#define D4_PIN 2  //GPIO02  //RedLed
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button

#define CLOCK_PIN 5 //D1_PIN
#define DATA_PIN 4  //D2_PIN

const uint8_t BUTTON_PIN=D8_PIN;
const uint8_t LED_PIN=D7_PIN;

const char *ssid     = "balabasKiev5";
const char *password = "wuWylKegayg2wu22";

int pressed=0;

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

TimeTriggerB displayTrigger(10000,1000,false);

TM1637 tm1637(CLOCK_PIN,DATA_PIN);
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
boolean displayPoint=false;

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  tm1637.set();
  tm1637.init();
  displayTrigger.setActive(true);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

}

void loop() {
  timeClient.update();

  if(displayTrigger.checkTrigger()){
	  displayTime(timeClient.getHours(),timeClient.getMinutes());
	  displayTrigger.saveTime();
  }

  int touchValue = digitalRead(BUTTON_PIN);

  if(touchValue!=pressed){
	  if (touchValue == HIGH)
	  {
		  Serial.println("TOUCHED");
		  digitalWrite(LED_PIN, HIGH);
	  }else{
		  Serial.println("released");
		  digitalWrite(LED_PIN, LOW);
	  }
  }

  pressed=touchValue;


}

void displayTime(int hours,int minutes){
	//Serial.println(timeClient.getFormattedTime());
	displayPoint=!displayPoint;

	if(displayPoint){
		tm1637.point(POINT_ON);
	}else{
		tm1637.point(POINT_OFF);
	}

	boolean update=false;

	int8_t TimeDispTmp[] = {0x00,0x00,0x00,0x00};

	TimeDispTmp[0] = hours / 10;
	TimeDispTmp[1] = hours % 10;
	TimeDispTmp[2] = minutes / 10;
	TimeDispTmp[3] = minutes % 10;

	for(uint8_t i=0;i<4;i++){
		if(TimeDispTmp[i]!=TimeDisp[i]){
			update=true;
			TimeDisp[i]!=TimeDispTmp[i];
		}
	}

	if(update){
		tm1637.display(TimeDisp);
	}

}
