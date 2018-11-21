/*
 * Looper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_MESSAGESENDER_H_
#define LIBRARIES_PINDIGITAL_MESSAGESENDER_H_

#include <Arduino.h>

class MessageSender{

public:
	virtual MessageSender(String recepientsStr){
		initialized=false;
	};

	virtual ~MessageSender(){};

	void parseRecepientString(String recepientsStr){
		 int ind=recepientsStr.indexOf(";");
		    uint8_t count=0;

		    while (ind!=-1){
		    	count++;
		    	ind=recepientsStr.indexOf(";",ind+1);
		    }

		    if(count==0){
		    	return;
		    }

		    String adresses[count];

		    uint8_t startIndex=0;
		    uint8_t endIndex=recepientsStr.indexOf(";");
		    uint8_t index=0;

		    while(endIndex!=-1){
		    	adresses[index]=recepientsStr.substring(startIndex,endIndex);
		    	index++;
		    	startIndex=endIndex+1;
		    	endIndex=recepientsStr.indexOf(";",startIndex);
		    	if(endIndex==-1){
		    		adresses[index]=recepientsStr.substring(startIndex);
		    	}
		    }

		    Serial.println(recepientsStr);

		    receipients=adresses;
		    receipientsCount=count;

		    for(uint8_t i=0;i<sizeof(adresses);i++){
		    	Serial.println(adresses[i]);
		    }

		    initialized=true;
	}

	virtual void sendMessage(String message,String receiver)=0;

	virtual void sendMessageToAllRecipients(String message){
		if(this->receipientsCount==0 || !initialized){
			return;
		}

		for(uint8_t i=0;i<receipientsCount;i++){
			sendMessage(message,receipients[i]);
		}
	}

private:
	String* receipients;
	uint8_t receipientsCount;

	boolean initialized;
};


#endif /* LIBRARIES_PINDIGITAL_LOOPER_H_ */
