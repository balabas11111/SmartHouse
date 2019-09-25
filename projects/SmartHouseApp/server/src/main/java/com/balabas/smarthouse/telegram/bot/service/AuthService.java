package com.balabas.smarthouse.telegram.bot.service;

import java.util.Set;

public interface AuthService {

    Set<String> getAllowedUserIds();
    
    boolean isUserIdAllowed(String id);
    
    boolean isUserNameAllowed(String name);
    
    String getBotToken();
    
    String getBotName();
    
}
