package com.balabas.smarthouse.telegram.bot.message;

import java.io.IOException;
import java.net.URL;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.logging.log4j.util.Strings;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardRemove;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.model.EntityClass;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.service.DeviceService;
import com.google.common.base.Charsets;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.io.Resources;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;

@Component
@Log4j2
public class SendMessageBuilder {

	@Autowired
	private DeviceService deviceService;
	
	@Autowired
	private InlineKeyboardBuilder inlineKeyboard;
	
	@Autowired
	@Getter
	private ReplyKeyboardBuilder replyKeyboard;
	
	@Autowired
	private ItemRendererBuilder itemRendererBuilder;
	
	@Autowired
	private ButtonBuilder buttons;
	
	public List<SendMessage> createServerStartedMessages(Long chatId, String serverName){
		List<SendMessage> msgs = Lists.newArrayList();
	    String text = String.format(BotMessageConstants.BOT_REGISTERED_MSG, Emoji.HAPPY_PERSON_RAISING_ONE_HAND,
	            serverName, new Date());
        msgs.add(createHideReplyKeyboardMessage(chatId, null, text));
        
        return msgs;
	}
	
	public List<SendMessage> createDeviceRegisteredMessages(String deviceName, Long chatId){
		List<SendMessage> msgs = Lists.newArrayList();
		
	    String text = String.format(BotMessageConstants.DEVICE_REGISTERED_MSG, Emoji.CHECK_MARK, deviceName, new Date());
	    
	    msgs.add(createRefreshDevicesListReplyKeyboard(chatId, text));
        return msgs;
	}
	
	public List<SendMessage> createRefreshDevicesListReplyKeyboard(ReplyContext context){
		List<SendMessage> msgs = Lists.newArrayList();
		msgs.add(context.createMsg(replyKeyboard.getRefreshDevicesListReplyKeyboard()));
        
        return msgs;
    }
	
	public SendMessage createRefreshDevicesListReplyKeyboard(Long chatId, String text){
		return ReplyContext.createMsg(replyKeyboard.getRefreshDevicesListReplyKeyboard(), null, chatId, text, true);
    }
	
	public SendMessage createRefreshDevicesListInlineKeyboard(ReplyContext cont){
        List<Device> devices = deviceService.getDevicesInitialized();
        
        cont.setText(
    		(devices.isEmpty())?
                    String.format(BotMessageConstants.NO_DEVICE_MSG,Emoji.WARNING):
                    String.format(BotMessageConstants.SELECT_DEVICE_MSG,Emoji.OUTBOX_TRAY));    
        
        return cont.createMsg(inlineKeyboard.getRefreshDevicesListInlineKeyboard());
    }
	
	public SendMessage createDevicesListInlineKeyboard(String serverName, ReplyContext cont){
	    List<Device> devices = deviceService.getDevicesInitialized();
	    
	    cont.setText(
			(devices.isEmpty())?
					String.format(BotMessageConstants.NO_DEVICE_MSG,Emoji.WARNING):
					String.format(BotMessageConstants.SERVER_SELECT_DEVICE_MSG,
                            Emoji.OUTBOX_TRAY, serverName));    
	    
		return cont.createMsg(inlineKeyboard.getDevicesOfServerInlineKeyboard(devices));
	}
	
	public SendMessage createGroupsOfDeviceInlineKeyboard(String deviceId, ReplyContext cont) {
		Device device = deviceService.getDeviceByDeviceId(deviceId).orElse(null);
		
		cont.setText(
			String.format(BotMessageConstants.SELECT_GROUP_MSG,
							Emoji.OUTBOX_TRAY, device.getDeviceDescr(),
							device.getDeviceFirmware()));
		
		return cont.createMsg(inlineKeyboard.getGroupsOfDeviceInlineKeyboard(device));
	}
	
	public SendMessage getEntitiesOfGroupInlineKeyboard(String deviceId, String groupName, ReplyContext cont){
		Device device = deviceService.getDeviceByDeviceId(deviceId).orElse(null);
		Group group = device.getGroup(groupName);
		
		Emoji groupEmoji = buttons.getEmojiByGroupName(groupName);
		String trans = buttons.getGroupNameTranslation(groupName);
		
		String text = String.format(BotMessageConstants.BUTTON,
				groupEmoji.toString(), trans);
		
		cont.setText(text);
		
		return cont.createMsg(inlineKeyboard.getEntitiesOfGroupInlineKeyboard(device, group));
	}
	
