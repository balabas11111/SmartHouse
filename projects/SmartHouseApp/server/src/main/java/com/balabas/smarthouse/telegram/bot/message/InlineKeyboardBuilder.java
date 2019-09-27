package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.InlineKeyboardButton;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;

import lombok.Getter;

import static com.balabas.smarthouse.telegram.bot.BotConstants.CALLBACK_SPLITTER;
import static com.balabas.smarthouse.telegram.bot.BotConstants.REFRESH_PREFFIX;
import static com.balabas.smarthouse.telegram.bot.BotConstants.DEVICE_PREFFIX;
import static com.balabas.smarthouse.telegram.bot.BotConstants.DEVICE_GROUP_PREFFIX;
import static com.balabas.smarthouse.telegram.bot.BotConstants.GROUP_PREFFIX;

import static com.balabas.smarthouse.telegram.bot.BotConstants.GROUP_DEVICE;

@Component
public class InlineKeyboardBuilder {

	@Autowired
	@Getter
	private ButtonBuilder buttons;
	
	public static InlineKeyboardButton createInlineKeyboardButton(String text,String callbackData){
		return new InlineKeyboardButton().setText(text).setCallbackData(callbackData);
	}
	
	public static String buildCallbackData(String... arg){
		StringBuilder result = new StringBuilder("");
		
		for(int i=0; i<arg.length; i++){
			result.append(arg[i]!=null?arg[i]:"");
			result.append(CALLBACK_SPLITTER);
		}
		
		return result.toString();
	}
	
	public String getReplyOriginator(String[] data){
		if(data.length>1){
			String key = data[0];
			if(REFRESH_PREFFIX.equals(key)
					|| DEVICE_PREFFIX.equals(key)
					|| DEVICE_GROUP_PREFFIX.equals(key)
					|| GROUP_PREFFIX.equals(key)){
				return key;
			}
		}
		return REFRESH_PREFFIX;
	}
	
	public void addRefreshButtonToKeyboard(List<List<InlineKeyboardButton>> rowsInline){
        List<InlineKeyboardButton> refreshRow = new ArrayList<>();
        refreshRow.add(createInlineKeyboardButton(
        					buttons.getRefreshDeviceListButton(),
        					buildCallbackData(REFRESH_PREFFIX, DEVICE_PREFFIX))
        				);
        rowsInline.add(refreshRow);
	}
	
	public InlineKeyboardMarkup getRefreshDevicesListInlineKeyboard( ) {
        InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
        List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
        
        List<InlineKeyboardButton> rowEmpty = new ArrayList<>();
        rowsInline.add(rowEmpty);
        
        addRefreshButtonToKeyboard(rowsInline);
        
        markup.setKeyboard(rowsInline);
        return markup;
    }

	public InlineKeyboardMarkup getDevicesListInlineKeyboard( List<Device> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		addRefreshButtonToKeyboard(rowsInline);
		
		for (Device device : devices) {
			List<InlineKeyboardButton> row = new ArrayList<>();
			row.add(createInlineKeyboardButton(
						buttons.getDeviceButton(device.getDeviceDescr()),
						buildCallbackData(DEVICE_PREFFIX, device.getDeviceId()))
			        );
			
			rowsInline.add(row);
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getGroupsOfDeviceInlineKeyboard(Device device) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (Group group : device.getGroups()) {
		    if(!GROUP_DEVICE.equals(group.getName())){
    			
		    	List<InlineKeyboardButton> row = new ArrayList<>();
    			row.add(createInlineKeyboardButton(
							buttons.getGroupButton(group.getName()),
							buildCallbackData(DEVICE_GROUP_PREFFIX, device.getDeviceId(), group.getName()))
    					);
    			
    			rowsInline.add(row);
		    }
		}
		
		addRefreshButtonToKeyboard(rowsInline);

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getGroupViewInlineKeyboard(Device device, Group group) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		List<InlineKeyboardButton> row = new ArrayList<>();
		row.add(createInlineKeyboardButton(
					buttons.getDeviceButton(device.getDeviceDescr()),
					buildCallbackData(GROUP_PREFFIX, device.getDeviceId(), group.getName()))
		        );
		
		rowsInline.add(row);
		
		addRefreshButtonToKeyboard(rowsInline);

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
}
