package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.InlineKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.InlineKeyboardButton;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS_OF_ENTITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_SELECT_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_DESCRIPTION;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_DEACTIVATE_ALARM_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARM_INTERVAL_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY;

import static com.balabas.smarthouse.server.view.Action.ACTION_DELETE_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_EDIT_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_ADD_ENTITY_FIELD_ALARM;

import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_ALARM;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_ALARM_FIELD;

import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_NAME;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;

import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_DEACTIVATE_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_EDIT_INTERVAL_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_REMOVE_INTERVAL_MESSAGE;

@Component
@SuppressWarnings("rawtypes")
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
	
	public InlineKeyboardMarkup getSetupMenuKeyboard() {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
    			
    	List<InlineKeyboardButton> row = new ArrayList<>();
    	
		row.add(createInlineKeyboardButton(
				ItemTextHelper.getEditAlarmsButton(), 
					Action.callback(ACTION_TYPE_EDIT_ALARMS, "", "")));
		row.add(createInlineKeyboardButton(
				ItemTextHelper.getEditPropertiesButton(), 
					Action.callback(ACTION_TYPE_EDIT_DEVICE_SELECT_LIST, "", "")));
		
		rowsInline.add(row);
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
		
	public InlineKeyboardMarkup getAlarmsMenuKeyboard(List<Device> devices) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (IDevice device : devices) {
    			
	    	List<InlineKeyboardButton> row = new ArrayList<>();
	    	
	    	String text = buttons.getDeviceButton(device);
	    	String callback = Action.callback(ACTION_TYPE_EDIT_ALARMS_OF_DEVICE, "", device);
	    	
			row.add(createInlineKeyboardButton(text, callback));
			
			rowsInline.add(row);
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getEntitiesAlarmsOfDeviceMenuKeyboard(List<IEntity> entities) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for (IEntity entity : entities) {
    			
	    	List<InlineKeyboardButton> row = new ArrayList<>();
	    	
	    	String text = buttons.getEntityButton(entity);
	    	String callback = Action.callback(ACTION_TYPE_EDIT_ALARMS_OF_ENTITY, "", ID_TYPE_ENTITY, entity.getId());
	    	
			row.add(createInlineKeyboardButton(text, callback));
			
			rowsInline.add(row);
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getEntityFieldsAlarmsOfEntityMenuKeyboard(IEntityAlarm entityAlarm, List<IEntityField> entityFields) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		List<InlineKeyboardButton> row = new ArrayList<>();
		
		row.add(createInlineKeyboardButton(ENTITY_ALARM_DEACTIVATE_MESSAGE, 
				Action.callback(ACTION_TYPE_DEACTIVATE_ALARM_OF_ENTITY, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
		row.add(createInlineKeyboardButton(ENTITY_ALARM_EDIT_INTERVAL_MESSAGE, 
				Action.callback(ACTION_TYPE_EDIT_ALARM_INTERVAL_OF_ENTITY, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
		row.add(createInlineKeyboardButton(ENTITY_ALARM_REMOVE_INTERVAL_MESSAGE, 
				Action.callback(ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
		
		rowsInline.add(row);
		
		for (IEntityField entityField : entityFields) {
    			
			row = new ArrayList<>();
	    	
			row.add(createInlineKeyboardButton(buttons.getEntityFieldButton(entityField), 
					Action.callback(ACTION_TYPE_EDIT_ALARMS_OF_ENTITY_FIELD, "", ID_TYPE_ENTITY_FIELD, entityField.getId())));
			
			rowsInline.add(row);
		}
		
		markup.setKeyboard(rowsInline);

		return markup;
	}
	
	public InlineKeyboardMarkup getEntityFieldsAlarmEditMenuKeyboard(IEntityAlarm entityAlarm, IEntityField entityField,
			List<Class> enabledAlarmClasses) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		List<InlineKeyboardButton> row = new ArrayList<>();
		
		Map<String, Class> possibleAlarms = new HashMap<>();
		Map<String, IEntityFieldAlarm> existingAlarms = new HashMap<>();
		
		enabledAlarmClasses.stream().forEach(c -> possibleAlarms.put(c.getName(), c));
		
		String data = new JSONObject()
				.put(ACTION_DATA_FIELD_NAME, "value")
				.toString();
		
		//remove/edit existing alarms
		for (IEntityFieldAlarm entityFieldAlarm : entityAlarm.getAlarms()) {
    		
			existingAlarms.put(entityFieldAlarm.getClass().getName(), entityFieldAlarm);
			
			row = new ArrayList<>();
			row.add(createInlineKeyboardButton(buttons.getEditEntityFieldAlarmButton(entityFieldAlarm), 
					Action.callback(ACTION_EDIT_ENTITY_FIELD_ALARM, data, ID_TYPE_ENTITY_ALARM_FIELD, entityFieldAlarm.getId())));
			row.add(createInlineKeyboardButton(buttons.getDeleteEntityFieldAlarmButton(entityFieldAlarm), 
					Action.callback(ACTION_DELETE_ENTITY_FIELD_ALARM, "", ID_TYPE_ENTITY_ALARM_FIELD, entityFieldAlarm.getId())));
			
			rowsInline.add(row);
		}
		//add alarms
		for (Class clazz : enabledAlarmClasses) {
    		
			if (!existingAlarms.containsKey(clazz.getName())) {
				
				data = new JSONObject()
								.put(ACTION_DATA_FIELD_NAME, "value")
								.put("class", clazz.getName())
								.toString();
				
				row = new ArrayList<>();
				row.add(createInlineKeyboardButton(buttons.getAddEntityFieldAlarmButton(clazz), 
						Action.callback(ACTION_ADD_ENTITY_FIELD_ALARM, data, ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
				
				rowsInline.add(row);
			}
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
		
		List<InlineKeyboardButton> rowD = new ArrayList<>();
    	
    	String textD = buttons.getButton(device.getEmoji(), device.getDescription() + " : " + "редактировать описание");
    	
    	String data = new JSONObject()
				.put(ACTION_DATA_FIELD_NAME, DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION).toString();

    	
    	String callbackD = Action.callback(ACTION_TYPE_EDIT_DEVICE_DESCRIPTION, data, device.getName());
    	
		rowD.add(createInlineKeyboardButton(textD, callbackD));
		
		rowsInline.add(rowD);
		
		device.getEntities().stream()
			.sorted((e1, e2) -> e1.getName().compareToIgnoreCase(e2.getName()))
			.forEach( entity ->{
    			
		    	List<InlineKeyboardButton> row = new ArrayList<>();
		    	
		    	String text = buttons.getButton(entity.getEmoji(), entity.getDescription());
		    	String callback = Action.callback(ACTION_TYPE_EDIT_ENTITITY, "", ID_TYPE_ENTITY, entity.getId());
		    	
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
    		
			List<Action> actions = EntityViewBuilder.getCommandButtonsForEntity(
					ACTION_TYPE_SEND_DATA_TO_DEVICE, entity, entityField);
			
			if(actions!=null && !actions.isEmpty()) {
				
				for (Action action : actions) {
					String text = action.getDescription();
					String callback = action.getCallbackData();
					
					List<InlineKeyboardButton> row = new ArrayList<>();
					row.add(createInlineKeyboardButton(text, callback));
					rowsInline.add(row);
				}
				 
			} else			
			if(!entityField.isReadOnly() && !DeviceConstants.ENTITY_FIELD_ID.equals(entityField.getName())) {
		    	List<InlineKeyboardButton> row = new ArrayList<>();
		    	
		    	String description = StringUtils.isEmpty(entityField.getDescription())?entityField.getValueStr():entityField.getDescription();
		    	
		    	String text = buttons.getButton(BotMessageConstants.EDIT_DEVICE_SELECT_ENTITY_FIELD_BUTTON, 
		    			entityField.getName(), description);
		    	
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
	
	public InlineKeyboardMarkup getCommandButtonsByEnabledFieldCommandButtonList(List<Action> actions) {
		InlineKeyboardMarkup markup =new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		
		for(Action action : actions) {
			rowsInline.add(
				Collections.singletonList(
					createInlineKeyboardButton(
						action.getDescription(),
						action.getCallbackData())
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
