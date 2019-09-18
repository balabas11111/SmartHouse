package com.balabas.telegram.bot.replyconstructors;

import com.balabas.telegram.bot.model.TelegramMessage;

import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.Message;

public interface ReplyConstructor {

    default boolean isTarget(Message messageIn){
        return false;
    }
    
    default boolean isTarget(CallbackQuery callbackQuery){
        return false;
    }
    
    default TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage){
        return null;
    }
    
    default TelegramMessage constructReplyMessage(CallbackQuery callbackQuery,
            TelegramMessage replyMessage){
        return null;
    }
}
