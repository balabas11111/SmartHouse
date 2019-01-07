/*
 * projectconsts.h
 *
 *  Created on: Jan 2, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef PROJECTCONSTS_H_
#define PROJECTCONSTS_H_

#define DEVICE_FIRMWARE_VER "v 2.0"
#define DEVICE_KIND "NTPclock Alarm BME-280 PIR 4D_display ThingSpeak"
#define DEVICE_DESCR "NTP часы, будильник, Температура/Влажность, Датчик движения"
#define DEVICE_LOCATION "Размещение часов"

#define GET_FILES_ALLOWED 3

/* NTP Clock Device with:
//-------------------------------------------------------------
 4 digits display;
 NTP synchronisation;
 BME-280
 PIR sensor;
//-------------------------------------------------------------
 Connection

  PirSensor          D4

  SignalLed PIR      D0
  I2C                D1, D2
  DS18D20            D3

  TM1637 - Display   D6 D7

  Beeper             D5
  Button             D8
 */


//specific settings for project
//#define HAS_LOCAl_CLOCK 1


#endif /* PROJECTCONSTS_H_ */
