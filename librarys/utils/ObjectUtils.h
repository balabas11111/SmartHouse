/*
 * ObjectUtils.h
 *
 *  Created on: 3 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_UTILS_OBJECTUTILS_H_
#define LIBRARIES_UTILS_OBJECTUTILS_H_

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var
#define UNUSED(expr) do { (void)(expr); } while (0)

#include "IPAddress.h"

class ObjectUtils {
public:
	virtual ~ObjectUtils(){};

	static IPAddress stringToIp(const char* ipStr){

		Serial.print(FPSTR(" ipStr="));
		Serial.print(ipStr);

		IPAddress addr=IPAddress(ipStrToNum(ipStr));

		Serial.print(FPSTR(" addr="));
		Serial.print(addr);

		return addr;
	}

	/**
	 * ipStrToNum
	 * works for IPV4 only
	 * parses  "10.1.1.200" and "10,1,1,200" strings
	 * extra spaces ignored  eg "10, 1, 1, 200" is OK also
	 * return uint32_t for passing to ipAddress( );
	 */
	static uint32_t ipStrToNum(const char* ipStr) {
	  const int SIZE_OF_NUMS = 4;
	  union {
		uint8_t bytes[SIZE_OF_NUMS];  // IPv4 address
		uint32_t dword;
	  } _address;
	  _address.dword = 0; // clear return

	  int i = 0;
	  uint8_t num = 0; // start with 0
	  while ((*ipStr) != '\0') {
	    // while not end of string
	    if ((*ipStr == '.') || (*ipStr == ',')) {
	      // store num and move on to next position
	      _address.bytes[i++] = num;
	      num = 0;
	      if (i>=SIZE_OF_NUMS) {
	        break; // filled array
	      }
	    } else {
	      if (*ipStr != ' ') {     	// skip blanks
	        num = (num << 3) + (num << 1); // *10 = *8+*2
	        num = num +  (*ipStr - '0');
	      }
	    }
	    ipStr++;
	  }
	  if (i<SIZE_OF_NUMS) {
	    // store last num
	    _address.bytes[i++] = num;
	  }
	  return _address.dword;
	}

	static void initSerial(){
		Serial.begin(115200);
		delay(2000);
		Serial.println(FPSTR("EntityLib Device starting"));
	}

	static void printHeap(){
		Serial.print(FPSTR("heap = "));
		Serial.println(ESP.getFreeHeap());
	}

	static void printMillis(){
		Serial.print(FPSTR("millis = "));
		Serial.println(millis());
	}

	static void printInt8Arr(uint8_t* arr){
		Serial.print(FPSTR("Arr= "));
		for(int i=0;i<8;i++){
			Serial.print(arr[i]);
		}
		Serial.println();
	}

	static void printTimeHeap(unsigned long start){
		unsigned long total = millis()-start;
		Serial.print(FPSTR("Time = "));
		Serial.print(total);
		Serial.print(FPSTR(" heap = "));
		Serial.println(ESP.getFreeHeap());
	}


};

#endif /* LIBRARIES_UTILS_OBJECTUTILS_H_ */
