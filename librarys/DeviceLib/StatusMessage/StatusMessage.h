/*
 * AbstractItemReq.h
 *
 *  Created on: Nov 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_STATUSMESSAGE_H_
#define LIBRARIES_DEVICELIB_STATUSMESSAGE_H_

#include "Arduino.h"
#include "StatusMessageConsts.h"

class StatusMessage{
public:
	String getJson(){
		String result= FPSTR(STATUS_statusInt);
				result+=getStatusIntAsString();
				result+=FPSTR(STATUS_statusCode);
				result+=getStatusCode();
				result+=FPSTR(STATUS_statusDescr);
				result+=getStatusDescr();
				result+=FPSTR(STATUS_statusHttp);
				result+=getStatusHttpAsString();
				result+=FPSTR(STATUS_message);
				result+=getMessage();
				result+=FPSTR(STATUS_data);
				result+=getData();
				result+=FPSTR(STATUS_SUFF);
		return result;
	}

	StatusMessage(int8_t status){
		this->set(status, "", "");
	}

	StatusMessage(int8_t status,String message){
		this->set(status, message, "");
	}

	StatusMessage(int8_t status,String message,String data){
		this->set(status, message, data);
	}

	static String constructJSON(int8_t status,String message,String data){
		return StatusMessage(status,message,data).getJson();
	}

	void set(int8_t status,String message,String data){
		setStatusCode(status);
		setMessage(message);
		setData(data);
	}

	void setStatusCode(int8_t status){ this->status=status;}

	void setMessage(String message){
		if(message!=NULL && message.length()!=0){this->message=message;}
	}

	void setData(String data){
		if(data!=NULL && data.length()!=0){this->data=data;}
	}

	int8_t getStatusInt(){
		if(isStatusUnknown(status)){
			return STATUS_UNKNOWN_INT;
		}
		return this->status;
	}

	String getStatusCode(){
		return getStatusCodeByIndex(status);
	}

	String getStatusDescr(){
		return getStatusDescrByIndex(status);
	}

	uint getStatusHttp(){
		return getStatusHttpByIndex(status);
	}

	String getMessage(){
		if(message==NULL || message.length()==0){
			return getStatusDescr();
		}
		return getStatusDescr()+": "+message;
	}

	String getData(){
		if(data==NULL){
			return "";
		}
		return data;
	}

	static boolean isStatusUnknown(int8_t i){
		return (i<0 || i>=sizeof(STATUSES));
	}

	boolean isOk(){
		return (getStatusInt() == STATUS_OK_INT
				|| getStatusInt()==STATUS_OK_ACCEPTED_INT
				|| getStatusInt()==STATUS_OK_SAVED_INT
				|| getStatusInt()==STATUS_OK_DELETED_INT
				);
	}

	static String getStatusCodeByIndex(int8_t i){
		if(isStatusUnknown(i)){
			return getUnknownStatusCode();
		}
		return FPSTR(STATUSES[i]);
	}

	static String getStatusDescrByIndex(int8_t i){
		if(isStatusUnknown(i)){
			return FPSTR(STATUS_UNKNOWN_DESCR);
		}
		return FPSTR(STATUSES_DESCR[i]);
	}

	static uint getStatusHttpByIndex(int8_t i){
		if(isStatusUnknown(i)){
			return STATUS_UNKNOWN_HTTP;
		}
		return STATUSES_HTTP[i];
	}
protected:
	String getStatusHttpAsString(){
		return String(getStatusHttp());
	}

	String getStatusIntAsString(){
		return String(getStatusInt());
	}


	static String getUnknownStatusCode(){
		return FPSTR(STATUS_UNKNOWN);
	}

private:
	int8_t status;
	String message;
	String data;
};

#endif /* LIBRARIES_DEVICELIB_STATUSMESSAGE_H_ */
