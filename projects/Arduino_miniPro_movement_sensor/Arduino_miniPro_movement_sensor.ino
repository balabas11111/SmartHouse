#include "Arduino.h"
#include "LedB.h"


LedB greenLed(13);
//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	greenLed.init();
	Serial.begin(9600);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	greenLed.turnOn();
	delay(1000);
	greenLed.turnOff();
	delay(1000);
}
