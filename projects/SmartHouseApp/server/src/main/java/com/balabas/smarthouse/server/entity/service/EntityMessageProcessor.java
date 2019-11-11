package com.balabas.smarthouse.server.entity.service;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Severity;

import static com.balabas.smarthouse.server.DeviceConstants.MESSAGE_FIELD_SEVERITY;
import static com.balabas.smarthouse.server.DeviceConstants.MESSAGE_FIELD_CODE;
import static com.balabas.smarthouse.server.DeviceConstants.MESSAGE_FIELD_TEXT;
import static com.balabas.smarthouse.server.DeviceConstants.MESSAGE_CODE_DEVICE_WILL_BE_RESTARTED;

@Service
public class EntityMessageProcessor  implements IEntityMessageProcessor{

	public static final String MSG_DEVICE_WILL_BE_RESTARTED = "будет перезапущено";
	public static final String MSG_DEFAULT = "%s \n Устройство %s\n\n %s";
	
	@Autowired
	private IMessageSender messageSender;
	
	@Override
	public void processMessage(IEntity entity, String entityMessageText) {
		JSONObject json = new JSONObject(entityMessageText);
		
		Severity severity =  Severity.getSeveritybyCode(json.optString(MESSAGE_FIELD_SEVERITY, null));
		String code = json.optString(MESSAGE_FIELD_CODE);
		String text = json.optString(MESSAGE_FIELD_TEXT);
		
		if(!StringUtils.isEmpty(code)) {
			switch(code) {
			case MESSAGE_CODE_DEVICE_WILL_BE_RESTARTED :
				text = MSG_DEVICE_WILL_BE_RESTARTED;
				break;
			}
		} 
		
		text = String.format(MSG_DEFAULT, severity.getDescription(),
					entity.getDevice().getDescription(), text);
		
		
		messageSender.sendMessageToAllUsers(severity, text);
	}
	
}
