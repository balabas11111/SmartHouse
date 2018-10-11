#include <ESP8266WiFi.h>
#include "TM1637.h"
#include "TimeTriggerB.h"
#include "Arduino.h"
#include "EmonLib.h"
#include <math.h>

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

const uint8_t BUTTON_UP_PIN=D8_PIN;
const uint8_t BUTTON_DOVN_PIN=D7_PIN;
const uint8_t LED_PIN=D6_PIN;

const char *ssid     = "balabasKiev5";
const char *password = "wuWylKegayg2wu22";

uint8_t pressedUp=0;
uint8_t pressedDovn=0;

TimeTriggerB displayTrigger(10000,1000,false);

TM1637 tm1637(CLOCK_PIN,DATA_PIN);
int8_t TimeDisp[] = {0x00,0x00,0x00,0x7f};
boolean displayPoint=false;

#define VOLT_CAL 750
EnergyMonitor emon1;
float supplyVoltage=0;

float voltTotal=0;
uint8_t maxIter=1;
uint8_t iter=0;

long lastTime=0;

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  tm1637.set();
  tm1637.init();
  displayTrigger.setActive(true);

  pinMode(BUTTON_UP_PIN, INPUT);
  pinMode(BUTTON_DOVN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  emon1.voltage(0, VOLT_CAL, 1.7);


  Serial.println("STarted");
}

void loop() {

  measureVolts();
/*
  if(iter==maxIter){

	  supplyVoltage=voltTotal/maxIter;
	  voltTotal=0;
	  iter=0;

	  displayVoltage(supplyVoltage);

	  Serial.print("time=");
	  Serial.println(millis()-lastTime);
	  lastTime=millis();
  }else{
	  voltTotal+=supplyVoltage;
	  iter++;
  }
*/


  if(displayTrigger.checkTrigger()){

	  displayVoltage(supplyVoltage);
	  displayTrigger.saveTime();
  }

  checkDovnButton();
  checkUpButton();


}



void measureVolts(){
	emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out

	  supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable

	  Serial.println(supplyVoltage);
}

void displayVoltage(float val){
		boolean update=false;

		int8_t TimeDispTmp[] = {0x00,0x00,0x00,0x7f};

		TimeDispTmp[0] = val / 100;
		TimeDispTmp[1] = (val-TimeDispTmp[0]*100)/10;
		TimeDispTmp[2] = round((val-TimeDispTmp[0]*100-TimeDispTmp[1]*10));

		if(TimeDispTmp[2]>9){
			//Serial.print("--shifted--");
			//Serial.println(TimeDispTmp[2]);
			TimeDispTmp[1]+=1;
			TimeDispTmp[2]=0;
		}
/*
		Serial.print(TimeDispTmp[0]);
		Serial.print("-");
		Serial.print(TimeDispTmp[1]);
		Serial.print("-");
		Serial.println(TimeDispTmp[2]);
		Serial.println("---------");
*/

		for(uint8_t i=0;i<3;i++){
			if(TimeDispTmp[i]!=TimeDisp[i]){
				update=true;
				TimeDisp[i]=TimeDispTmp[i];
			}
		}

		if(update){
			tm1637.display(TimeDisp);
		}
}

void checkUpButton(){
	 uint8_t touchValue = digitalRead(BUTTON_UP_PIN);

	  if(touchValue!=pressedUp){
		  if (touchValue == HIGH)
		  {
			  Serial.println("TOUCHED UP");
			  digitalWrite(LED_PIN, HIGH);
		  }else{
			  Serial.println("released UP");
			  digitalWrite(LED_PIN, LOW);
		  }
	  }

	  pressedUp=touchValue;
}

void checkDovnButton(){
	 uint8_t touchValue = digitalRead(BUTTON_DOVN_PIN);

	  if(touchValue!=pressedDovn){
		  if (touchValue == HIGH)
		  {
			  Serial.println("TOUCHED DOVN");
			  digitalWrite(LED_PIN, HIGH);
		  }else{
			  Serial.println("released DOVN");
			  digitalWrite(LED_PIN, LOW);
		  }
	  }

	  pressedDovn=touchValue;
}
