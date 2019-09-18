package com.balabas.telegram.bot.replyconstructors;

import org.springframework.stereotype.Component;

import io.fouad.jtb.core.beans.Message;

import com.balabas.telegram.bot.model.TelegramMessage;

@Component
public class ReplyConstructorHelpMessage implements ReplyConstructor {

    @Override
    public boolean isTarget(Message messageIn) {
        return "/help".equals(messageIn.getText());
    }

    @Override
    public TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage) {
        
        replyMessage.setMessage("<em>God will help</em> or just call to <b>balabas</b>");
        return replyMessage;
    }

}
