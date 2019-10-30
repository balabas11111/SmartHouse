package com.balabas.smarthouse.server.entity.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.descriptor.Severity;
import com.balabas.smarthouse.server.entity.model.descriptor.State;

import static com.balabas.smarthouse.server.DeviceMessageConstants.buildMessage;

import java.util.function.Consumer;

import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_RECONNECTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_INITIALIZED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_DISCONNECTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_BACK_FROM_DISCONNECT;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class DeviceStateChangeService implements IDeviceStateChangeService {

	@Value("${smarthouse.server.send.devicereconnect:true}")
	private boolean sendDeviceReconnect;
	
	@Autowired
	private IMessageSender sender;

	@Autowired
	IEntityAlarmService alarmService;

	@Override
	public void stateChanged(IDevice device, State newState) {
		State oldState = device.getState();
		boolean setNewState = false;

		if (oldState != null && !oldState.equals(newState)) {

			setNewState = true;
			
			Consumer<IDevice> function = this::emptyAction;

			if( State.DISCONNECTED.equals(oldState) && State.UPDATED.equals(newState)) {
				
				function = this::onDeviceBackFromDisconnect;
				
			} else if (State.REREGISTERED.equals(newState)) {
				
				function = this::onDeviceReRegistered;
				
			} else if (State.INIT_DATA_RECEIVED.equals(newState)) {
				
				function = this::onDeviceInitialDataReceived;
				
			} else if (State.UPDATED.equals(newState)) {
				
				function = this::onDeviceDataUpdated;
				
			} else if (State.DISCONNECTED.equals(newState) && !State.TIMED_OUT.equals(oldState)) {
				
				function = this::onDeviceDisconnected;

			} else if (State.TIMED_OUT.equals(newState) && !State.DISCONNECTED.equals(oldState)) {
				// device data is too old
			} 
			
			function.accept(device);
		}

		if (setNewState) {
			device.setState(newState);
		}

	}
	
	private void emptyAction(IDevice device) {
		log.debug("Empty action on " + device.getName());
	}

	private void onDeviceReRegistered(IDevice device) {
		String message = buildMessage(MSG_DEVICE_RECONNECTED, device.getDescription());
		alarmService.reattachAlarms(device);
		if(sendDeviceReconnect) {
			sender.sendMessageToAllUsers(Severity.WARN, message);
		}
		
		log.warn(message);
	}

	private void onDeviceInitialDataReceived(IDevice device) {
		// init data was received
		alarmService.reattachAlarms(device);
		alarmService.checkAlarmsSendNotifications(device);

		String message = buildMessage(MSG_DEVICE_INITIALIZED, device.getDescription());
		
		sender.sendMessageToAllUsers(Severity.INFO,  message);
		device.setState(State.CONNECTED);
		device.setInitialized(true);
		
		log.info(message);
	}
	
	private void onDeviceBackFromDisconnect(IDevice device) {
		String message = buildMessage(MSG_DEVICE_BACK_FROM_DISCONNECT, device.getDescription());
		sender.sendMessageToAllUsers(Severity.INFO,  message);
		
		onDeviceDataUpdated(device);
		
		log.info(message);
	}
	
	private void onDeviceDataUpdated(IDevice device){
		// device data was updated
		alarmService.checkAlarmsSendNotifications(device);
		
		device.setState(State.CONNECTED);
	}
	
	private void onDeviceDisconnected(IDevice device) {
		String message = buildMessage(MSG_DEVICE_DISCONNECTED, device.getDescription());
		log.error(message);
		sender.sendMessageToAllUsers(Severity.ERROR, message);
	}

}
