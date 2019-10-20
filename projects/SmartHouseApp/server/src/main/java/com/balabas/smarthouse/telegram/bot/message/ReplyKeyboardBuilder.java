package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.KeyboardRow;

@Component
public class ReplyKeyboardBuilder {
	
	protected ReplyKeyboardMarkup createReplyKeyboardMarkup() {
		ReplyKeyboardMarkup replyKeyboardMarkup = new ReplyKeyboardMarkup();
        replyKeyboardMarkup.setSelective(true);
        replyKeyboardMarkup.setResizeKeyboard(true);
        replyKeyboardMarkup.setOneTimeKeyboard(false);
        
        return replyKeyboardMarkup;
	}
	
	public ReplyKeyboardMarkup getMainMenuReplyKeyboard() {
		ReplyKeyboardMarkup replyKeyboardMarkup = createReplyKeyboardMarkup();
		
		List<KeyboardRow> keyboard = new ArrayList<>();
        
		keyboard.add(getRefreshDevicesListViewKeyboard());
		keyboard.add(getRefreshDevicesListEditReplyKeyboard());
        
        replyKeyboardMarkup.setKeyboard(keyboard);

        return replyKeyboardMarkup;
	}
	
	public KeyboardRow getRefreshDevicesListViewKeyboard() {
		return getKeyboardRow(ItemTextHelper.getRefreshDeviceListButton());
	}
	
	public KeyboardRow getRefreshDevicesListEditReplyKeyboard() {
		return getKeyboardRow(ItemTextHelper.getEditDeviceListButton());
	}
	
	private KeyboardRow getKeyboardRow(String text) {
		KeyboardRow row = new KeyboardRow();
        row.add(text);
        
        return row; 
	}
	
}
