package com.balabas.smarthouse.telegram.bot.handler;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.service.IDeviceService;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.AfterBotRegistration;
import com.balabas.smarthouse.telegram.bot.message.ActionIdentity;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_SELECT_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ENTITITY_FIELD;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_HELP;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;

import static com.balabas.smarthouse.server.view.Action.ACTION_DATA_FIELD_NAME;

@Log4j2
@Component
public class SmartHouseBotHandler extends BaseLogPollingBotHandler {

	@Autowired
	IDeviceService deviceService;
	
	private Map<Long, Action> currentEditActions = new HashMap<>();

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
		
		Action action = 
				(currentEditActions.containsKey(message.getChatId()))?
						getFieldEditActionByReply(message.getChatId(), data):
						ActionIdentity.getByData(data);	
		
		ReplyContext cont = ReplyContext.builder()
				.replyToMessageId(message.getMessageId())
				.replyToMessageIdIfUnknown(message.getMessageId())
				.chatId(message.getChatId()).build();
		
		List<SendMessage> msgs =
				executeReplyAction(action, cont);

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
				//.replyToMessageId(callback.getMessage().getMessageId())
				.replyToMessageIdIfUnknown(callback.getMessage().getMessageId())
				.chatId(callback.getMessage().getChatId()).build();
		
		List<SendMessage> msgs =
				executeReplyAction(action, cont);

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
		
		try {
			switch (action.getAction()) {
			case ACTION_TYPE_VIEW_DEVICE_LIST:
				msgs.addAll(messageBuilder.createDevicesListView(authService.getServerName(), context));
				//msgs.addAll(messageBuilder.createRefreshDevicesListReplyKeyboard(context));
				break;
			case ACTION_TYPE_EDIT_DEVICE_SELECT_LIST:
				msgs.add(messageBuilder.createDevicesListEdit(authService.getServerName(), context));
				break;
			case ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE:
				msgs.add(messageBuilder.getEntitiesOfDeviceToEdit(action, context));
				break;
			case ACTION_TYPE_EDIT_ENTITITY:
				msgs.add(messageBuilder.getFieldsOfDeviceToEdit(action, context));
				break;
			case ACTION_TYPE_EDIT_ENTITITY_FIELD:
				msgs.add(messageBuilder.getFieldToEdit(action, context));
				action.setActionRebuildData(ACTION_TYPE_SEND_DATA_TO_DEVICE);
				currentEditActions.put(context.getChatId(), action);
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
			case ACTION_TYPE_SEND_DATA_TO_DEVICE:
				sendDataToDevice(action, context, msgs);
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
	
	private void sendDataToDevice(Action action, ReplyContext context, List<SendMessage> msgs){
		try{
			deviceService.processDeviceAction(action);

			action.setGroupId(ItemType.SENSORS.getCode());
			
			msgs.add(messageBuilder.createDeviceRefreshed(null, context.getChatId()));
			msgs.addAll(messageBuilder.createGroupView(action, context));
		}catch(Throwable e){
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
			sendMessages(messageBuilder.createDeviceRegisteredMessages(deviceName, chatId.longValue() ));
		});
	}
	
}
