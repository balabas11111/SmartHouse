package com.balabas.smarthouse.telegram.bot.handler;

import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.meta.exceptions.TelegramApiRequestException;

import com.balabas.smarthouse.telegram.bot.service.AuthService;

import lombok.AllArgsConstructor;
import lombok.extern.log4j.Log4j2;

@Log4j2
@AllArgsConstructor
public class SmartHouseBotHandler extends TelegramLongPollingBot {

	private AuthService authService;

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
		return authService.getBotName();
	}

	@Override
	public String getBotToken() {
		return authService.getBotToken();
	}

	@Override
	public void clearWebhook() throws TelegramApiRequestException {}

}
