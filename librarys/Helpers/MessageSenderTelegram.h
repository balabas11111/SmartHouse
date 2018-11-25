/*
 * MessageSenderTelegram.h
 *
 *  Created on: Nov 21, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_MESSAGESENDERTELEGRAM_H_
#define LIBRARIES_HELPERS_MESSAGESENDERTELEGRAM_H_

#include "Arduino.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <EspSettingsBox.h>
#include <Initializable.h>
#include <Loopable.h>

class MessageSenderTelegram: public Initializable, public Loopable{
public:
	MessageSenderTelegram(UniversalTelegramBot* bot){
		this->bot=bot;
		initialize(false);
	}
	virtual ~MessageSenderTelegram(){};

	virtual boolean initialize(boolean _init){
		Serial.println("Init Telegram bot");

		if(_init){
			//loop();
		}

		botLastTime=millis();
		Serial.println("done with bot");
		return true;
	}

	virtual boolean loop(){

		if(botLastTime+botInterval<millis()){
			Serial.println("Scan for meesages");

			int numNewMessages = bot->getUpdates(bot->last_message_received + 1);

			while(numNewMessages) {
			  Serial.println("got response");
			  handleNewMessages(numNewMessages);
			  numNewMessages = bot->getUpdates(bot->last_message_received + 1);
			}

			botLastTime = millis();
		}
		return true;
	}

	void handleNewMessages(int numNewMessages) {
	  Serial.println("handleNewMessages");
	  Serial.println(String(numNewMessages));

	  for (int i=0; i<numNewMessages; i++) {
	    String chat_id = String(bot->messages[i].chat_id);
	    String text = bot->messages[i].text;

	    String from_name = bot->messages[i].from_name;

	    Serial.println("from_name");
	    Serial.println(from_name);

	    Serial.println("text");
		Serial.println(text);

		Serial.println("chat_id");
		Serial.println(chat_id);

	    if (from_name == "") from_name = "Guest";

	    if (text == "/ledon") {
	      bot->sendMessage(chat_id, "Led is ON", "");
	    }

	    if (text == "/ledoff") {
	      bot->sendMessage(chat_id, "Led is OFF", "");
	    }

	    if (text == "/status") {
	        bot->sendMessage(chat_id, "Status", "");
	    }

	    if (text == "/options") {
	      String keyboardJson = "[[\"/ledon\", \"/ledoff\"],[\"/status\"]]";
	      bot->sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
	    }

	    if (text == "/start") {
	      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
	      welcome += "This is Reply Keyboard Markup example.\n\n";
	      welcome += "/ledon : to switch the Led ON\n";
	      welcome += "/ledon : to switch the Led ON\n";
	      welcome += "/ledoff : to switch the Led OFF\n";
	      welcome += "/status : Returns current status of LED\n";
	      welcome += "/options : returns the reply keyboard\n";
	      bot->sendMessage(chat_id, welcome, "Markdown");
	    }
	  }
	}

	void sendMessage(String message){
		bot->sendMessage(espSettingsBox->telegramReceivers, message);
	}

private:
	EspSettingsBox* espSettingsBox;
	UniversalTelegramBot* bot;//=new UniversalTelegramBot(espSettingsBox->telegramApiKey,&client);
	unsigned long botLastTime;
	unsigned long botInterval;

};

#endif /* LIBRARIES_HELPERS_MESSAGESENDERTELEGRAM_H_ */
