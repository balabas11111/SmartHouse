package com.balabas.smarthouse.telegram.bot.handler;

import java.util.Collections;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.service.IActionService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.service.IServerManageService;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.AfterBotRegistration;
import com.balabas.smarthouse.telegram.bot.message.ActionIdentity;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;
import com.balabas.smarthouse.telegram.bot.service.CurrentEditActionService;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_APP_BUTTONS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_HIDE_REPLY_KEYBOARD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE_EDIT_FIELDS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SAVE_DEVICE_PROPERTY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_DEVICES;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_ALARMS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_MAIN_COMMANDS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_DEVICE_VALUE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_SELECT_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_RESTART_APPLICATION;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_HELP;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SETUP;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_DESCRIPTION;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_METRICS;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_METRIC;

@Log4j2
@Component
public class SmartHouseBotHandler extends BaseLogPollingBotHandler {

	@Autowired
	IDeviceManageService deviceService;

	@Autowired
	IActionService actionService;
	
	@Autowired
	IServerManageService serverManageService;
	
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
		action.setValueByKeyRemoveKey(data);
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
				serverManageService.restartServer(5);
				break;
			case ACTION_TYPE_VIEW_APP_BUTTONS:
			case ACTION_TYPE_VIEW_MAIN_COMMANDS:
				msgs.addAll(messageBuilder.createMainMenuReplyKeyboard(context));
				break;
			case ACTION_TYPE_HIDE_REPLY_KEYBOARD:
				msgs.add(messageBuilder.createHideReplyKeyboardMessage(action, context));
				break;
			case ACTION_TYPE_VIEW_ALL_DEVICES:
				msgs.addAll(messageBuilder.createViewOfDevices(action, context));
				break;
			case ACTION_TYPE_VIEW_ALL_METRICS:
				msgs.addAll(messageBuilder.createViewOfDevicesVirtual(action, context));
				break;
			case ACTION_TYPE_VIEW_ENTITIES_OF_METRIC:
				msgs.addAll(messageBuilder.createViewOfEntityMetrics(action, context));
				break;
			case ACTION_TYPE_VIEW_ALL_DEVICE_VALUE_LIST:
				msgs.addAll(messageBuilder.createViewOfAllDevicesGroup(action, context));
				break;
			case ACTION_TYPE_VIEW_ALL_ALARMS:
				msgs.addAll(messageBuilder.createViewOfValarmsV2(action, context));
				break;
			case ACTION_TYPE_SETUP:
				msgs.addAll(messageBuilder.createSetupMenu(action, context));
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
				msgs.addAll(messageBuilder.createViewOfDeviceGroup(action, context));
				break;
			case ACTION_TYPE_SAVE_DEVICE_PROPERTY:
				sendDataToDevice(action, context, msgs);
				msgs.add(messageBuilder.getEntitiesOfDeviceToEdit(action, context));
				break;
			case ACTION_TYPE_SEND_DATA_TO_DEVICE:
				sendDataToDevice(action, context, msgs);
				break;
			case ACTION_TYPE_SEND_DATA_TO_DEVICE_EDIT_FIELDS:
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
	
	private void rebuildAndSaveCurrentAction(Action action, ReplyContext context, String newActionName) {
		action.setActionRebuildData(newActionName);
		currentEditActions.put(context.getChatId(), action);
	}

	private void sendDataToDevice(Action action, ReplyContext context, List<SendMessage> msgs) {
		try {
			boolean sendExecuted = actionService.executeDeviceAction(action);

			if(StringUtils.isEmpty(action.getGroupName())) {
				action.setGroupName(ItemType.SENSORS.getCode());
			}

			if (sendExecuted) {
				if(ItemType.SENSORS.getCode().equals(action.getGroupName())) {
					String message = "";
					if (action.getDeviceName() != null && !action.getDeviceName().isEmpty()) {
						Device device = deviceService.getDeviceByName(action.getDeviceName());
	
						if (device != null) {
							message += device.getEmoji().toString() + " " + device.getDescription();
						}
					}
	
					msgs.add(messageBuilder.createDataSentToDevice(message, context.getChatId()));
					msgs.addAll(messageBuilder.createViewOfDeviceGroup(action, context));
					
					log.info("Sensors data sent to device");
				}
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
