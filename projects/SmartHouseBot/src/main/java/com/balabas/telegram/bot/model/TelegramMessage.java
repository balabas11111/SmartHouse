package com.balabas.telegram.bot.model;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.ReplyMarkup;
import lombok.Data;

@Data
public class TelegramMessage {

	public static final int SENDER_MODE_TEXT = 0;
    public static final int SENDER_MODE_KEYBOARD = 1;
    public static final int SENDER_MODE_MULTI = 2;
	
    private int mode =-1;
    private TelegramBotApi api;
    private String message;
    
    private long chatId;
    private ReplyMarkup replyMarkup;
    
    public TelegramMessage(TelegramBotApi api, String message,
            long chatId) {
        super();
        this.mode = SENDER_MODE_TEXT;
        this.api = api;
        this.message = message;
        this.chatId = chatId;
    }

    public TelegramMessage(TelegramBotApi api, String message,
            long chatId, ReplyMarkup replyMarkup) {
        super();
        this.mode = SENDER_MODE_KEYBOARD;
        this.api = api;
        this.message = message;
        this.chatId = chatId;
        this.replyMarkup = replyMarkup;
    }
    
    public void setReplyMarkup(ReplyMarkup replyMarkup){
        this.mode = SENDER_MODE_KEYBOARD;
        this.replyMarkup = replyMarkup;
    }
    
   public boolean isValid(){
       return this.mode>-1 && this.api!=null
               && this.message!=null
               && this.chatId>0;
   }
}
