package com.balabas.smarthouse.telegram.bot.handler;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.meta.exceptions.TelegramApiRequestException;

import com.balabas.smarthouse.telegram.bot.message.SendMessageBuilder;
import com.balabas.smarthouse.telegram.bot.service.AuthService;

import lombok.extern.log4j.Log4j2;

@Log4j2
public abstract class BaseLogPollingBotHandler  extends TelegramLongPollingBot {

	@Autowired
	protected AuthService authService;
	
	@Autowired
	protected SendMessageBuilder messageBuilder;
	
	public void sendTextMessageToAllUsers(String text){
        authService.getAllowedUserIds().stream().forEach(
                chatId -> sendMessage(messageBuilder.createTextMessage(chatId.longValue(), text)));
    }
	
	public boolean sendHtmlMessageToAllUsers(String text){
		boolean sent = true;
		
		for(Integer chatId : authService.getAllowedUserIds()) {
			boolean s = sendMessage(messageBuilder.createHtmlMessage(chatId.longValue(), text));
			
			sent = s && sent;
		}
		
		return sent;
    }
	
	protected void sendMessages(List<SendMessage> msgs){
		msgs.stream().forEach(this::sendMessage);
	}
	
	protected boolean sendMessage(SendMessage msg){
		try {
			execute(msg);
			return true;
		} catch (TelegramApiException e) {
			log.error(e);
		}
		return false;
	}
	
	protected boolean checkAuthorization(Message msg){
		try{
			return authService.isUserIdAllowed(msg.getFrom().getId());
		}catch(Exception e){
			return false;
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
