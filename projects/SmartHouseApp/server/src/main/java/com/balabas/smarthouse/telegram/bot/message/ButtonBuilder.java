package com.balabas.smarthouse.telegram.bot.message;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.telegram.bot.BotConstants;

@Component
public class ButtonBuilder {

	Map<String,String> groupNameMap = new HashMap<>();
    Map<String,Emoji> groupImageMap = new HashMap<>();
    
    Emoji emojiDefault = Emoji.STAR;
    
    public ButtonBuilder(){
        groupNameMap.put(BotConstants.GROUP_SENSORS, BotConstants.GROUP_SENSORS_NAME);
        groupNameMap.put(BotConstants.GROUP_SERVICES, BotConstants.GROUP_SERVICES_NAME);
        groupNameMap.put(BotConstants.GROUP_SETTINGS, BotConstants.GROUP_SETTINGS_NAME);
        
        groupImageMap.put(BotConstants.GROUP_SENSORS, Emoji.RIGHT_HAND);
        groupImageMap.put(BotConstants.GROUP_SERVICES, Emoji.BRIEFCASE);
        groupImageMap.put(BotConstants.GROUP_SETTINGS, Emoji.SETTINGS);
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
	
	public String getDeviceButton(String deviceDescr) {
		return String.format(BotMessageConstants.DEVICE_BUTTON,Emoji.OUTBOX_TRAY, deviceDescr);
	}
	
	public String getGroupButton(String groupName) {
	    Emoji emoji = getEmojiByGroupName(groupName);
	    String name = getGroupNameTranslation(groupName);
	    
		return String.format(BotMessageConstants.GROUP_BUTTON,emoji, name);
	}
}
