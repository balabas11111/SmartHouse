package com.balabas.telegram.bot.replyconstructors;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import io.fouad.jtb.core.beans.InlineKeyboardButton;
import io.fouad.jtb.core.beans.InlineKeyboardMarkup;
import io.fouad.jtb.core.beans.Message;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.telegram.bot.model.TelegramMessage;
import com.balabas.telegram.bot.service.TelegramComponentsUtil;
import com.balabas.telegram.bot.service.server.SmartHouseServerService;

import static com.balabas.telegram.bot.BotConstants.DEVICE_PREFFIX;

@Component
public class ReplyConstructorGetDevicesList implements ReplyConstructor {

    @Autowired
    private SmartHouseServerService service;

    @Override
    public boolean isTarget(Message messageIn) {
        String msg = messageIn.getText();

        return "/d".equals(msg) || "/dev".equals(msg) || "/devices".equals(msg);
    }

    @Override
    public TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage) {

        List<Device> devices = service.getDevices();

        InlineKeyboardMarkup replyMarkup = TelegramComponentsUtil
                .createInlineKeyboardMarkup();

        InlineKeyboardButton[][] buttons = new InlineKeyboardButton[1][devices
                .size()];

        int i = 0;
        for (Device device : devices) {

            buttons[0][i] = new InlineKeyboardButton(device.getDeviceDescr(), null, DEVICE_PREFFIX+device.getDeviceId(),
                    null);
            i++;
        }

        replyMarkup.setInlineKeyboard(buttons);

        replyMessage.setMessage("<b>Выберите устройство</b></br>");
        replyMessage.setReplyMarkup(replyMarkup);

        return replyMessage;
    }

}
