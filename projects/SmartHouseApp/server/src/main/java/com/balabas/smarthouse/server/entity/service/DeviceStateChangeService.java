package com.balabas.smarthouse.server.entity.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.Severity;
import com.balabas.smarthouse.server.entity.model.State;

@Service
public class DeviceStateChangeService implements IDeviceStateChangeService {

	@Autowired
	private IMessageSender sender;
	
	@Override
	public void stateChanged(IDevice device, State newState) {
		State oldState = device.getState();
		boolean setNewState = false;

		if (oldState != null && !oldState.equals(newState)) {

			setNewState = true;

			if (State.REREGISTERED.equals(newState)) {
				sender.sendMessageToAllUsers(Severity.WARN, "Устройство передподключено : " + device.getDescription());
			} else
			if (State.INIT_DATA_RECEIVED.equals(newState)) {
				//init data was received
				//TODO: watch items here
				
				sender.sendMessageToAllUsers(Severity.INFO, "Устройство инициализировано : " + device.getDescription());
				device.setState(State.CONNECTED);
				
			} else if (State.UPDATED.equals(newState)) {
				//device data was updated
				//TODO: watch items here
				
				device.setState(State.CONNECTED);
				
			} else if (State.DISCONNECTED.equals(newState) && !State.TIMED_OUT.equals(oldState)) {
				//device received bad data or request error
				sender.sendMessageToAllUsers(Severity.ERROR, "Устройство отключено : " + device.getDescription());
				
				
			} else if (State.TIMED_OUT.equals(newState) && !State.DISCONNECTED.equals(oldState)) {
				//device data is too old
			}

		}

		if (setNewState) {
			device.setState(newState);
		}

	}

}
