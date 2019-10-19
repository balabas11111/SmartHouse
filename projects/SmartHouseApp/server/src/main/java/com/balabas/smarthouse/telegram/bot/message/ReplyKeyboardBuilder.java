package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.KeyboardRow;

import com.balabas.smarthouse.server.view.Action;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_UNKNOWN;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;

import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_HOME;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_ROOT;

@Component
public class ReplyKeyboardBuilder {
	
	@Autowired
	private ItemTextHelper button;
	
	public Action getActionByReplyButton(String data){
		if(data!=null && !data.isEmpty()) {
			if(data.startsWith(button.getRefreshDeviceListButton())
					|| COMMAND_HOME.equals(data)
					|| COMMAND_ROOT.equals(data)){
				return Action.fromColumnList(ACTION_TYPE_VIEW_DEVICE_LIST); 
			}
		}
		return Action.fromColumnList(ACTION_TYPE_UNKNOWN);
	}

	protected ReplyKeyboardMarkup createReplyKeyboardMarkup() {
		ReplyKeyboardMarkup replyKeyboardMarkup = new ReplyKeyboardMarkup();
        replyKeyboardMarkup.setSelective(true);
        replyKeyboardMarkup.setResizeKeyboard(true);
        replyKeyboardMarkup.setOneTimeKeyboard(false);
        
        return replyKeyboardMarkup;
	}
	
	public ReplyKeyboardMarkup getRefreshDevicesListReplyKeyboard() {
		ReplyKeyboardMarkup replyKeyboardMarkup = createReplyKeyboardMarkup();
		
		List<KeyboardRow> keyboard = new ArrayList<>();
        KeyboardRow firstRow = new KeyboardRow();
        firstRow.add(button.getRefreshDeviceListButton());
        
        keyboard.add(firstRow);
        replyKeyboardMarkup.setKeyboard(keyboard);

        return replyKeyboardMarkup;
	}
}
