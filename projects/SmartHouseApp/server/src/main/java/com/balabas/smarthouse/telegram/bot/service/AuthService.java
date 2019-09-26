package com.balabas.smarthouse.telegram.bot.service;

import java.util.Set;

public interface AuthService {

    Set<Integer> getAllowedUserIds();
    
    boolean isUserIdAllowed(Integer id);
    
    boolean isUserNameAllowed(String name);
    
    String getBotToken();
    
    String getBotName();
    
    String getServerName();
    
}
