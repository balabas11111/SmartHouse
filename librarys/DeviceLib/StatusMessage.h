/*
 * AbstractItemReq.h
 *
 *  Created on: Nov 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_STATUSMESSAGE_H_
#define LIBRARIES_DEVICELIB_STATUSMESSAGE_H_

#include "Arduino.h"
#include "ESP_Consts.h"

//---------------------Abstract Statuses---------------------------
const int8_t STATUS_UNKNOWN_INT            PROGMEM =-1;

const uint8_t STATUS_OK_INT                PROGMEM =0;
const uint8_t STATUS_ERROR_INT             PROGMEM =1;
const uint8_t STATUS_NOT_FOUND_INT         PROGMEM =2;
const uint8_t STATUS_INVALID_LENGTH_INT    PROGMEM =3;
const uint8_t STATUS_PARSE_ERROR_INT       PROGMEM =4;
const uint8_t STATUS_UPDATE_ERROR_INT      PROGMEM =5;
const uint8_t STATUS_PARAM_NOT_FOUND_INT   PROGMEM =6;
const uint8_t STATUS_VALIDATION_ERROR_INT  PROGMEM =7;
const uint8_t STATUS_ITEM_NOT_FOUND_INT    PROGMEM =8;
const uint8_t STATUS_FAILED_INT            PROGMEM =9;
const uint8_t STATUS_SEND_ERROR_INT        PROGMEM =10;

const PROGMEM char STATUS_UNKNOWN[]            = "UNKNOWN";

const PROGMEM char STATUS_OK[]                 = "Ok";
const PROGMEM char STATUS_ERROR[]              = "Error";
const PROGMEM char STATUS_NOT_FOUND[]          = "NotFound";
const PROGMEM char STATUS_INVALID_LENGTH[]     = "InvalidLength";
const PROGMEM char STATUS_PARSE_ERROR[]        = "ParseError";
const PROGMEM char STATUS_UPDATE_ERROR[]       = "UpdateError";
const PROGMEM char STATUS_PARAM_NOT_FOUND[]    = "ParamNotFound";
const PROGMEM char STATUS_VALIDATION_ERROR[]   = "ValidationError";
const PROGMEM char STATUS_ITEM_NOT_FOUND[]     = "ItemNotFound";
const PROGMEM char STATUS_FAILED[]             = "Failed";
const PROGMEM char STATUS_SEND_ERROR[]         = "SendError";

const char* const STATUSES[] PROGMEM=
{
		STATUS_OK,
		STATUS_ERROR,
		STATUS_NOT_FOUND,
		STATUS_INVALID_LENGTH,
		STATUS_PARSE_ERROR,
		STATUS_UPDATE_ERROR,
		STATUS_PARAM_NOT_FOUND,
		STATUS_VALIDATION_ERROR,
		STATUS_ITEM_NOT_FOUND,
		STATUS_FAILED,
		STATUS_SEND_ERROR
};

const PROGMEM char STATUS_PREF[] = "{\"status\": \"";
const PROGMEM char STATUS_MIDL1[] = "\",\"statusInt\": \"";
const PROGMEM char STATUS_MIDL2[] = "\",\"message\": \"";
const PROGMEM char STATUS_SUFF[] = "\"}";

class StatusMessage{
public:
	StatusMessage(int8_t status){
		this->status=status;
		this->message="";
	}

	StatusMessage(int8_t status,String message){
		this->status=status;
		this->message=message;
	}

	static String constructJSON(int8_t status,String message){
		return StatusMessage(status,message).getJson();
	}

	String getJson(){
		String result= FPSTR(STATUS_PREF);
				result+=getStatusName();
				result+=FPSTR(STATUS_MIDL1);
				result+=String(status);
				result+=FPSTR(STATUS_MIDL2);
				result+=getMessage();
				result+=FPSTR(STATUS_SUFF);
		return result;
	}

	void setValues(int8_t status,String message){
		this->status=status;
		this->message=message;
	}

	void setStatus(int8_t status){ this->status=status;}

	void setMessage(String message){this->message=message;}

	String getStatusName(){
		if(status<0 || status>=sizeof(STATUSES)){
			return FPSTR(STATUS_UNKNOWN);
		}
		return FPSTR(STATUSES[status]);
	}

	String getMessage(){
		if(message==NULL || message.length()==0){
			return getStatusName();
		}
		return message;
	}

	boolean isOk(){
		return this->status == STATUS_OK_INT;
	}
private:
	int8_t status;
	String message;
};

#endif /* LIBRARIES_DEVICELIB_STATUSMESSAGE_H_ */
