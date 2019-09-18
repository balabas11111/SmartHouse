package com.balabas.telegram.bot.model;

import com.balabas.telegram.bot.service.TelegramMsgSenderImpl;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.ReplyMarkup;
import lombok.Data;

@Data
public class TelegramMessage {

    private int mode =-1;
    private TelegramBotApi api;
    private String message;
    
    private long chatId;
    private ReplyMarkup replyMarkup;
    
    public TelegramMessage(TelegramBotApi api, String message,
            long chatId) {
        super();
        this.mode = TelegramMsgSenderImpl.SENDER_MODE_TEXT;
        this.api = api;
        this.message = message;
        this.chatId = chatId;
    }

    public TelegramMessage(TelegramBotApi api, String message,
            long chatId, ReplyMarkup replyMarkup) {
        super();
        this.mode = TelegramMsgSenderImpl.SENDER_MODE_KEYBOARD;
        this.api = api;
        this.message = message;
        this.chatId = chatId;
        this.replyMarkup = replyMarkup;
    }
    
    public void setReplyMarkup(ReplyMarkup replyMarkup){
        this.mode = TelegramMsgSenderImpl.SENDER_MODE_KEYBOARD;
        this.replyMarkup = replyMarkup;
    }
    
   public boolean isValid(){
       return this.mode>-1 && this.api!=null
               && this.message!=null
               && this.chatId>0;
   }
}
