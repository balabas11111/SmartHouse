package com.balabas.smarthouse.telegram.bot.handler;

import javax.xml.transform.TransformerException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.starter.AfterBotRegistration;

import com.balabas.smarthouse.server.service.DeviceService;
import com.balabas.smarthouse.telegram.bot.BotConstants;
import com.balabas.smarthouse.telegram.bot.message.BotMessageConstants;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Component
public class SmartHouseBotHandler extends BaseLogPollingBotHandler {

	@Autowired
	DeviceService deviceService;
	
	@Override
	public void onUpdateReceived(Update update) {
		if (update.hasMessage()) {
			Message message = update.getMessage();

			if(!checkAuthorization(message)){
				sendMessage(messageBuilder.createUnauthorized(message.getChatId()));
			}
			
			if(processCommand(message)){
				return;
			}
			if (message.hasText()) {
				sendMessage(messageBuilder.createUnknown(message.getText(), message.getChatId(), message.getMessageId()));
			}
		}else if(update.hasCallbackQuery()){
			processCallback(update.getCallbackQuery());
		}
	}
	
	private boolean processCallback(CallbackQuery callback){
		boolean result = false;
			String[] data = callback.getData().split(BotConstants.CALLBACK_SPLITTER);
			
			String originator = messageBuilder.getReplyOriginator(data);
			Integer messageId = null;
			Long chatId = callback.getMessage().getChatId();
			
			SendMessage msg = null;
			
			try{
				switch(originator){
					case BotConstants.REFRESH_PREFFIX:
						msg = messageBuilder.createDevicesList(authService.getServerName(), messageId, chatId);
						break;
					case BotConstants.DEVICE_PREFFIX:
						msg = messageBuilder.createGroupsOfDevice(data[1], messageId, chatId);
						break;
					case BotConstants.DEVICE_GROUP_PREFFIX:
						msg = messageBuilder.createGroupView(data[1], data[2], messageId, chatId);
						break;
					default:
						msg = messageBuilder.createUnknown(null, chatId, callback.getMessage().getMessageId());
				}
			}catch(NullPointerException | TransformerException ex){
				log.error(ex);
				msg = messageBuilder.createError(null, chatId);
			}
			
			
			sendMessage(msg);
		return result;
	}
	
	private boolean processCommand(Message message){
		String text = message.getText();
		boolean result = false; 
		
		if(BotMessageConstants.COMMAND_HOME.equals(text)){
			result = true;
			sendMessage(messageBuilder.createRefreshDevicesList(message.getMessageId(), message.getChatId()));
		}
		
		return result;
	}
	
	@AfterBotRegistration
	public void sendServerStartedToAllUsers(){
	    authService.getAllowedUserIds().stream().forEach(
	            chatId -> sendMessage(messageBuilder.createServerStartedMessage(chatId.longValue(), authService.getServerName())));
    }
	
	public void sendDeviceRegisteredToAllUsers(){
        authService.getAllowedUserIds().stream().forEach(
            chatId -> {
                sendMessage(messageBuilder.createDeviceRegisteredMessage(chatId.longValue(), authService.getServerName()));
                sendMessage(messageBuilder.createRefreshDevicesList(null, chatId.longValue()));
            });
    }
	
	
}
