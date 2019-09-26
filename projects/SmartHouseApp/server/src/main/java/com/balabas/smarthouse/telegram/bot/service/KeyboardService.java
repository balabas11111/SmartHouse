package com.balabas.smarthouse.telegram.bot.service;

import java.util.ArrayList;
import java.util.List;

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
	public static final String DEVICE_PREFFIX = "device";
	public static final String DEVICE_GROUP_PREFFIX = "deviceGroup";

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
		
		for (Device device : devices) {
			List<InlineKeyboardButton> rowInline = new ArrayList<>();
			rowInline.add(new InlineKeyboardButton().setText(getDeviceButton(device)).setCallbackData(DEVICE_PREFFIX+PREFFIX_SPLITTER + device.getDeviceId()));
			
			rowsInline.add(rowInline);
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getGroupsInlineKeyboard( Device device) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (Group group : device.getGroups()) {
			List<InlineKeyboardButton> rowInline = new ArrayList<>();
			rowInline.add(new InlineKeyboardButton().setText(getGroupButton(group))
							.setCallbackData(DEVICE_GROUP_PREFFIX+PREFFIX_SPLITTER + device.getDeviceId()
									+PREFFIX_SPLITTER + group.getName())
						);
			
			rowsInline.add(rowInline);
		}

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
		return getButton(Emoji.REFRESH, "refresh");
	}
	
	public boolean isDeviceKey(String key){
		return key.startsWith(Emoji.OUTBOX_TRAY.toString());
	}
	
	private String getDeviceButton(Device device) {
		return String.format(BotMessageConstants.DEVICE_BUTTON,Emoji.OUTBOX_TRAY, device.getDeviceDescr());
	}
	
	private String getGroupButton(Group group) {
		return String.format(BotMessageConstants.GROUP_BUTTON,Emoji.OUTBOX_TRAY, group.getName());
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
	
	
	
}
