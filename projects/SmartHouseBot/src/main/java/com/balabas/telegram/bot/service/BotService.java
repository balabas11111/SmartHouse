package com.balabas.telegram.bot.service;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import io.fouad.jtb.core.JTelegramBot;
import io.fouad.jtb.core.UpdateHandler;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class BotService implements InitializingBean {

	@Value("${telegram.bot.apiToken}")
	private String apiToken;
	
	@Value("${telegram.bot.botName}")
	private String botName;
	
	@Override
	public void afterPropertiesSet() throws Exception {
		log.info( "Starting BotService botName="+botName+" apiToken="+apiToken );
        //String apiToken="737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";
        UpdateHandler handler=new SimpleUpdateHandler();
        
        JTelegramBot bot = new JTelegramBot(botName, apiToken, handler);
        bot.start();
        log.info("bot started");
		
	}

}
