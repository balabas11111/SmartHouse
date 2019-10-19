package com.balabas.smarthouse.telegram.bot.message;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.Emoji;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SETTINGS;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SERVICES;

import static com.balabas.smarthouse.telegram.bot.BotConstants.GROUP_SENSORS_NAME;
import static com.balabas.smarthouse.telegram.bot.BotConstants.GROUP_SERVICES_NAME;
import static com.balabas.smarthouse.telegram.bot.BotConstants.GROUP_SETTINGS_NAME;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.GROUP_VIEW_HEADER_MSG;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.GROUP_COMMAND_HEADER_MSG;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_DISPLAY_HEADER_MSG;

@Component
public class ItemTextHelper {

	Map<String,String> groupNameMap = new HashMap<>();
    Map<String,Emoji> groupImageMap = new HashMap<>();
    
    Emoji emojiDefault = Emoji.STAR;
    
    public ItemTextHelper(){
        groupNameMap.put(GROUP_SENSORS, GROUP_SENSORS_NAME);
        groupNameMap.put(GROUP_SERVICES, GROUP_SERVICES_NAME);
        groupNameMap.put(GROUP_SETTINGS, GROUP_SETTINGS_NAME);
        
        groupImageMap.put(GROUP_SENSORS, Emoji.HAND_RIGHT);
        groupImageMap.put(GROUP_SERVICES, Emoji.BRIEFCASE);
        groupImageMap.put(GROUP_SETTINGS, Emoji.HAMMER_AND_WRENCH);
    }
    
    public Emoji getEmojiByGroupName(String groupName){
    	return (groupName==null || !groupImageMap.containsKey(groupName))?emojiDefault:groupImageMap.get(groupName);
    }
    
    public String getGroupNameTranslation(String groupName){
    	return (groupName==null || !groupNameMap.containsKey(groupName))?groupName:groupNameMap.get(groupName);
    }
    
    public String getRefreshDeviceListButton(){
    	return String.format(BotMessageConstants.REFRESH_BUTTON,Emoji.REFRESH);
    }
	
    public String getDeviceButton(Emoji emoji, String deviceDescr) {
		return getButton(emoji, deviceDescr);
	}
    
	public String getDeviceButton(String deviceDescr) {
		return getButton(Emoji.OUTBOX_TRAY, deviceDescr);
	}
	
	public String getGroupButton(String groupName) {
	    Emoji emoji = getEmojiByGroupName(groupName);
	    String name = getGroupNameTranslation(groupName);
	    
		return getButton(emoji, name);
	}
	
	public String getGroupHeader(String deviceDescription, String groupName){
		return getGroupHeader(
				Emoji.OUTBOX_TRAY.toString(), deviceDescription,
				getEmojiByGroupName(groupName).toString(), getGroupNameTranslation(groupName));
	}
	
	public String getGroupHeader(String deviceEmoji, String deviceDescription, String groupEmoji, String groupDescription){
		return String.format(GROUP_VIEW_HEADER_MSG,
				deviceEmoji, deviceDescription,
				groupEmoji, groupDescription);
	}
	
	public String getGroupCommandHeader(String deviceEmoji, String deviceDescription, String groupEmoji, String groupDescription){
		return String.format(GROUP_COMMAND_HEADER_MSG,
				deviceEmoji, deviceDescription,
				groupEmoji, groupDescription);
	}
	
	public String getEntityHeader(String entityEmoji, String entityDescription){
		return String.format(ENTITY_DISPLAY_HEADER_MSG,
				entityEmoji, entityDescription);
	}
	
	public String getButton(Emoji emoji, String text) {
		return getButton(BotMessageConstants.BUTTON, emoji, text);
	}
	
	public String getButton(String format, Emoji emoji, String text) {
		return String.format(format,emoji==null?"":emoji.toString(), text);
	}
	
	public String getButton(String format, Emoji emoji) {
		return String.format(format,emoji==null?"":emoji.toString());
	}
	
}
