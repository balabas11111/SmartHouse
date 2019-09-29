package com.balabas.smarthouse.telegram.bot.handler;

import java.util.List;

import javax.xml.transform.TransformerException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.starter.AfterBotRegistration;

import com.balabas.smarthouse.server.service.DeviceService;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_DEVICE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_GROUPS_OF_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ENTITIES_OF_GROUP;

@Log4j2
@Component
public class SmartHouseBotHandler extends BaseLogPollingBotHandler {

	@Autowired
	DeviceService deviceService;

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
		Action action = messageBuilder.getReplyKeyboard().getActionByReplyButton(data);

		ReplyContext cont = ReplyContext.builder()
				.replyToMessageId(message.getMessageId())
				.replyToMessageIdIfUnknown(message.getMessageId())
				.chatId(message.getChatId()).build();
		
		List<SendMessage> msgs =
				executeReplyAction(action, cont);

		sendMessages(msgs);
	}

	private void processCallback(CallbackQuery callback) {
		Action action = Action.fromCallbackData(callback.getData());
		
		ReplyContext cont = ReplyContext.builder()
				.replyToMessageIdIfUnknown(callback.getMessage().getMessageId())
				.chatId(callback.getMessage().getChatId()).build();
		
		List<SendMessage> msgs =
				executeReplyAction(action, cont);

		sendMessages(msgs);
	}
	
	private List<SendMessage> executeReplyAction(Action action, ReplyContext context) {
		List<SendMessage> msgs = Lists.newArrayList();
		
		try {
			switch (action.getAction()) {
			case ACTION_TYPE_VIEW_DEVICE_LIST:
				msgs.add(messageBuilder.createDevicesListInlineKeyboard(authService.getServerName(), context));
				//messageBuilder.createDevicesListInlineKeyboard(authService.getServerName(), context);
				//msgs.addAll(messageBuilder.createRefreshDevicesListReplyKeyboard(context));
				break;
			case ACTION_TYPE_VIEW_GROUPS_OF_DEVICE:
				msgs.add(messageBuilder.createGroupsOfDeviceInlineKeyboard(action.getDeviceId(),context));
				break;
			case ACTION_TYPE_VIEW_ENTITIES_OF_GROUP:
				msgs.addAll(messageBuilder.createGroupView(action.getDeviceId(), 
						action.getGroupId(), context));
				break;
			case ACTION_TYPE_SEND_DATA_TO_DEVICE:
					deviceService.processDeviceAction(action);
					action.setActionRebuildData(ACTION_TYPE_VIEW_ENTITIES_OF_GROUP);
					executeReplyAction(action, context);
					break;
			default:
				context.setText(null);
				msgs.add(messageBuilder.createUnknown(context));
			}
		} catch (NullPointerException | TransformerException ex) {
			log.error(ex);
			msgs.add(messageBuilder.createError(null, context.getChatId()));
		}
		
		return msgs;
	}

	@AfterBotRegistration
	public void sendServerStartedToAllUsers() {
		authService.getAllowedUserIds().stream().forEach(chatId -> { 
			sendMessages(messageBuilder.createServerStartedMessages(chatId.longValue(), authService.getServerName()));
			//sendMessage(messageBuilder.createHideReplyKeyboardMessage(chatId.longValue(), null));
			//sendMessage(messageBuilder.createRefreshDevicesListReplyKeyboard(chatId.longValue()));
		});
		
	}

	public void sendDeviceRegisteredToAllUsers(String deviceName) {
		authService.getAllowedUserIds().stream().forEach(chatId -> {
			sendMessages(messageBuilder.createDeviceRegisteredMessages(deviceName, chatId.longValue() ));
			//sendMessages(messageBuilder.createRefreshDevicesListReplyKeyboard(chatId.longValue()));
		});
	}

}
