package com.balabas.smarthouse.server.entity.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.Severity;
import com.balabas.smarthouse.server.entity.model.State;

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
				
				onDeviceRegistered(device);
				
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

	private void onDeviceRegistered(IDevice device) {
		alarmService.activateAlarms(device);
		sender.sendMessageToAllUsers(Severity.WARN, "Устройство передподключено : " + device.getDescription());
	}

	private void onDeviceInitialDataReceived(IDevice device) {
		// init data was received
		alarmService.activateAlarms(device);
		alarmService.checkAlarms(device);

		sender.sendMessageToAllUsers(Severity.INFO, "Устройство инициализировано : " + device.getDescription());
		device.setState(State.CONNECTED);
	}
	
	private void onDeviceDataUpdated(IDevice device){
		// device data was updated
		alarmService.checkAlarms(device);
		sendAlarmNotifications(device);
		sendAlarmFinishedNotifications(device);
		
		device.setState(State.CONNECTED);
	}
	
	private void sendAlarmNotifications(IDevice device) {
		List<IEntityAlarm> alarms = alarmService.getAlarmsWithAlarmNotificationRequired(device);

		for (IEntityAlarm alarm : alarms) {
			try {
				alarm.setNotified(sender.sendHtmlMessageToAllUsers(alarm.getAlarmText()));
			} catch (Exception e) {
				log.error(e);
				alarm.setNotified(false);
			}
		}

	}
	
	private void sendAlarmFinishedNotifications(IDevice device) {
		List<IEntityAlarm> alarms = alarmService.getAlarmsWithAlarmFinished(device);

		for (IEntityAlarm alarm : alarms) {
			try {
				alarm.setNotified(sender.sendHtmlMessageToAllUsers(alarm.getAlarmText()));
			} catch (Exception e) {
				log.error(e);
				alarm.setNotified(false);
			}
		}
	}
	
	private void onDeviceDisconnected(IDevice device) {
		sender.sendMessageToAllUsers(Severity.ERROR, "Устройство отключено : " + device.getDescription());
	}

}
