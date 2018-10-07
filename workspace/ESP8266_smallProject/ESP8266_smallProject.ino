#include "Arduino.h"
#include "LedB.h"

LedB redLed(4);
//The setup function is called once at startup of the sketch
void setup()
{
	redLed.init();
	Serial.println("Started");
}

// The loop function is called in an endless loop
void loop()
{

Serial.println("13");
redLed.turnOn();
delay(2000);
redLed.turnOff();
delay(2000);

}
