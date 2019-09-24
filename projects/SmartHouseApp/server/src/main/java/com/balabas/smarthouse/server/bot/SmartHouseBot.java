package com.balabas.smarthouse.server.bot;

import org.springframework.beans.factory.annotation.Value;
import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.meta.exceptions.TelegramApiRequestException;

import lombok.AllArgsConstructor;
import lombok.extern.log4j.Log4j2;

@Log4j2
@AllArgsConstructor
public class SmartHouseBot extends TelegramLongPollingBot {

	@Value("${telegram.bot.apiToken}")
	private String botToken;

	@Value("${telegram.bot.botName}")
	private String botName;

	@Override
	public void onUpdateReceived(Update update) {
		if (update.hasMessage()) {
			Message message = update.getMessage();

			if (message.hasText()) {
				sendTextMessage(message.getChatId().toString(), message.getText());
			}
		}

	}

	public void sendTextMessage(String chatId, String text) {
		SendMessage sendMessageRequest = new SendMessage();
		sendMessageRequest.setChatId(chatId);
		sendMessageRequest.setText(text);
		try {
			execute(sendMessageRequest);
		} catch (TelegramApiException e) {
			log.error(e);
		}
	}

	@Override
	public String getBotUsername() {
		return botName;
	}

	@Override
	public String getBotToken() {
		return botToken;
	}

	@Override
	public void clearWebhook() throws TelegramApiRequestException {
	}

}
