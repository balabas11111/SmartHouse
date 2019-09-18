package com.balabas.telegram.bot.replyconstructors;

import org.springframework.stereotype.Component;

import io.fouad.jtb.core.beans.Message;

import com.balabas.telegram.bot.model.TelegramMessage;

@Component
public class ReplyConstructorSettingsMessage implements ReplyConstructor {

    @Override
    public boolean isTarget(Message messageIn) {
        return "/settings".equals(messageIn.getText());
    }

    @Override
    public TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage) {
        
        replyMessage.setMessage("*Tune your brain*");
        return replyMessage;
    }

}
