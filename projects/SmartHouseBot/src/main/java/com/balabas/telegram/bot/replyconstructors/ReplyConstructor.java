package com.balabas.telegram.bot.replyconstructors;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

import org.json.JSONException;
import org.xml.sax.SAXException;

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
            TelegramMessage replyMessage) throws UnsupportedEncodingException{
        return null;
    }
    
    default TelegramMessage constructReplyMessage(CallbackQuery callbackQuery,
            TelegramMessage replyMessage) throws UnsupportedEncodingException, JSONException, ParserConfigurationException, SAXException, IOException, TransformerException{
        return null;
    }
}
