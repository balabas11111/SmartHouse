package com.balabas.smarthouse.telegram.bot.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Service;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.InlineKeyboardButton;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.KeyboardRow;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;

@Service
public class KeyboardService {
	
	public static final String PREFFIX_SPLITTER = ":";
	public static final String REFRESH_PREFFIX = "refresh";
	public static final String DEVICE_PREFFIX = "device";
	public static final String DEVICE_GROUP_PREFFIX = "deviceGroup";
	
	public static final String GROUP_DEFAULT = "default";
	public static final String GROUP_DEVICE = "device";
	public static final String GROUP_SETTINGS = "settings";
	public static final String GROUP_SENSORS = "sensors";
	public static final String GROUP_SERVICES = "services";
	
	public static final String GROUP_SETTINGS_NAME = "Настройки";
    public static final String GROUP_SENSORS_NAME = "Датчики";
    public static final String GROUP_SERVICES_NAME = "Сервисы";
    
    Map<String,String> groupNameMap = new HashMap<>();
    Map<String,Emoji> groupImageMap = new HashMap<>();
    
    Emoji emojiDefault = Emoji.STAR;
    
    public KeyboardService(){
        groupNameMap.put(GROUP_SENSORS, GROUP_SENSORS_NAME);
        groupNameMap.put(GROUP_SERVICES, GROUP_SERVICES_NAME);
        groupNameMap.put(GROUP_SETTINGS, GROUP_SETTINGS_NAME);
        
        groupImageMap.put(GROUP_SENSORS, Emoji.RIGHT_HAND);
        groupImageMap.put(GROUP_SERVICES, Emoji.BRIEFCASE);
        groupImageMap.put(GROUP_SETTINGS, Emoji.SETTINGS);
    }

	public ReplyKeyboardMarkup getRefreshKeyboard(){
		ReplyKeyboardMarkup markup = createKeyboard();
		List<KeyboardRow> keyboard = new ArrayList<>();
		
		KeyboardRow row = new KeyboardRow();
		row.add(getHomeKey());
		keyboard.add(row);
		
		markup.setKeyboard(keyboard);
		
		return markup;
	}
	
	public InlineKeyboardMarkup getDevicesInlineKeyboard( List<Device> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		addRefreshButtonToKeyboard(rowsInline);
		
		for (Device device : devices) {
			List<InlineKeyboardButton> row = new ArrayList<>();
			row.add(new InlineKeyboardButton()
			        .setText(getDeviceButton(device))
			        .setCallbackData(DEVICE_PREFFIX+PREFFIX_SPLITTER + device.getDeviceId()));
			
			rowsInline.add(row);
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getDeviceRefreshInlineKeyboard( ) {
        InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
        List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
        
        List<InlineKeyboardButton> rowEmpty = new ArrayList<>();
        rowsInline.add(rowEmpty);
        
        addRefreshButtonToKeyboard(rowsInline);
        
        markup.setKeyboard(rowsInline);
        return markup;
    }
	
	public InlineKeyboardMarkup getGroupsInlineKeyboard( Device device) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (Group group : device.getGroups()) {
		    if(!GROUP_DEVICE.equals(group.getName())){
    			List<InlineKeyboardButton> row = new ArrayList<>();
    			row.add(new InlineKeyboardButton()
    			                .setText(getGroupButton(group))
    							.setCallbackData(DEVICE_GROUP_PREFFIX+PREFFIX_SPLITTER + device.getDeviceId()
    									+PREFFIX_SPLITTER + group.getName())
    						);
    			
    			rowsInline.add(row);
		    }
		}
		
		addRefreshButtonToKeyboard(rowsInline);

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public boolean isHomeKey(String key){
		return key.startsWith(Emoji.HOUSE_BUILDING.toString());
	}

	public String getHomeKey() {
		return getButton(Emoji.HOUSE_BUILDING, "Home");
	}
	
	public boolean isRefreshKey(String key){
		return key.startsWith(Emoji.REFRESH.toString());
	}
	
	public String getRefreshKey() {
	    return String.format(BotMessageConstants.REFRESH_BUTTON,Emoji.REFRESH);
	}
	
	public boolean isDeviceKey(String key){
		return key.startsWith(Emoji.OUTBOX_TRAY.toString());
	}
	
	private String getDeviceButton(Device device) {
		return String.format(BotMessageConstants.DEVICE_BUTTON,Emoji.OUTBOX_TRAY, device.getDeviceDescr());
	}
	
	private String getGroupButton(Group group) {
	    Emoji emoji = groupImageMap.containsKey(group.getName())?
	                    groupImageMap.get(group.getName()):emojiDefault;
	                    
	    String name = groupNameMap.containsKey(group.getName())?
	                    groupNameMap.get(group.getName()):group.getName();
		return String.format(BotMessageConstants.GROUP_BUTTON,emoji, name);
	}
	
	private String getButton(Emoji emoji, String text){
		return String.format(BotMessageConstants.BUTTON, emoji, text);
	}
	
	private ReplyKeyboardMarkup createKeyboard(){
		ReplyKeyboardMarkup replyKeyboardMarkup = new ReplyKeyboardMarkup();
		replyKeyboardMarkup.setSelective(true);
		replyKeyboardMarkup.setResizeKeyboard(true);
		replyKeyboardMarkup.setOneTimeKeyboard(true);
		
		return replyKeyboardMarkup;
	}
	
	private void addRefreshButtonToKeyboard(List<List<InlineKeyboardButton>> rowsInline){
        List<InlineKeyboardButton> refreshRow = new ArrayList<>();
        refreshRow.add(new InlineKeyboardButton().setText(getRefreshKey())
                        .setCallbackData(REFRESH_PREFFIX + PREFFIX_SPLITTER + DEVICE_PREFFIX+PREFFIX_SPLITTER));
        rowsInline.add(refreshRow);
	}
	
}
