package com.balabas.telegram.bot.service.auth;

import java.util.LinkedHashSet;
import java.util.Set;

import lombok.extern.log4j.Log4j2;

import org.springframework.beans.factory.BeanInitializationException;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.telegram.bot.model.TelegramMessage;

import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.Message;
import io.fouad.jtb.core.beans.User;

@Log4j2
@Service
public class TelegramAuthServiceImpl implements TelegramAuthService,
        InitializingBean {

    @Value("${telegram.bot.allowednames}")
    private String allowedNames;

    @Value("${telegram.bot.allowedids}")
    private String allowedIds;

    private Set<User> allowedUsers;

    @Override
    public boolean checkAuth(CallbackQuery callbackQuery) {
        return checkAuth(callbackQuery.getFrom());
    }

    @Override
    public boolean checkAuth(Message msg) {
        return checkAuth(msg.getFrom());
    }

    @Override
    public boolean checkAuth(User user) {
        return checkAuth(user.getId(), user.getUsername());
    }

    @Override
    public boolean checkAuth(int id, String userName) {
        
        //log.info("checkAuth id ="+id+" userName="+userName+" allowedUsers size="+allowedUsers.size());
        /*     
        for(User user: allowedUsers){
            
            if(user.getId() == id && user.getUsername().equals(userName)){
                return true;
            }
        }
        
        return false;
        */
        return allowedUsers.stream().anyMatch(
                user -> (user.getId() == id && user.getUsername().equals(
                        userName)));
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        String[] arrayNames = allowedNames.split(",");
        String[] arrayIds = allowedIds.split(",");

        if (arrayNames.length != arrayIds.length) {
            throw new BeanInitializationException("Bad length of allowed users");
        }

        allowedUsers = new LinkedHashSet<User>();

        for (int i = 0; i < arrayNames.length; i++) {
            int id = Integer.parseInt(arrayIds[i]);
            String userName = arrayNames[i];

            User user = new User(id, null, null, userName);
            allowedUsers.add(user);

            log.info("add User " + user.toString());
        }
    }

    @Override
    public boolean checkAuth(TelegramMessage message) {
        return allowedUsers.stream().anyMatch(
                user -> user.getId() == message.getChatId());
    }

}
