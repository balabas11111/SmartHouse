package com.balabas.telegram.bot.service;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.Message;
import io.fouad.jtb.core.beans.ReplyMarkup;
import io.fouad.jtb.core.builders.ApiBuilder;
import io.fouad.jtb.core.enums.ChatAction;
import io.fouad.jtb.core.enums.ParseMode;
import lombok.extern.log4j.Log4j2;

import org.springframework.stereotype.Service;

import com.balabas.telegram.bot.model.TelegramMessage;

@Log4j2
@Service
public class TelegramMsgSenderImpl implements TelegramMsgSender {

    public static final String BAD_MESSAGE = "<b>400</b> bad message";
    public static final String NOT_AUTHORIZED_MESSAGE = "<b>403</b> not authorized";
    public static final String DONT_KNOW_YOU = "Я ни знаю кто ты ваще. Не пеши мне больше";

    public static final int SENDER_MODE_TEXT = 0;
    public static final int SENDER_MODE_KEYBOARD = 1;

    @Override
    public void sendNotAuthorized(TelegramBotApi arg0, Message msg) {
        sendNotAuthorized(arg0, msg.getChat().getId());
    }

    @Override
    public void sendNotAuthorized(TelegramBotApi arg0, long chatId) {
        sendTextMessage(arg0, NOT_AUTHORIZED_MESSAGE, chatId);
    }

    @Override
    public void sendBadMessage(TelegramBotApi arg0, long chatId) {
        sendTextMessage(arg0, BAD_MESSAGE, chatId);
    }

    @Override
    public void sendTextMessage(TelegramBotApi arg0, String msg, long chatId) {
        try {
            ApiBuilder.api(arg0).sendChatAction(ChatAction.TYPING)
                    .toChatId(chatId).execute();
            ApiBuilder.api(arg0).sendMessage(msg).toChatId(chatId)
                    .parseMessageAs(ParseMode.HTML).execute();
        } catch (Exception e) {
            log.error(e.getMessage());
            e.printStackTrace();
        }
    }

    @Override
    public void sendKeyboard(TelegramBotApi arg0, String msg, long chatId,
            ReplyMarkup replyMarkup) {
        try {
            ApiBuilder.api(arg0).sendMessage(msg).toChatId(chatId)
                    .applyReplyMarkup(replyMarkup).execute();
        } catch (Exception e) {
            log.error(e.getMessage());
            e.printStackTrace();
        }
    }

    @Override
    public void sendMessage(TelegramMessage message) {
        if (message == null) {
            log.error("Message == null");
            return;
        }

        if (!message.isValid()) {
            log.error("Invalid Telegram Message");
            return;
        }

        switch (message.getMode()) {
        case TelegramMsgSenderImpl.SENDER_MODE_TEXT:
            sendTextMessage(message.getApi(), message.getMessage(),
                    message.getChatId());
            break;
        case TelegramMsgSenderImpl.SENDER_MODE_KEYBOARD:
            sendKeyboard(message.getApi(), message.getMessage(),
                    message.getChatId(), message.getReplyMarkup());
            break;
        default:
            sendBadMessage(message.getApi(), message.getChatId());
        }
    }

}
