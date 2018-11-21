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

class MessageSenderTelegram: public Initializable{
public:
	MessageSenderTelegram(EspSettingsBox* espSettingsBox){
		this->espSettingsBox=espSettingsBox;

	}
	virtual ~MessageSenderTelegram();

	virtual boolean initialize(boolean _init){
		Serial.println("Init Telegram bot");
		bot=new UniversalTelegramBot(espSettingsBox->telegramApiKey,client);

		Serial.println("done");
		return true;
	}

	void sendMessage(String message){
		bot.sendMessage(espSettingsBox->telegramReceivers, message);
	}

private:
	WiFiClientSecure client;
	UniversalTelegramBot bot;//(BOTtoken, client);
	EspSettingsBox* espSettingsBox;
};

#endif /* LIBRARIES_HELPERS_MESSAGESENDERTELEGRAM_H_ */
