package com.balabas.telegram.bot.service;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.List;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

import lombok.extern.log4j.Log4j2;

import org.json.JSONException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.xml.sax.SAXException;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.Message;

import com.balabas.telegram.bot.BotConstants;
import com.balabas.telegram.bot.model.TelegramMessage;
import com.balabas.telegram.bot.replyconstructors.ReplyConstructor;
import com.balabas.telegram.bot.util.TelegramComponentsUtil;

@Log4j2
@Service
public class ReplyConstructServiceImpl implements ReplyConstructService {

    @Autowired
    List<ReplyConstructor> constructors;

    @Override
    public TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage) {

        log.info("constructReplyMessage messageText="+msgIn.getText()+" chatId="+msgIn.getChat().getId());

        ReplyConstructor constructor = getReplyConstructor(msgIn);

        if (constructor != null) {
            try {
            	replyMessage = constructor.constructReplyMessage(msgIn, replyMessage);
			} catch (Exception e) {
				log.trace(e);
				replyMessage.setMessage(BotConstants.DEVICE_ERROR_MSG);
			}
            
            return replyMessage;
        }

        return null;
    }

    private ReplyConstructor getReplyConstructor(Message msgIn) {
        for (ReplyConstructor constructor : constructors) {
            if (constructor.isTarget(msgIn)) {
                return constructor;
            }
        }
        return null;
    }
    
    private ReplyConstructor getReplyConstructor(CallbackQuery query) {
        for (ReplyConstructor constructor : constructors) {
            if (constructor.isTarget(query)) {
                return constructor;
            }
        }
        return null;
    }

    @Override
    public TelegramMessage constructReplyMessage(TelegramBotApi api,
            Message msgIn, long chatId) {
        long chat = chatId > 0 ? chatId : msgIn.getChat().getId();

        return constructReplyMessage(msgIn, new TelegramMessage(api, null,
                chat));
    }

    @Override
    public TelegramMessage constructReplyMessage(TelegramBotApi api,
            Message msgIn) {
        return constructReplyMessage(api, msgIn, 0);
    }

    @Override
    public TelegramMessage constructReplyMessage(TelegramBotApi api,
            CallbackQuery query) throws ParserConfigurationException, SAXException, IOException, TransformerException {
        
        log.info("constructReplyMessage queryText="+query.getData()+" chatId="+TelegramComponentsUtil.getChatId(query));
        
        ReplyConstructor constructor = getReplyConstructor(query);
        if (constructor != null) {
        	TelegramMessage replyMessage = new TelegramMessage(api, null,
			        query.getMessage().getChat().getId()); 
        	
            try {
            	replyMessage = constructor.constructReplyMessage(query, replyMessage );
			} catch (UnsupportedEncodingException | JSONException e ) {
				log.trace(e);
				replyMessage.setMessage(BotConstants.DEVICE_ERROR_MSG);
			}
            
            return replyMessage;
        }

        return null;
    }
}
