package com.balabas.telegram.bot.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.telegram.bot.model.TelegramMessage;
import com.balabas.telegram.bot.service.auth.TelegramAuthService;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.UpdateHandler;
import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.ChosenInlineResult;
import io.fouad.jtb.core.beans.InlineQuery;
import io.fouad.jtb.core.beans.Message;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class BotServiceUpdateHandler implements UpdateHandler {

	@Autowired
	private TelegramAuthService authService;

	@Autowired
	private TelegramMsgSender sender;

	@Autowired
	private ReplyConstructService replyService;

	public void onCallbackQueryReceived(TelegramBotApi api, int id, CallbackQuery query) {
		log.info("onCallbackQueryReceived");
		if (!authService.checkAuth(query)) {
			log.error("CallbackQuery Not authorized");
			return;
		}

		TelegramMessage message = null;

		try {
			message = replyService.constructReplyMessage(api, query);
		} catch (Exception e) {
			e.printStackTrace();
		}

		sender.sendMessage(message);
	}

	public void onChosenInlineResultReceived(TelegramBotApi api, int arg1, ChosenInlineResult arg2) {
		log.info("onChosenInlineResultReceived");
		log.info(api);
	}

	public void onEditedMessageReceived(TelegramBotApi api, int arg1, Message arg2) {
		log.info("onEditedMessageReceived");
		log.info(api);
	}

	public void onGetUpdatesFailure(Exception arg0) {
		log.info("onGetUpdatesFailure");
		log.info(arg0);
	}

	public void onInlineQueryReceived(TelegramBotApi api, int arg1, InlineQuery arg2) {
		log.info("onInlineQueryReceived");
		log.info(api);
	}

	public void onMessageReceived(TelegramBotApi api, int arg1, Message msg) {
		log.info("onMessageReceived " + msg.toString());
		if (!authService.checkAuth(msg)) {
			if (msg != null && msg.getChat() != null) {
				sender.sendNotAuthorized(api, msg.getChat().getId());
			}
			return;
		}

		TelegramMessage message = replyService.constructReplyMessage(api, msg);
		sender.sendMessage(message);
	}

}
