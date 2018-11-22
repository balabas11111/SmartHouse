package com.balabas.telegram.telbot;

import io.fouad.jtb.core.JTelegramBot;
import io.fouad.jtb.core.UpdateHandler;

/**
 * Hello world!
 *
 */
public class SimpleTelegramBot 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        String apiToken="737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";
        UpdateHandler handler=new SimpleUpdateHandler();
        
        JTelegramBot bot = new JTelegramBot("BotName", apiToken, handler);
        bot.start();
        System.out.println("bot started");
    }
}