	public List<SendMessage> createGroupView(String deviceId, String groupId, ReplyContext context){
		List<SendMessage> result = Lists.newArrayList();
		
		Device device = deviceService.getDeviceByDeviceId(deviceId).orElse(null);
		if(device!=null){
			Group group = device.getGroup(groupId);
			
			StringBuilder builder = new StringBuilder();
			builder.append(buttons.getGroupHeader(device.getDescription(), group.getName()));
			
			
			group.getEntities().stream()
				.filter(e->EntityClass.DEFAULT.equals(e.getEntityRenderer()))
				.sorted((e1,e2)->e1.getDescription().compareToIgnoreCase(e2.getDescription()))
				.forEach(ent-> entityToTemplate(ent, builder));
			
			result.add(createHtmlMessage(context.getChatId(), builder.toString()));
			
			itemRendererBuilder.build(group.getEntities(), context.getChatId()).forEach(result::add);
		}
		return result;
	}
	
	private void entityToTemplate(Entity entity, StringBuilder builder){
		entityValueMapToTemplate(entity.getName(), entity.getValues(), builder);
		
		if(entity.hasSensorItems()){
			builder.append("\n Значения \n\n");
			entity.getSensorItems().stream().forEach(si-> entityValueMapToTemplate(entity.getName() + ENTITY_FIELD_SENSOR_ITEMS, si.getValues(), builder));
		}
		
		builder.append("\n");
	}
	
	private void entityValueMapToTemplate(String entName, Map<String,String> values, StringBuilder builder){
		if(entName==null || values == null){
			return;
		}
		String result = "";
		String baseTmplPath = "templates/bot/text/";
		
		try{
			String tmplPath = baseTmplPath + entName + ".txt";
			
			URL u = this.getClass().getClassLoader().getResource(tmplPath);
			result = Resources.toString(u, Charsets.UTF_8);
			
			for(Entry<String,String> entry : values.entrySet()){
				String tmplKey = "_"+entry.getKey()+"_";
				
				result = result.replaceAll(tmplKey, entry.getValue());
			}
			
			result+="\n";
		}catch (IOException e) {
			log.error(e);
			result = entName+" "+Joiner.on(",").withKeyValueSeparator("=").join(values);
		}
		
		builder.append(result);
	}
	
	 
	
	public SendMessage createUnknown(ReplyContext context){
		String text = String.format(BotMessageConstants.DONT_UNDERSTAND_MSG, Emoji.FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE, (context.getText() == null || context.getText().isEmpty())?"":context.getText());
		
		return createHtmlMessage(context.getChatId(), text, context.getReplyToMessageIdIfUnknown());
		
	}
	
	public SendMessage createUnauthorized(Long chatId){
		String text = String.format(BotMessageConstants.UNAUTHORIZED_MSG, Emoji.ERROR);
		
		return createHtmlMessage(chatId, text);
		
	}
	
	public SendMessage createServerError(String msg, Long chatId){
		return createMessage(BotMessageConstants.MESSAGE_SERVER_ERROR, Emoji.STOP.toString(), chatId, msg);
	}
	
	public SendMessage createDeviceError(String msg, Long chatId){
		return createMessage(BotMessageConstants.MESSAGE_DEVICE_ERROR, Emoji.STOP.toString(), chatId, msg);
	}
	
	public SendMessage createDeviceRefreshed(String msg, Long chatId){
		return createMessage(BotMessageConstants.MESSAGE_DEVICE_REFRESHED, Emoji.CHECK_MARK.toString(), chatId, msg);
	}
	
	public SendMessage createMessage(String format, String emoji, Long chatId, String msg){
		String text = String.format(format, emoji, (msg == null || msg.isEmpty())?"":msg);
		return createHtmlMessage(chatId, text);
	}
	
	public SendMessage createHideReplyKeyboardMessage(Long chatId, Integer messageId, String msgText) {
        SendMessage sendMessage = new SendMessage();
        sendMessage.setChatId(chatId.toString());
        sendMessage.enableMarkdown(true);
        sendMessage.setReplyToMessageId(messageId);
        
        String text = Strings.isNotEmpty(msgText)?msgText:Emoji.WAVING_HAND_SIGN.toString();
        
        sendMessage.setText(text);

        ReplyKeyboardRemove replyKeyboardRemove = new ReplyKeyboardRemove();
        replyKeyboardRemove.setSelective(true);
        sendMessage.setReplyMarkup(replyKeyboardRemove);

        return sendMessage;
    }
	
	public SendMessage createHtmlMessage(Long chatId, String text) {
		SendMessage msg = createTextMessage(chatId, text);
		
		msg.enableHtml(true);
		
		return msg;
	}
	
	public SendMessage createHtmlMessage(Long chatId, String text, Integer replyToMessageId) {
		SendMessage msg = createHtmlMessage(chatId, text);
		msg.setReplyToMessageId(replyToMessageId);
		
		return msg;
	}
	
	public SendMessage createTextMessage(Long chatId,String text){
		SendMessage msg = new SendMessage();
		msg.setChatId(chatId.toString());
		msg.setText(text);
		
		return msg;
	}
	
}
