package com.balabas.smarthouse.telegram.bot.handler;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.CallbackQuery;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.Update;

import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.server.service.DeviceService;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.AfterBotRegistration;
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
		if("/input".equals(data)){
			/*
			EditMessageText editMarkup = new EditMessageText();
			editMarkup.setChatId(callbackquery.getMessage().getChatId().toString());
			editMarkup.setInlineMessageId(callbackquery.getInlineMessageId());
			editMarkup.setText("[​](" + this.urls.get(index)[1] + ")");
			editMarkup.enableMarkdown(true);
			editMarkup.setMessageId(callbackquery.getMessage().getMessageId());
			editMarkup.setReplyMarkup(markup);
			*/
		}
		
		
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
		/*
		EditMessageText editMarkup = new EditMessageText();
		editMarkup.setChatId(callback.getMessage().getChatId().toString());
		editMarkup.setInlineMessageId(callback.getInlineMessageId());
		editMarkup.setText("[​](" + this.urls.get(index)[1] + ")");
		editMarkup.enableMarkdown(true);
		editMarkup.setMessageId(callback.getMessage().getMessageId());
		editMarkup.setReplyMarkup(markup);
		*/
		ReplyContext cont = ReplyContext.builder()
				//.replyToMessageId(callback.getMessage().getMessageId())
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
				msgs.addAll(messageBuilder.createDevicesListInlineKeyboard(authService.getServerName(), context));
				break;
			case ACTION_TYPE_VIEW_GROUPS_OF_DEVICE:
				msgs.add(messageBuilder.createGroupsOfDeviceInlineKeyboard(action.getDeviceId(),context));
				break;
			case ACTION_TYPE_VIEW_ENTITIES_OF_GROUP:
				msgs.addAll(messageBuilder.createGroupView(action.getDeviceId(), 
						action.getGroupId(), context));
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
			
			msgs.add(messageBuilder.createDeviceRefreshed(null, context.getChatId()));
			msgs.addAll(messageBuilder.createGroupView(action.getDeviceId(), action.getGroupId(), context));
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
	
	@SuppressWarnings("rawtypes")
	public void sendNotificationToAllUsers(Notification notification) {
		String text = messageBuilder.createNotificationMessage(notification);
		
		authService.getAllowedUserIds().stream().forEach(chatId -> 
			sendMessage(messageBuilder.createHtmlMessage(chatId.longValue(), text))
		);
	}

}
