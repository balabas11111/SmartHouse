package com.balabas.smarthouse.server.entity.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.descriptor.Severity;
import com.balabas.smarthouse.server.entity.model.descriptor.State;

import static com.balabas.smarthouse.server.DeviceMessageConstants.buildMessage;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_RECONNECTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_INITIALIZED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_DISCONNECTED;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class DeviceStateChangeService implements IDeviceStateChangeService {

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

			if (State.REREGISTERED.equals(newState)) {
				
				onDeviceReRegistered(device);
				
			} else if (State.INIT_DATA_RECEIVED.equals(newState)) {
				
				onDeviceInitialDataReceived(device);
				
			} else if (State.UPDATED.equals(newState)) {
				
				onDeviceDataUpdated(device);
				
			} else if (State.DISCONNECTED.equals(newState) && !State.TIMED_OUT.equals(oldState)) {
				
				onDeviceDisconnected(device);

			} else if (State.TIMED_OUT.equals(newState) && !State.DISCONNECTED.equals(oldState)) {
				// device data is too old
			}

		}

		if (setNewState) {
			device.setState(newState);
		}

	}

	private void onDeviceReRegistered(IDevice device) {
		String message = buildMessage(MSG_DEVICE_RECONNECTED, device.getDescription());
		alarmService.activateAlarms(device);
		sender.sendMessageToAllUsers(Severity.WARN, message);
		
		log.warn(message);
	}

	private void onDeviceInitialDataReceived(IDevice device) {
		// init data was received
		alarmService.activateAlarms(device);
		alarmService.checkAlarmsSendNotifications(device);

		String message = buildMessage(MSG_DEVICE_INITIALIZED, device.getDescription());
		
		sender.sendMessageToAllUsers(Severity.INFO,  message);
		device.setState(State.CONNECTED);
		device.setInitialized(true);
		
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
