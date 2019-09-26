package com.balabas.telegram.bot.service.auth;

import com.balabas.telegram.bot.model.TelegramMessage;

import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.Message;
import io.fouad.jtb.core.beans.User;

public interface TelegramAuthService {

    boolean checkAuth(CallbackQuery callbackQuery);
    
    boolean checkAuth(Message msg);
    
    boolean checkAuth(User user);
    
    boolean checkAuth(TelegramMessage message);
    
    boolean checkAuth(int id, String userName);
}
