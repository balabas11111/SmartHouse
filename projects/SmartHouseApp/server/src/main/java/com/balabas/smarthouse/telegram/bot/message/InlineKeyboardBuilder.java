package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.InlineKeyboardButton;

import com.balabas.smarthouse.server.entity.model.Emoji;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.State;
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
	
	public InlineKeyboardMarkup getDevicesOfServerInlineKeyboard(List<IDevice> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (IDevice device : devices) {
    			
	    	List<InlineKeyboardButton> row = new ArrayList<>();
	    	
	    	Emoji emoji = Emoji.PAGER;
	    	String description = device.getDescription();
	    	String action = ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;
	    	
	    	if (!device.isInitialized()) {
	    		emoji = Emoji.HOURGLASS;
	    		description += " (Не инициализировано)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	} else if(State.TIMED_OUT.equals(device.getState())){
	    		emoji = Emoji.GHOST;
	    		description += " (Не подключено)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	} else if(State.FAILED.equals(device.getState())){
	    		emoji = Emoji.RED_CIRCLE;
	    		description += " (Ошибка данных)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	} else if(State.DISCONNECTED.equals(device.getState())){
	    		emoji = Emoji.SKULL;
	    		description += " (Ошибка подключения)";
	    		action = ACTION_TYPE_VIEW_DEVICE_LIST;
	    	}
	    	
	    	String text = buttons.getDeviceButton(emoji, description);
	    	String callback = Action.callback(action,"", device.getName(), GROUP_SENSORS);
	    	
			row.add(createInlineKeyboardButton(text, callback));
			
			rowsInline.add(row);
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getGroupsOfDeviceInlineKeyboard(IDevice device) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (IGroup group : device.getGroups()) {
		    if(!GROUP_DEVICE.equals(group.getName())){
    			
		    	List<InlineKeyboardButton> row = new ArrayList<>();
    			row.add(createInlineKeyboardButton(
							buttons.getGroupButton(group.getName()),
							Action.callback(ACTION_TYPE_VIEW_ENTITIES_OF_GROUP,"", device.getName(), group.getName()))
    					);
    			
    			rowsInline.add(row);
		    }
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getEntitiesOfGroupInlineKeyboard(IDevice device, IGroup group) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		String callback = Action.callback(ACTION_TYPE_VIEW_GROUPS_OF_DEVICE,"", device.getName(), group.getName()); 
		
		List<InlineKeyboardButton> row = new ArrayList<>();
		row.add(createInlineKeyboardButton(
					buttons.getDeviceButton(device.getDescription()),
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
