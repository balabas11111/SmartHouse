package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.InlineKeyboardButton;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IEntityFieldCommandButton;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;

import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_NAME;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;

@Component
public class InlineKeyboardBuilder {

	@Autowired
	@Getter
	private ItemTextHelper buttons;
	
	public static InlineKeyboardButton createInlineKeyboardButton(String text,String callbackData){
		return new InlineKeyboardButton().setText(text).setCallbackData(callbackData);
	}
	
	public void addRefreshButtonToKeyboard(List<List<InlineKeyboardButton>> rowsInline){
        List<InlineKeyboardButton> row = new ArrayList<>();
        row.add(createInlineKeyboardButton(
        		ItemTextHelper.getRefreshDeviceListButton(),
        					Action.callback(ACTION_TYPE_VIEW_DEVICE_LIST))
        				);
        rowsInline.add(row);
	}
	
	public InlineKeyboardMarkup getRefreshDevicesListInlineKeyboard( ) {
        InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
        List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
        
        List<InlineKeyboardButton> row = new ArrayList<>();
        row.add(createInlineKeyboardButton(
        		ItemTextHelper.getRefreshDeviceListButton(),
				Action.callback(ACTION_TYPE_VIEW_DEVICE_LIST))
			);
        rowsInline.add(row);
        
        markup.setKeyboard(rowsInline);
        return markup;
    }
	
	public InlineKeyboardMarkup getDevicesOfServerInlineKeyboardView(List<Device> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (IDevice device : devices) {
    			
	    	List<InlineKeyboardButton> row = new ArrayList<>();
	    	
	    	ActionContext ac = new ActionContext(device);
	    	
	    	String text = buttons.getDeviceButton(ac.getEmoji(), ac.getDescription());
	    	String callback = Action.callback(ACTION_TYPE_VIEW_ENTITIES_OF_GROUP, "", device.getName(), GROUP_SENSORS);
	    	
			row.add(createInlineKeyboardButton(text, callback));
			
			rowsInline.add(row);
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getDevicesOfServerInlineKeyboardEdit(List<Device> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (IDevice device : devices) {
    			
	    	List<InlineKeyboardButton> row = new ArrayList<>();
	    	
	    	ActionContext ac = new ActionContext(device);
	    	
	    	String text = buttons.getDeviceButton(ac.getEmoji(), ac.getDescription());
	    	String callback = Action.callback(ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE, "", device.getName());
	    	
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
	
	public InlineKeyboardMarkup getEntitiesOfDeviceInlineKeyboard(IDevice device) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		device.getEntities().stream()
			.sorted((e1, e2) -> e1.getName().compareToIgnoreCase(e2.getName()))
			.forEach( entity ->{
    			
		    	List<InlineKeyboardButton> row = new ArrayList<>();
		    	
		    	String text = buttons.getButton(entity.getEmoji(), entity.getDescription());
		    	String callback = Action.callback(ACTION_TYPE_EDIT_ENTITITY, "", entity);
		    	
				row.add(createInlineKeyboardButton(text, callback));
				
				rowsInline.add(row);
			});
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getFieldsOfEntityInlineKeyboard(IEntity entity) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		entity.getEntityFields().stream().sorted((ef1, ef2)->ef1.getName().compareToIgnoreCase(ef2.getName()))
		.forEach( entityField ->
		{
    		
			List<IEntityFieldCommandButton> commands = EntityViewBuilder.getCommandButtonsForEntity(
					ACTION_TYPE_SEND_DATA_TO_DEVICE, entity, entityField);
			
			if(commands!=null && !commands.isEmpty()) {
				
				for (IEntityFieldCommandButton comBtn : commands) {
					String text = comBtn.getButtonText();
					String callback = comBtn.getActionCallback();
					
					List<InlineKeyboardButton> row = new ArrayList<>();
					row.add(createInlineKeyboardButton(text, callback));
					rowsInline.add(row);
				}
				 
			} else			
			if(!entityField.isReadOnly() && !DeviceConstants.ENTITY_FIELD_ID.equals(entityField.getName())) {
		    	List<InlineKeyboardButton> row = new ArrayList<>();
		    	
		    	String text = buttons.getButton(BotMessageConstants.EDIT_DEVICE_SELECT_ENTITY_FIELD_BUTTON, 
		    			entityField.getName(), entityField.getDescription());
		    	
		    	String data = new JSONObject()
		    					.put(ACTION_DATA_FIELD_NAME, entityField.getName()).toString();
		    	
		    	String callback = Action.callback(ACTION_TYPE_EDIT_ENTITITY_FIELD, data, entity);
		    	
				row.add(createInlineKeyboardButton(text, callback));
				
				rowsInline.add(row);
			}
		});
		
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
	
	public InlineKeyboardMarkup getCommandButtonsByEnabledFieldCommandButtonList(List<IEntityFieldCommandButton> commands) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for(IEntityFieldCommandButton command : commands) {
			rowsInline.add(
				Collections.singletonList(
					createInlineKeyboardButton(
						command.getButtonText(),
						command.getActionCallback())
			    ));
		}
		
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
