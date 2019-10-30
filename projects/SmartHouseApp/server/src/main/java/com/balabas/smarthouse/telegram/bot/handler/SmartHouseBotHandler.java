package com.balabas.smarthouse.telegram.bot.handler;

import java.util.Collections;
import java.util.List;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;

import com.balabas.smarthouse.server.ServerApplication;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.service.IActionService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.AfterBotRegistration;
import com.balabas.smarthouse.telegram.bot.message.ActionIdentity;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;
import com.balabas.smarthouse.telegram.bot.service.CurrentEditActionService;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SAVE_DEVICE_PROPERTY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_SELECT_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_RESTART_APPLICATION;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_HELP;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;

import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_NAME;
import static com.balabas.smarthouse.server.view.Action.ACTION_DELETE_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_EDIT_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_ADD_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_SAVE_NEW_ENTITY_FIELD_ALARM;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_CREATE_ALARM_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_SAVE_ENTITY_FIELD_ALARM;


import static com.balabas.smarthouse.server.view.Action.ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SETUP;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS_OF_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS_OF_ENTITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_DESCRIPTION;

@Log4j2
@Component
public class SmartHouseBotHandler extends BaseLogPollingBotHandler {

	@Autowired
	IDeviceManageService deviceService;

	@Autowired
	IActionService actionService;
	
	@Autowired
	IEntityAlarmService alarmService;

	@Autowired
	CurrentEditActionService currentEditActions;

	@Override
	public void onUpdateReceived(Update update) {
		if (update.hasMessage()) {
			Message message = update.getMessage();

			if (!checkAuthorization(message)) {
				sendMessage(messageBuilder.createUnauthorized(message.getChatId()));
			}

			processMessage(message);

		} else if (update.hasCallbackQuery()) {
			processCallback(update.getCallbackQuery());
		}
	}

	private void processMessage(Message message) {
		String data = message.getText();

		Action action = (currentEditActions.has(message.getChatId()))
				? getFieldEditActionByReply(message.getChatId(), data)
				: ActionIdentity.getByData(data);

		ReplyContext cont = ReplyContext.builder().replyToMessageId(message.getMessageId())
				.replyToMessageIdIfUnknown(message.getMessageId()).chatId(message.getChatId()).build();

		List<SendMessage> msgs = executeReplyAction(action, cont);

		sendMessages(msgs);
	}

	private Action getFieldEditActionByReply(Long chatId, String data) {
		Action action = currentEditActions.remove(chatId);
		action.setValueByKeyRemoveKey(ACTION_DATA_FIELD_NAME, data);
		return action;
	}

	private void processCallback(CallbackQuery callback) {
		Action action = Action.fromCallbackData(callback.getData());

		ReplyContext cont = ReplyContext.builder()
				// .replyToMessageId(callback.getMessage().getMessageId())
				.replyToMessageIdIfUnknown(callback.getMessage().getMessageId())
				.chatId(callback.getMessage().getChatId()).build();

		List<SendMessage> msgs = executeReplyAction(action, cont);

		try {
			sendMessages(msgs);
		} catch (Exception e) {
			msgs = Collections.singletonList(messageBuilder.createDeviceError(null, callback.getMessage().getChatId()));
			sendMessages(msgs);

		}
	}

