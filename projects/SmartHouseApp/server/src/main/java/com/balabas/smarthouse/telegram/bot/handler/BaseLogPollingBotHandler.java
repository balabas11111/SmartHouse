package com.balabas.smarthouse.telegram.bot.handler;

import java.util.Date;

import org.springframework.beans.factory.annotation.Autowired;
import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardRemove;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.meta.exceptions.TelegramApiRequestException;

import com.balabas.smarthouse.telegram.bot.service.AuthService;
import com.balabas.smarthouse.telegram.bot.service.BotMessageConstants;
import com.balabas.smarthouse.telegram.bot.service.Emoji;
import com.balabas.smarthouse.telegram.bot.service.KeyboardService;

import lombok.extern.log4j.Log4j2;

@Log4j2
public abstract class BaseLogPollingBotHandler  extends TelegramLongPollingBot {

	@Autowired
	protected AuthService authService;
	
	@Autowired
	protected KeyboardService keyboardService;
	
	public void sendEchoMessage(Long chatId, String text){
		sendHtmlMessage(chatId, String.format(BotMessageConstants.DONT_UNDERSTAND_MSG, text, Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE));
	}
	
	public void sendTextMessage(Long chatId, String text) {
		SendMessage msg = createMessage(chatId, text);
		sendMessage(msg);
	}
	
	public void sendHtmlMessage(Long chatId, String text) {
		SendMessage msg = createMessage(chatId, text);
		
		msg.enableHtml(true);
		sendMessage(msg);
	}
	
	public void sendTextMessageToAllUsers(String text){
        authService.getAllowedUserIds().stream().forEach(
                chatId -> sendTextMessage(chatId.longValue(), text));
    }
	
	public void sendHtmlMessageToAllUsers(String text){
        authService.getAllowedUserIds().stream().forEach(
                chatId -> sendHtmlMessage(chatId.longValue(), text));
    }
	
	protected SendMessage createMessage(Long chatId, String text){
		SendMessage sendMessageRequest = new SendMessage();
		sendMessageRequest.setChatId(chatId.toString());
		sendMessageRequest.setText(text);
		
		return sendMessageRequest;
	}
	
	protected SendMessage createKeyBoardMessage(ReplyKeyboardMarkup markup, Integer messageId, Long chatId, String text){
		SendMessage sendMessage = new SendMessage();
        sendMessage.enableMarkdown(true);

        sendMessage.setReplyMarkup(markup);
        if(messageId!=null){
        	sendMessage.setReplyToMessageId(messageId);
        }
        sendMessage.setChatId(chatId);
        sendMessage.setText(text);
        sendMessage.enableHtml(true);
        
        return sendMessage;
	}
	
	protected SendMessage createKeyBoardInlineMessage(InlineKeyboardMarkup markup, Integer messageId, Long chatId, String text){
		SendMessage sendMessage = new SendMessage();
        sendMessage.enableMarkdown(true);

        sendMessage.setReplyMarkup(markup);
        if(messageId!=null){
            sendMessage.setReplyToMessageId(messageId);
        }
        sendMessage.setChatId(chatId);
        sendMessage.setText(text);
        sendMessage.enableHtml(true);
        
        return sendMessage;
	}
	
	protected SendMessage createDeviceRegisteredMessage(Long chatId, String serverName){
	    String text = String.format(BotMessageConstants.DEVICE_REGISTERED_MSG, Emoji.HAPPY_PERSON_RAISING_ONE_HAND,
	            serverName, new Date());
	    
	    SendMessage msg = createMessage(chatId, text);
        msg.enableHtml(true);
        return msg;
	}
	
	protected SendMessage createUnauthorizedMessage(Long chatId){
		String text = String.format(BotMessageConstants.UNAUTHORIZED_MSG, Emoji.WAVING_HAND_SIGN);
		
		SendMessage msg = createMessage(chatId, text);
		
		msg.enableHtml(true);
        
        return msg;
	}
	
	protected void sendHideKeyboard(Long chatId, Integer messageId) throws TelegramApiException {
        SendMessage sendMessage = new SendMessage();
        sendMessage.setChatId(chatId.toString());
        sendMessage.enableMarkdown(true);
        sendMessage.setReplyToMessageId(messageId);
        sendMessage.setText(Emoji.WAVING_HAND_SIGN.toString());

        ReplyKeyboardRemove replyKeyboardRemove = new ReplyKeyboardRemove();
        replyKeyboardRemove.setSelective(true);
        sendMessage.setReplyMarkup(replyKeyboardRemove);

        execute(sendMessage);
    }
	
	protected void sendMessage(SendMessage msg){
		try {
			execute(msg);
		} catch (TelegramApiException e) {
			log.error(e);
		}
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
