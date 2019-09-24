package com.balabas.smarthouse.server.bot;

import java.util.LinkedHashSet;
import java.util.Set;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

import org.springframework.beans.factory.BeanInitializationException;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

@Log4j2
@Service
public class TelegramAuthServiceImpl implements TelegramAuthService,
        InitializingBean {

    @Value("${telegram.bot.allowednames}")
    private String allowedNames;

    @Value("${telegram.bot.allowedids}")
    private String allowedIds;

    @Getter
    private Set<String> allowedUserIds;


    @Override
    public void afterPropertiesSet() throws Exception {
        String[] arrayNames = allowedNames.split(",");
        String[] arrayIds = allowedIds.split(",");

        if (arrayNames.length != arrayIds.length) {
            throw new BeanInitializationException("Bad length of allowed users");
        }

        allowedUserIds = new LinkedHashSet<String>();

        for (int i = 0; i < arrayNames.length; i++) {
            allowedUserIds.add(arrayIds[i]);

            log.info("add User Id" + arrayIds[i]);
        }
    }

}
