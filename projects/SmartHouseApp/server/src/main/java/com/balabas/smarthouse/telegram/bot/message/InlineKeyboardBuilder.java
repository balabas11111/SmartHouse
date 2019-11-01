package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

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
import com.google.common.collect.Lists;

import lombok.Getter;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_RESTART_APPLICATION;
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

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_CREATE_ALARM_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION;
import static com.balabas.smarthouse.server.view.Action.ACTION_ALARM_OF_ENTITY_CHANGE_SOUND;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARM_INTERVAL_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY;

import static com.balabas.smarthouse.server.view.Action.ACTION_DELETE_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_EDIT_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_ADD_ENTITY_FIELD_ALARM;

import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_ALARM;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_ALARM_FIELD;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;

import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_CREATE_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_DEACTIVATE_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_ACTIVATE_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_EDIT_INTERVAL_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_REMOVE_INTERVAL_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_ENABLE_SOUND_MESSAGE;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.ENTITY_ALARM_DISABLE_SOUND_MESSAGE;

@Component
@SuppressWarnings("rawtypes")
public class InlineKeyboardBuilder {

	@Autowired
	@Getter
	private ItemTextHelper buttons;

	public static List<InlineKeyboardButton> createInlineKeyboardButtonRow(String text, String callbackData) {
		List<InlineKeyboardButton> row = Lists.newArrayList();
		InlineKeyboardButton btn = new InlineKeyboardButton().setText(text).setCallbackData(callbackData);
		row.add(btn);
		return row;
	}
	
	public static InlineKeyboardButton createInlineKeyboardButton(String text, String callbackData) {
		return new InlineKeyboardButton().setText(text).setCallbackData(callbackData);
	}

	public void addRefreshButtonToKeyboard(List<List<InlineKeyboardButton>> rowsInline) {
		List<InlineKeyboardButton> row = new ArrayList<>();
		row.add(createInlineKeyboardButton(ItemTextHelper.getRefreshDeviceListButton(),
				Action.callback(ACTION_TYPE_VIEW_DEVICE_LIST)));
		rowsInline.add(row);
	}

	public InlineKeyboardMarkup getRefreshDevicesListInlineKeyboard() {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		List<InlineKeyboardButton> row = new ArrayList<>();
		row.add(createInlineKeyboardButton(ItemTextHelper.getRefreshDeviceListButton(),
				Action.callback(ACTION_TYPE_VIEW_DEVICE_LIST)));
		rowsInline.add(row);

		markup.setKeyboard(rowsInline);
		return markup;
	}

