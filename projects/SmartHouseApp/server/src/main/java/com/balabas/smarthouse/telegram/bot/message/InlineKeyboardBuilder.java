package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.InlineKeyboardButton;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Device.DeviceState;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;

@Component
public class InlineKeyboardBuilder {

	@Autowired
	@Getter
	private ButtonBuilder buttons;
	
	public static InlineKeyboardButton createInlineKeyboardButton(String text,String callbackData){
		return new InlineKeyboardButton().setText(text).setCallbackData(callbackData);
	}
	
	public void addRefreshButtonToKeyboard(List<List<InlineKeyboardButton>> rowsInline){
        List<InlineKeyboardButton> row = new ArrayList<>();
        row.add(createInlineKeyboardButton(
        					buttons.getRefreshDeviceListButton(),
        					Action.callback(ACTION_TYPE_VIEW_DEVICE_LIST))
        				);
        rowsInline.add(row);
	}
	
	public InlineKeyboardMarkup getRefreshDevicesListInlineKeyboard( ) {
        InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
        List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
        
        List<InlineKeyboardButton> row = new ArrayList<>();
        row.add(createInlineKeyboardButton(
				buttons.getRefreshDeviceListButton(),
				Action.callback(ACTION_TYPE_VIEW_DEVICE_LIST))
			);
        rowsInline.add(row);
        
        markup.setKeyboard(rowsInline);
        return markup;
    }
	
	public InlineKeyboardMarkup getDevicesOfServerInlineKeyboard(List<Device> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (Device device : devices) {
    			
	    	List<InlineKeyboardButton> row = new ArrayList<>();
	    	
	    	Emoji emoji = Emoji.PAGER;
	    	String description = device.getDescription();
	    	String action = ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;
	    	
	    	if (!device.isInitialDataReceived()) {
	    		emoji = Emoji.HOURGLASS;
	    		description += " (Не инициализировано)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	} else if(DeviceState.TIMED_OUT.equals(device.getState())){
	    		emoji = Emoji.GHOST;
	    		description += " (Не подключено)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	} else if(DeviceState.FAILED.equals(device.getState())){
	    		emoji = Emoji.RED_CIRCLE;
	    		description += " (Ошибка данных)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	} else if(DeviceState.DISCONNECTED.equals(device.getState())){
	    		emoji = Emoji.SKULL;
	    		description += " (Ошибка подключения)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	}
	    	
	    	String text = buttons.getDeviceButton(emoji, description);
	    	String callback = Action.callback(action,"", device.getDeviceId(), GROUP_SENSORS);
	    	
			row.add(createInlineKeyboardButton(text, callback));
			
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
							Action.callback(ACTION_TYPE_VIEW_ENTITIES_OF_GROUP,"", device.getDeviceId(), group.getName()))
    					);
    			
    			rowsInline.add(row);
		    }
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getEntitiesOfGroupInlineKeyboard(Device device, Group group) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		String callback = Action.callback(ACTION_TYPE_VIEW_GROUPS_OF_DEVICE,"", device.getDeviceId(), group.getName()); 
		
		List<InlineKeyboardButton> row = new ArrayList<>();
		row.add(createInlineKeyboardButton(
					buttons.getDeviceButton(device.getDeviceDescr()),
					callback
					)
		        );
		
		rowsInline.add(row);
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getToggleOnOffInlineKeyboard(String text, String onText, String offText, String actionOn, String actionOff) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		List<InlineKeyboardButton> row = new ArrayList<>();
		
		row.add(createInlineKeyboardButton(
				buttons.getButton(null, onText),
				actionOn)
		    );
		
		row.add(createInlineKeyboardButton(
				buttons.getButton(null, offText),
				actionOff)
	        );	
		
		rowsInline.add(row);

		markup.setKeyboard(rowsInline);

		return markup;
	}
	
}
