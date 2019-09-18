package com.balabas.telegram.bot.service;

import com.balabas.telegram.bot.model.TelegramMessage;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.Message;
import io.fouad.jtb.core.beans.ReplyMarkup;

public interface TelegramMsgSender {

    void sendNotAuthorized(TelegramBotApi arg0,Message msg);
    
    void sendNotAuthorized(TelegramBotApi arg0,long chatId);
    
    void sendBadMessage(TelegramBotApi arg0,long chatId);
    
    void sendTextMessage(TelegramBotApi arg0,String msg,long chatId);
    
    void sendKeyboard(TelegramBotApi arg0,String msg,long chatId,ReplyMarkup replyMarkup);
    
    void sendMessage(TelegramMessage message);
}
