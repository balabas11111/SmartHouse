/*
 * StatusMessageConsts.h
 *
 *  Created on: 19 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_STATUSMESSAGECONSTS_H_
#define LIBRARIES_DEVICELIB_STATUSMESSAGECONSTS_H_

#include "Arduino.h"
#include "projectConsts.h"

//---------------------StatusMessage---------------------------
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
const uint8_t STATUS_NOT_IMPLEMENTED_INT   PROGMEM =11;
const uint8_t STATUS_OK_ACCEPTED_INT       PROGMEM =12;
const uint8_t STATUS_OK_SAVED_INT          PROGMEM =13;
const uint8_t STATUS_OK_DELETED_INT        PROGMEM =14;
const uint8_t STATUS_OK_DISABLED_INT       PROGMEM =15;
const uint8_t STATUS_OK_SENT_INT           PROGMEM =16;
const uint8_t STATUS_OK_NO_TO_SEND_INT     PROGMEM =17;

#ifdef DEVICE_LOCALE_RU
	#include "projectConstsLoc_Ru.h"
	#include "Locales/ConstsLoc_Ru.h"
#endif
#ifdef DEVICE_LOCALE_EN
	#include "projectConstsLoc_En.h"
	#include "Locales/ConstsLoc_En.h"
#endif

const PROGMEM char STATUS_UNKNOWN[]            = "UNKNOWN";
const PROGMEM uint STATUS_UNKNOWN_HTTP       = 428;

const char* const STATUSES[] PROGMEM=
{
		"Ok",
		"Error",
		"NotFound",
		"InvalidLength",
		"ParseError",
		"UpdateError",
		"ParamNotFound",
		"ValidationError",
		"ItemNotFound",
		"Failed",
		"SendError",
		"NotImplemented",
		"Accepted",
		"Saved",
		"Deleted",
		"Disabled",
		"Sent",
		"NoDataToSend"
};

const uint STATUSES_HTTP[] PROGMEM=
{
		200,
		500,
		404,
		422,
		406,
		406,
		404,
		406,
		404,
		500,
		523,
		501,
		202,
		202,
		202,
		202,
		202,
		202
};

const PROGMEM char STATUS_statusInt[] = "{\"statusInt\": \"";
const PROGMEM char STATUS_statusCode[] = "\",\"statusCode\"\": \"";
const PROGMEM char STATUS_statusDescr[] = "\",\"statusDescr\"\": \"";
const PROGMEM char STATUS_statusHttp[] = "\",\"statusHttp\"\": \"";
const PROGMEM char STATUS_message[] = "\",\"message\": \"";
const PROGMEM char STATUS_data[] = "\",\"data\": \"";
const PROGMEM char STATUS_SUFF[] = "\"}";

#endif /* LIBRARIES_DEVICELIB_STATUSMESSAGECONSTS_H_ */
