package com.balabas.telegram.bot.replyconstructors;

import org.springframework.stereotype.Component;

import io.fouad.jtb.core.beans.Message;

import com.balabas.telegram.bot.model.TelegramMessage;

@Component
public class ReplyConstructorWhoisMessage implements ReplyConstructor {

    @Override
    public boolean isTarget(Message messageIn) {
        return messageIn.getText().startsWith("/whois");
    }

    @Override
    public TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage) {
        if(msgIn.getText().contains("Pushkin")){
            replyMessage.setMessage("<b>Мечков Павел Александрович - торчок</b>");
        }else{
            replyMessage.setMessage("не знаю такого...");
        }
        return replyMessage;
    }

}