	public InlineKeyboardMarkup getDevicesOfServerInlineKeyboardView(List<Device> devices) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
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
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		rowsInline.add(createInlineKeyboardButtonRow(ItemTextHelper.getEditAlarmsButton(),
				Action.callback(ACTION_TYPE_EDIT_ALARMS, "", "")));
		rowsInline.add(createInlineKeyboardButtonRow(ItemTextHelper.getEditPropertiesButton(),
				Action.callback(ACTION_TYPE_EDIT_DEVICE_SELECT_LIST, "", "")));
		rowsInline.add(createInlineKeyboardButtonRow(ItemTextHelper.getRestartApplicationButton(),
				Action.callback(ACTION_TYPE_RESTART_APPLICATION, "", "")));

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getAlarmsMenuKeyboard(List<Device> devices) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();

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
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();

		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		for (IEntity entity : entities) {

			String text = buttons.getEntityButton(entity);
			String callback = Action.callback(ACTION_TYPE_EDIT_ALARMS_OF_ENTITY, "", ID_TYPE_ENTITY, entity.getId());

			rowsInline.add(createInlineKeyboardButtonRow(text, callback));
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getEntityFieldsAlarmsOfEntityMenuKeyboard(IEntity entity, IEntityAlarm entityAlarm,
			List<IEntityField> entityFields) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();

		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		if (entityAlarm == null) {
			rowsInline.add(createInlineKeyboardButtonRow(ENTITY_ALARM_CREATE_MESSAGE,
					Action.callback(ACTION_TYPE_CREATE_ALARM_OF_ENTITY, "", ID_TYPE_ENTITY, entity.getId())));
		} else {

			if (entityAlarm.isActivated()) {
				rowsInline.add(createInlineKeyboardButtonRow(ENTITY_ALARM_DEACTIVATE_MESSAGE, Action.callback(
						ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
				
				String soundMessage = entityAlarm.isSound()?ENTITY_ALARM_DISABLE_SOUND_MESSAGE:ENTITY_ALARM_ENABLE_SOUND_MESSAGE;
				
				rowsInline.add(createInlineKeyboardButtonRow(soundMessage, Action.callback(
						ACTION_ALARM_OF_ENTITY_CHANGE_SOUND, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
				
				rowsInline.add(createInlineKeyboardButtonRow(ENTITY_ALARM_EDIT_INTERVAL_MESSAGE,
						Action.callback(ACTION_TYPE_EDIT_ALARM_INTERVAL_OF_ENTITY,
								Action.createActionDataFieldByFieldValue(), ID_TYPE_ENTITY_ALARM,
								entityAlarm.getId())));
				
				if (entityAlarm.isNotificationRepeatable()) {
					rowsInline.add(createInlineKeyboardButtonRow(ENTITY_ALARM_REMOVE_INTERVAL_MESSAGE, Action.callback(
							ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
				}
			} else {
				rowsInline.add(createInlineKeyboardButtonRow(ENTITY_ALARM_ACTIVATE_MESSAGE, Action.callback(
						ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION, "", ID_TYPE_ENTITY_ALARM, entityAlarm.getId())));
			}

			for (IEntityField entityField : entityFields) {

				rowsInline.add(createInlineKeyboardButtonRow(ItemTextHelper.getEntityFieldButtonText(entityField),
						Action.callback(ACTION_TYPE_EDIT_ALARMS_OF_ENTITY_FIELD, "", ID_TYPE_ENTITY_FIELD,
								entityField.getId())));

			}
		}
		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getEntityFieldsAlarmEditMenuKeyboard(IEntityAlarm entityAlarm, IEntityField entityField,
			Map<Integer, Class> enabledAlarmClasses) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();

		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();
		Map<String, IEntityFieldAlarm> existingAlarms = new HashMap<>();

		String data = Action.createActionDataFieldByFieldValue();

		// remove/edit existing alarms
		for (IEntityFieldAlarm entityFieldAlarm : entityAlarm.getAlarms()) {

			if(entityFieldAlarm.getWatchedItem().getId().equals(entityField.getId())) {
				existingAlarms.put(entityFieldAlarm.getClass().getName(), entityFieldAlarm);
	
				rowsInline.add(createInlineKeyboardButtonRow(buttons.getEditEntityFieldAlarmButton(entityFieldAlarm), Action.callback(
						ACTION_EDIT_ENTITY_FIELD_ALARM, data, ID_TYPE_ENTITY_ALARM_FIELD, entityFieldAlarm.getId())));
				
				rowsInline.add(createInlineKeyboardButtonRow(buttons.getDeleteEntityFieldAlarmButton(entityFieldAlarm),
						Action.callback(ACTION_DELETE_ENTITY_FIELD_ALARM, "", ID_TYPE_ENTITY_ALARM_FIELD,
								entityFieldAlarm.getId())));
			}
		}
		// add alarms
		for (Entry<Integer, Class> entry : enabledAlarmClasses.entrySet()) {

			Class clazz = entry.getValue();

			if (!existingAlarms.containsKey(clazz.getName())) {

				data = Action.createActionDataFieldByFieldValueAndClassName(entry.getKey().toString());

				rowsInline.add(createInlineKeyboardButtonRow(buttons.getAddEntityFieldAlarmButton(clazz), Action
						.callback(ACTION_ADD_ENTITY_FIELD_ALARM, data, ID_TYPE_ENTITY_FIELD, entityField.getId())));
			}
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getDevicesOfServerInlineKeyboardEdit(List<Device> devices) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		for (IDevice device : devices) {

			ActionContext ac = new ActionContext(device);

			String text = buttons.getDeviceButton(ac.getEmoji(), ac.getDescription());
			String callback = Action.callback(ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE, "", device.getName());

			rowsInline.add(createInlineKeyboardButtonRow(text, callback));
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getGroupsOfDeviceInlineKeyboard(IDevice device) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		for (IGroup group : device.getGroups()) {
			if (!GROUP_DEVICE.equals(group.getName())) {

				rowsInline.add(createInlineKeyboardButtonRow(buttons.getGroupButton(group.getName()),
						Action.callback(ACTION_TYPE_VIEW_ENTITIES_OF_GROUP, "", device.getName(), group.getName())));
			}
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getEntitiesOfDeviceInlineKeyboard(IDevice device) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		List<InlineKeyboardButton> rowD = new ArrayList<>();

		String textD = ItemTextHelper.getButton(device.getEmoji(),
				device.getDescription() + " : " + "редактировать описание");

		String data = Action.createActionDataFieldByFieldValue(DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION);

		String callbackD = Action.callback(ACTION_TYPE_EDIT_DEVICE_DESCRIPTION, data, device.getName());

		rowD.add(createInlineKeyboardButton(textD, callbackD));

		rowsInline.add(rowD);

		device.getEntities().stream().sorted((e1, e2) -> e1.getName().compareToIgnoreCase(e2.getName()))
				.forEach(entity -> {

					List<InlineKeyboardButton> row = new ArrayList<>();

					String text = ItemTextHelper.getButton(entity.getEmoji(), entity.getDescription());
					String callback = Action.callback(ACTION_TYPE_EDIT_ENTITITY, "", ID_TYPE_ENTITY, entity.getId());

					row.add(createInlineKeyboardButton(text, callback));

					rowsInline.add(row);
				});

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getFieldsOfEntityInlineKeyboard(IEntity entity) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		entity.getEntityFields().stream().sorted((ef1, ef2) -> ef1.getName().compareToIgnoreCase(ef2.getName()))
				.forEach(entityField -> {

					List<Action> actions = EntityViewBuilder.getCommandButtonsForEntity(ACTION_TYPE_SEND_DATA_TO_DEVICE,
							entity, entityField);

					if (actions != null && !actions.isEmpty()) {

						for (Action action : actions) {
							String text = action.getDescription();
							String callback = action.getCallbackData();

							rowsInline.add(createInlineKeyboardButtonRow(text, callback));
						}

					} else if (!entityField.isReadOnly()
							&& !DeviceConstants.ENTITY_FIELD_ID.equals(entityField.getName())) {

						String description = StringUtils.isEmpty(entityField.getDescription())
								? entityField.getValueStr()
								: entityField.getDescription();

						String text = buttons.getButton(BotMessageConstants.EDIT_DEVICE_SELECT_ENTITY_FIELD_BUTTON,
								entityField.getName(), description);

						String data = Action.createActionDataFieldByFieldValue(entityField.getName());

						String callback = Action.callback(ACTION_TYPE_EDIT_ENTITITY_FIELD, data, entity);

						rowsInline.add(createInlineKeyboardButtonRow(text, callback));
					}
				});

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getEntitiesOfGroupInlineKeyboard(IDevice device, IGroup group) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		String callback = Action.callback(ACTION_TYPE_VIEW_GROUPS_OF_DEVICE, "", device.getName(), group.getName());

		rowsInline.add(createInlineKeyboardButtonRow(buttons.getDeviceButton(device.getDescription()), callback));

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getCommandButtonsByEnabledFieldCommandButtonList(List<Action> actions) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		for (Action action : actions) {
			rowsInline.add(Collections
					.singletonList(createInlineKeyboardButton(action.getDescription(), action.getCallbackData())));
		}

		markup.setKeyboard(rowsInline);

		return markup;
	}

	public InlineKeyboardMarkup getToggleOnOffInlineKeyboard(String text, String onText, String offText,
			String actionOn, String actionOff) {
		InlineKeyboardMarkup markup = new InlineKeyboardMarkup();
		List<List<InlineKeyboardButton>> rowsInline = new ArrayList<>();

		List<InlineKeyboardButton> row = new ArrayList<>();

		row.add(createInlineKeyboardButton(ItemTextHelper.getButton(null, onText), actionOn));

		row.add(createInlineKeyboardButton(ItemTextHelper.getButton(null, offText), actionOff));

		rowsInline.add(row);

		markup.setKeyboard(rowsInline);

		return markup;
	}

}
