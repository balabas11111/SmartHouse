package com.balabas.smarthouse.telegram.bot.service;

import java.util.HashSet;
import java.util.Set;

import lombok.Getter;

import org.springframework.beans.factory.BeanInitializationException;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

@Service
public class AuthServiceImpl implements AuthService,
        InitializingBean {
	
    @Value("${telegram.bot.allowednames}")
    private String allowedNames;

    @Value("${telegram.bot.allowedids}")
    private String allowedIds;
    
    @Value("${telegram.bot.apiToken}")
    @Getter
	private String botToken;
	
	@Value("${telegram.bot.botName}")
	@Getter
	private String botName;
	
	@Value("${smarthouse.server.name}")
	@Getter
	private String serverName;

	@Getter
    private Set<Integer> allowedUserIds = new HashSet<>();

    private Set<String> allowedUserNames = new HashSet<>();

    @Override
    public void afterPropertiesSet() throws Exception {
        String[] arrayNames = allowedNames.split(",");
        String[] arrayIds = allowedIds.split(",");

        if (arrayNames.length != arrayIds.length) {
            throw new BeanInitializationException("Bad length of allowed users");
        }

        for (int i = 0; i < arrayNames.length; i++) {
            allowedUserIds.add(Integer.valueOf(arrayIds[i]));
            allowedUserNames.add(arrayNames[i]);
        }
    }


	@Override
	public boolean isUserIdAllowed(Integer id) {
		return presented(id, allowedUserIds);
	}


	@Override
	public boolean isUserNameAllowed(String name) {
		return presented(name, allowedUserNames);
	}

	private <T> boolean  presented(T el, Set<T> set){
		return set.stream().anyMatch(s-> s.equals(el));
	}
	
}
