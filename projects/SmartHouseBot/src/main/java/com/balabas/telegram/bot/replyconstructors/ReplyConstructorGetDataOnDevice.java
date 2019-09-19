package com.balabas.telegram.bot.replyconstructors;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.XML;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.xml.sax.SAXException;

import io.fouad.jtb.core.beans.CallbackQuery;
import lombok.extern.log4j.Log4j2;

import com.balabas.telegram.bot.model.TelegramMessage;
import com.balabas.telegram.bot.service.TransformationService;
import com.balabas.telegram.bot.service.server.SmartHouseServerService;

import static com.balabas.telegram.bot.BotConstants.DEVICE_PREFFIX;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

@Log4j2
@Component
public class ReplyConstructorGetDataOnDevice implements ReplyConstructor {

	@Autowired
	private SmartHouseServerService serverService;
	
	@Autowired
	private TransformationService transformService;

	@Override
	public boolean isTarget(CallbackQuery callbackQuery){
		 String data = callbackQuery.getData();
		 
		return data.startsWith(DEVICE_PREFFIX); 
    }

	@Override
	public TelegramMessage constructReplyMessage(CallbackQuery callbackQuery, TelegramMessage replyMessage) throws JSONException, ParserConfigurationException, SAXException, IOException, TransformerException {

			String data = callbackQuery.getData();
			
			String deviceId =data.substring(DEVICE_PREFFIX.length());
			
			log.info("deviceid ="+deviceId);
			
			JSONObject deviceData = serverService.getDataOnDevice(deviceId, null);
			String result = transformService.transformToHtml(deviceData);

			//replyMessage.setMode(TelegramMessage.SENDER_MODE_MULTI);
			replyMessage.setMessage(result);
			
		return replyMessage;
	}

}