	private List<SendMessage> executeReplyAction(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();

		currentEditActions.remove(context.getChatId());

		action = actionService.getDeviceGroupEntityNameActionByIdAction(action);
		action.setServerName(authService.getServerName());

		try {
			switch (action.getAction()) {
			case ACTION_TYPE_RESTART_APPLICATION:
				msgs.add(messageBuilder.createServerWillBeRestartedMsg(context));
				ServerApplication.restart();
				break;
			case ACTION_TYPE_VIEW_DEVICE_LIST:
				msgs.addAll(messageBuilder.createDevicesListView(action, context));
				break;
			case ACTION_TYPE_SETUP:
				msgs.addAll(messageBuilder.createSetupMenu(action, context));
				break;
			case ACTION_TYPE_EDIT_ALARMS:
				msgs.addAll(messageBuilder.createEditAlarmsMenu(action, context));
				break;
			case ACTION_TYPE_EDIT_ALARMS_OF_DEVICE:
				msgs.addAll(messageBuilder.createEditAlarmsOfDevice(action, context));
				break;
			case ACTION_TYPE_EDIT_ALARMS_OF_ENTITY:
				msgs.addAll(messageBuilder.createEditAlarmsOfEntity(action, context));
				break;
			case ACTION_TYPE_CREATE_ALARM_OF_ENTITY:
			case ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION:
			case ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY:
				sendDataToAlarmService(action, context, msgs);
				msgs.addAll(messageBuilder.createEditAlarmsOfEntity(action, context));
				break;
			case ACTION_TYPE_EDIT_ALARMS_OF_ENTITY_FIELD:
				msgs.addAll(messageBuilder.createEditAlarmsOfEntityField(action, context));
				break;
			case ACTION_EDIT_ENTITY_FIELD_ALARM:
				msgs.add(messageBuilder.getAlarmToBeSavedMessage(action, context));
				rebuildAndSaveCurrentAction(action, context, ACTION_SAVE_ENTITY_FIELD_ALARM);
				break;
			case ACTION_DELETE_ENTITY_FIELD_ALARM:
				sendDataToAlarmService(action, context, msgs);
				msgs.addAll(messageBuilder.createEditAlarmsOfEntity(action, context));
				break;
			case ACTION_ADD_ENTITY_FIELD_ALARM:
				msgs.add(messageBuilder.getAlarmToBeSavedMessage(action, context));
				rebuildAndSaveCurrentAction(action, context, ACTION_SAVE_NEW_ENTITY_FIELD_ALARM);
				break;
			case ACTION_SAVE_ENTITY_FIELD_ALARM:
			case ACTION_SAVE_NEW_ENTITY_FIELD_ALARM:
				sendDataToAlarmService(action, context, msgs);
				msgs.addAll(messageBuilder.createEditAlarmsOfEntity(action, context));
				break;
			case ACTION_TYPE_EDIT_DEVICE_SELECT_LIST:
				msgs.add(messageBuilder.createDevicesListEdit(action, context));
				break;
			case ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE:
				msgs.add(messageBuilder.getEntitiesOfDeviceToEdit(action, context));
				break;
			case ACTION_TYPE_EDIT_ENTITITY:
				msgs.add(messageBuilder.getFieldsOfEntityToEdit(action, context));
				break;
			case ACTION_TYPE_EDIT_ENTITITY_FIELD:
				msgs.add(messageBuilder.getFieldToEdit(action, context));
				rebuildAndSaveCurrentAction(action, context, ACTION_TYPE_SEND_DATA_TO_DEVICE);
				break;
			case ACTION_TYPE_EDIT_DEVICE_DESCRIPTION:
				msgs.add(messageBuilder.getDeviceDescriptionToEdit(action, context));
				rebuildAndSaveCurrentAction(action, context, ACTION_TYPE_SAVE_DEVICE_PROPERTY);
				break;
			case ACTION_TYPE_VIEW_HELP:
				msgs.add(messageBuilder.createHelpMsg(context));
				break;
			case ACTION_TYPE_VIEW_GROUPS_OF_DEVICE:
				msgs.add(messageBuilder.createGroupsOfDeviceInlineKeyboard(action, context));
				break;
			case ACTION_TYPE_VIEW_ENTITIES_OF_GROUP:
				msgs.addAll(messageBuilder.createGroupView(action, context));
				break;
			case ACTION_TYPE_SAVE_DEVICE_PROPERTY:
				sendDataToDevice(action, context, msgs);
				msgs.add(messageBuilder.getEntitiesOfDeviceToEdit(action, context));
				break;
			case ACTION_TYPE_SEND_DATA_TO_DEVICE:
				sendDataToDevice(action, context, msgs);
				msgs.add(messageBuilder.getEntitiesOfDeviceToEdit(action, context));
				break;
			default:
				context.setText(null);
				msgs.add(messageBuilder.createUnknown(context));
			}
		} catch (Exception ex) {
			log.error(ex);
			msgs.add(messageBuilder.createServerError(null, context.getChatId()));
		}

		return msgs;
	}
	
	private void sendDataToAlarmService(Action action, ReplyContext context, List<SendMessage> msgs) throws ReflectiveOperationException {
		switch (action.getAction()) {
		case ACTION_TYPE_CREATE_ALARM_OF_ENTITY:
			deviceService.createNewEntityAlarm(action.getTargetId());
			break;
		case ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION:
			alarmService.changeEntityAlarmActivation(action.getTargetId());
			break;
		case ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY:
			alarmService.removeMessageIntervalOnEntityAlarm(action.getTargetId());
			break;
		case ACTION_DELETE_ENTITY_FIELD_ALARM:
			alarmService.removeEntityFieldAlarm(action.getTargetId());
			break;
		case ACTION_SAVE_ENTITY_FIELD_ALARM:
			String val = new JSONObject(action.getData()).getString("value");
			
			alarmService.updateAlarmValueOfEntityAlarm(val, action.getTargetId());
			break;
		case ACTION_SAVE_NEW_ENTITY_FIELD_ALARM:
			JSONObject json = new JSONObject(action.getData());
			
			String newAlarmClassName = json.getString("class");
			String value = json.getString("value");
			
			alarmService.createNewEntityFieldAlarmInEntityAlarm(newAlarmClassName, value, action.getTargetId());
			break;
		}
		
		msgs.add(messageBuilder.createAlarmUpdatedMessage(action, context));
	}
	
	private void rebuildAndSaveCurrentAction(Action action, ReplyContext context, String newActionName) {
		action.setActionRebuildData(newActionName);
		currentEditActions.put(context.getChatId(), action);
	}

	private void sendDataToDevice(Action action, ReplyContext context, List<SendMessage> msgs) {
		try {
			boolean sendExecuted = actionService.executeDeviceAction(action);

			action.setGroupName(ItemType.SENSORS.getCode());

			if (sendExecuted) {
				String message = "";
				if (action.getDeviceName() != null && !action.getDeviceName().isEmpty()) {
					Device device = deviceService.getDeviceByName(action.getDeviceName());

					if (device != null) {
						message += device.getEmoji().toString() + " " + device.getDescription();
					}
				}

				msgs.add(messageBuilder.createDataSentToDevice(message, context.getChatId()));
				msgs.addAll(messageBuilder.createGroupView(action, context));
				
				log.info("Data sent to device");
			} else {
				msgs.add(messageBuilder.createDeviceDataSavedOnServer(null, context.getChatId()));
				log.info("Device properties updated");
			}

		} catch (Throwable e) {
			msgs.add(messageBuilder.createDeviceError(null, context.getChatId()));
		}
	}

	@AfterBotRegistration
	public void sendServerStartedToAllUsers() {
		authService.getAllowedUserIds().stream().forEach(chatId -> {
			sendMessages(messageBuilder.createServerStartedMessages(chatId.longValue(), authService.getServerName()));
		});

	}

	public void sendDeviceRegisteredToAllUsers(String deviceName) {
		authService.getAllowedUserIds().stream().forEach(chatId -> {
			sendMessages(messageBuilder.createDeviceRegisteredMessages(deviceName, chatId.longValue()));
		});
	}

}
