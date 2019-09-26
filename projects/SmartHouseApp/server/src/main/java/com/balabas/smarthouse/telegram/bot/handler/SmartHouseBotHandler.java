package com.balabas.smarthouse.telegram.bot.handler;

import java.util.Date;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.starter.AfterBotRegistration;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.service.DeviceService;
import com.balabas.smarthouse.telegram.bot.service.BotMessageConstants;
import com.balabas.smarthouse.telegram.bot.service.Emoji;
import com.balabas.smarthouse.telegram.bot.service.KeyboardService;

@Component
public class SmartHouseBotHandler extends BaseLogPollingBotHandler {

	@Autowired
	DeviceService deviceService;
	
	@Override
	public void onUpdateReceived(Update update) {
		if (update.hasMessage()) {
			Message message = update.getMessage();

			if(!checkAuthorization(message)){
				sendMessage(createUnauthorizedMessage(message.getChatId()));
			}
			
			if(processCommand(message)){
				return;
			}
			if (message.hasText()) {
				sendEchoMessage(message.getChatId(), message.getText());
			}
		}else if(update.hasCallbackQuery()){
			processCallback(update.getCallbackQuery());
		}
	}
	
	private boolean processCallback(CallbackQuery callback){
		boolean result = false;
			String[] data = callback.getData().split(KeyboardService.PREFFIX_SPLITTER);
			
			if(data.length>1){
			    if(data[0].equals(KeyboardService.REFRESH_PREFFIX) 
                        && data[1].equals(KeyboardService.DEVICE_PREFFIX)){
                
			        result = true;
			        sendDevicesKeyboard(null, callback.getMessage().getChatId());
			        
			    }else
				if(data[0].equals(KeyboardService.DEVICE_PREFFIX)){
					String deviceId = data[1];
					Device device = deviceService.getDeviceByDeviceId(deviceId).orElseGet(null);
					
					result = true;
					
					if(device!=null){
						sendDevicesGroupKeyboard(device,callback.getMessage().getMessageId(),callback.getMessage().getChatId());
					}else{
						sendEchoMessage(callback.getMessage().getChatId(), BotMessageConstants.NOT_FOUND);
					}
				} else
	                if(data[0].equals(KeyboardService.DEVICE_GROUP_PREFFIX)){
	                    Long chatId = callback.getMessage().getChatId();
	                    
	                    String deviceId = data[1];
	                    String groupId = data[2];
	                    Device device = deviceService.getDeviceByDeviceId(deviceId).orElseGet(null);
	                    
	                    result = true;
	                    
	                    if(device==null){
	                        sendEchoMessage(chatId, BotMessageConstants.NOT_FOUND);
	                        return false;
	                    }
	                    
	                    Group group = device.getGroup(groupId);
	                    
	                    if(group == null){
	                        sendEchoMessage(chatId, BotMessageConstants.NOT_FOUND);
                            return false;
	                    }
	                    
	                    sendHtmlMessage(chatId, group.getData().toString());
	                } 
			
			}else{
				sendEchoMessage(callback.getMessage().getChatId(), callback.getMessage().getText());
			}
		
		return result;
	}
	
	private boolean processCommand(Message message){
		String text = message.getText();
		boolean result = false; 
		
		if(BotMessageConstants.COMMAND_HOME.equals(text)){
			result = true;
			sendDevicesKeyboard(message.getMessageId(), message.getChatId());
		}
		
		return result;
	}
	
	@AfterBotRegistration
	public void sendServerStartedToAllUsers(){
	    authService.getAllowedUserIds().stream().forEach(
	            chatId -> { 
	                        sendHtmlMessage( chatId.longValue(), 
	                                String.format(BotMessageConstants.BOT_REGISTERED_MSG, Emoji.CHECK_MARK, authService.getServerName(), new Date()));
	            });
    }
	
	public void sendDeviceRegisteredToAllUsers(){
        authService.getAllowedUserIds().stream().forEach(
            chatId -> {
                sendMessage(createDeviceRegisteredMessage(chatId.longValue(), authService.getServerName()));
                sendDevicesRefreshKeyboard(null, chatId.longValue());
            });
    }
	
	private void sendDevicesRefreshKeyboard(Integer messageId, Long chatId){
        List<Device> devices = deviceService.getDevices();
        
        String text = (devices.size()==0)?
                        String.format(BotMessageConstants.NO_DEVICE_MSG,Emoji.WARNING):
                        String.format(BotMessageConstants.SELECT_DEVICE_MSG,Emoji.OUTBOX_TRAY);    
        
        SendMessage msg = createKeyBoardInlineMessage(
                keyboardService.getDeviceRefreshInlineKeyboard(),
                messageId, chatId, text);
        sendMessage(msg);
    }
	
	private void sendDevicesKeyboard(Integer messageId, Long chatId){
	    List<Device> devices = deviceService.getDevices();
	    
	    String text = (devices.size()==0)?
	                    String.format(BotMessageConstants.NO_DEVICE_MSG,Emoji.WARNING):
                        String.format(BotMessageConstants.SERVER_SELECT_DEVICE_MSG,
                                        Emoji.OUTBOX_TRAY, authService.getServerName());    
	    
		SendMessage msg = createKeyBoardInlineMessage(
		        keyboardService.getDevicesInlineKeyboard(deviceService.getDevices()),
		        null, chatId, text);
		sendMessage(msg);
	}
	
	private void sendDevicesGroupKeyboard(Device device, Integer messageId, Long chatId){
		SendMessage msg = createKeyBoardInlineMessage(
		        keyboardService.getGroupsInlineKeyboard(device),
				null, chatId,
				String.format(BotMessageConstants.SELECT_GROUP_MSG,Emoji.OUTBOX_TRAY, device.getDeviceDescr(),
				        device.getDeviceFirmware()));
		sendMessage(msg);
	}
}
