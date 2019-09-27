package com.balabas.smarthouse.telegram.bot.message;

import java.util.Date;
import java.util.List;

import javax.xml.transform.TransformerException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardRemove;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.service.DeviceService;

@Component
public class SendMessageBuilder {

	@Autowired
	private DeviceService deviceService;
	
	@Autowired
	private InlineKeyboardBuilder keyboard;
	
	@Autowired
	private GroupViewBuilder groupViewBuilder;
	
	public SendMessage createKeyBoardInlineMessage(InlineKeyboardMarkup markup, Integer messageId, Long chatId, String text){
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
	
	public String getReplyOriginator(String[] data){
		return keyboard.getReplyOriginator(data);
	}
	
	public SendMessage createServerStartedMessage(Long chatId, String serverName){
	    String text = String.format(BotMessageConstants.DEVICE_REGISTERED_MSG, Emoji.HAPPY_PERSON_RAISING_ONE_HAND,
	            serverName, new Date());
	    
	    SendMessage msg = createHtmlMessage(chatId, text);
        msg.enableHtml(true);
        return msg;
	}
	
	public SendMessage createDeviceRegisteredMessage(Long chatId, String serverName){
	    String text = String.format(BotMessageConstants.BOT_REGISTERED_MSG, Emoji.CHECK_MARK, serverName, new Date());
	    
	    SendMessage msg = createHtmlMessage(chatId, text);
        msg.enableHtml(true);
        return msg;
	}
	
	public SendMessage createRefreshDevicesList(Integer replyToMessageId, Long chatId){
        List<Device> devices = deviceService.getDevices();
        
        String text = (devices.isEmpty())?
                        String.format(BotMessageConstants.NO_DEVICE_MSG,Emoji.WARNING):
                        String.format(BotMessageConstants.SELECT_DEVICE_MSG,Emoji.OUTBOX_TRAY);    
        
        return createKeyBoardInlineMessage(
        		keyboard.getRefreshDevicesListInlineKeyboard(),
        		replyToMessageId, chatId, text);
    }
	
	public SendMessage createDevicesList(String serverName, Integer messageId, Long chatId){
	    List<Device> devices = deviceService.getDevices();
	    
	    String text = (devices.isEmpty())?
	                    String.format(BotMessageConstants.NO_DEVICE_MSG,Emoji.WARNING):
                        String.format(BotMessageConstants.SERVER_SELECT_DEVICE_MSG,
                                        Emoji.OUTBOX_TRAY, serverName);    
	    
		return createKeyBoardInlineMessage(
		        keyboard.getDevicesListInlineKeyboard(devices),
		        messageId, chatId, text);
	}
	
	public SendMessage createGroupsOfDevice(String deviceId, Integer messageId, Long chatId) {
		Device device = deviceService.getDeviceByDeviceId(deviceId).orElse(null);
		
		return createKeyBoardInlineMessage(
		        keyboard.getGroupsOfDeviceInlineKeyboard(device),
		        messageId, chatId,
				String.format(BotMessageConstants.SELECT_GROUP_MSG,Emoji.OUTBOX_TRAY, device.getDeviceDescr(),
				        device.getDeviceFirmware()));
	}
	
	public SendMessage createGroupView(String deviceId, String groupId, Integer messageId, Long chatId) throws TransformerException{
		Device device = deviceService.getDeviceByDeviceId(deviceId).orElse(null);
		Group group = device.getGroup(groupId);
		
		String text = //String.format(BotMessageConstants.GROUP_DISPLAY_HEADER_MSG, Emoji.OUTBOX_TRAY, device.getDeviceDescr(), group.getName())  
						groupViewBuilder.build(device, group);
		
		return createKeyBoardInlineMessage(
		        keyboard.getGroupViewInlineKeyboard(device, group),
		        messageId, chatId, text);
	}
	
	public SendMessage createUnknown(String msg, Long chatId, Integer replytoMessageId){
		String text = String.format(BotMessageConstants.DONT_UNDERSTAND_MSG, Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE, (msg == null || msg.isEmpty())?"":msg);
		
		return createHtmlMessage(chatId, text, replytoMessageId);
		
	}
	
	public SendMessage createUnauthorized(Long chatId){
		String text = String.format(BotMessageConstants.UNAUTHORIZED_MSG, Emoji.ERROR);
		
		return createHtmlMessage(chatId, text);
		
	}
	
	public SendMessage createError(String msg, Long chatId){
		String text = String.format(BotMessageConstants.ERROR_MSG, Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE, (msg == null || msg.isEmpty())?"":msg);
		
		return createHtmlMessage(chatId, text);
	}
	
	public SendMessage sendHideKeyboard(Long chatId, Integer messageId) {
        SendMessage sendMessage = new SendMessage();
        sendMessage.setChatId(chatId.toString());
        sendMessage.enableMarkdown(true);
        sendMessage.setReplyToMessageId(messageId);
        sendMessage.setText(Emoji.WAVING_HAND_SIGN.toString());

        ReplyKeyboardRemove replyKeyboardRemove = new ReplyKeyboardRemove();
        replyKeyboardRemove.setSelective(true);
        sendMessage.setReplyMarkup(replyKeyboardRemove);

        return sendMessage;
    }
	
	public SendMessage createHtmlMessage(Long chatId, String text, Integer replyToMessageId) {
		SendMessage msg = createHtmlMessage(chatId, text);
		if(replyToMessageId!=null){
			msg.setReplyToMessageId(replyToMessageId);
		}
		
		return msg;
	}
	
	public SendMessage createHtmlMessage(Long chatId, String text) {
		SendMessage msg = createTextMessage(chatId, text);
		
		msg.enableHtml(true);
		
		return msg;
	}
	
	public SendMessage createTextMessage(Long chatId,String text){
		SendMessage msg = new SendMessage();
		msg.setChatId(chatId.toString());
		msg.setText(text);
		
		return msg;
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
	
	
}
