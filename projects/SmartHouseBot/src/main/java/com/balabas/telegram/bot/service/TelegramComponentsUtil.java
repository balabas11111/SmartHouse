package com.balabas.telegram.bot.service;

import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.InlineKeyboardButton;
import io.fouad.jtb.core.beans.InlineKeyboardMarkup;
import io.fouad.jtb.core.beans.Message;

public class TelegramComponentsUtil {

    private TelegramComponentsUtil() {   }

    public static InlineKeyboardMarkup createInlineKeyboardMarkup(){
        return new InlineKeyboardMarkup();
    }
    
    public static InlineKeyboardButton createInlineKeyboardButton(
            String buttonText, String url, String callBackData,
            String switchInlineQuery) {
        
        return new InlineKeyboardButton(buttonText, url, callBackData,
                switchInlineQuery);
    }
    
    public static long getChatId(CallbackQuery q){
        return getChatId(q.getMessage());
    }
    
    public static long getChatId(Message m){
        return m.getChat().getId();
    }
}
