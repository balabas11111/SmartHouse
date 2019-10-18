package com.balabas.smarthouse.server.entity.alarm;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IEntityField;
import com.balabas.smarthouse.server.entity.model.Severity;
import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.balabas.smarthouse.server.entity.service.IMessageSender;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceMessageConstants.buildMessage;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_STARTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_FINISHED;

@Service
@Log4j2
public class EntityAlarmService implements IEntityAlarmService, InitializingBean {

	@Getter
	@Value("${smarthouse.server.repository.path}")
	private String storagePath = "repository/alarm/";

	@Getter
	@Value("${smarthouse.server.alarm.interval:30}")
	private Long alarmInterval;
	
	@Getter
	@Value("${smarthouse.server.alarm.singleAlarmMessage:true}")
	private boolean singleAlarmMessage;

	@Autowired
	private IMessageSender sender;

	List<IEntityAlarm> alarms = new ArrayList<>();

	@Override
	public void afterPropertiesSet() throws Exception {
		log.info("Alarm service started");
	}

	private File getStorageFile(boolean create) throws IOException {
		File file = new File(storagePath + this.getClass().getSimpleName() + ".json");
		log.info("Storage =" + file.getAbsolutePath());
		if (create && !file.exists()) {
			boolean created = file.createNewFile();
			log.info("file created " + created);
		}
		return file;

	}

	@Override
	public void loadAlarms() throws IOException {
		File file = getStorageFile(false);
		if (file.exists()) {
			ObjectMapper mapper = new ObjectMapper();

			List<IEntityAlarm> loaded = mapper.readValue(file, new TypeReference<List<IEntityAlarm>>() {
			});

			alarms = loaded;
		} else {
			log.warn("No alarm file loaded");
		}
	}

	@Override
	public void saveAlarms() throws IOException {
		File file = getStorageFile(true);

		ObjectMapper mapper = new ObjectMapper();
		mapper.writeValue(file, alarms);

		log.info("Alarms saved");

	}

	@Override
	public List<IEntityAlarm> getActiveEntityAlarms() {
		return alarms.stream().filter(IEntityAlarm::isActive).collect(Collectors.toList());
	}

	@Override
	public List<IEntityAlarm> getEntityAlarmsWithAlarmDetected(IDevice device) {
		return alarms.stream().filter(
				a -> a.isAlarmed() && a.getDevice().equals(device) && device.getEntities().contains(a.getEntity()))
				.collect(Collectors.toList());
	}

	@Override
	public List<IEntityAlarm> getActiveEntityAlarms(IDevice device) {
		return alarms.stream().filter(a -> a.getDevice().equals(device) && device.getEntities().contains(a.getEntity()))
				.collect(Collectors.toList());
	}

	@Override
	public IEntityAlarm getAlarm(String deviceName, String entityName) {
		return alarms.stream().filter(a -> a.getDeviceName().equals(deviceName) && a.getEntityName().equals(entityName))
				.findFirst().orElse(null);
	}

	@Override
	public IEntityAlarm getAlarm(IDevice device, IEntity entity) {
		return getAlarm(device.getName(), entity.getName());
	}

	@Override
	public void registerAlarm(IEntityAlarm alarm) {
		IEntityAlarm existing = getAlarm(alarm.getDeviceName(), alarm.getEntityName());

		if (existing != null) {
			alarms.remove(existing);
		}

		alarms.add(alarm);

		log.info("Alarm registered " + alarm.getDeviceName() + " e=" + alarm.getEntityName());
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	@Override
	public void activateAlarms(IDevice device) {
		device.getEntities().stream().forEach(e -> Optional.ofNullable(getAlarm(device, e)).ifPresent(al -> {
			al.setDevice(device);
			al.setEntity(e);

			al.getAlarms().forEach(a -> {
				IEntityField field = e.getField(a.getName());
				if (a.acceptsAsWatched(field)) {
					a.setWatchedItem(field);
				}
			});

			al.setSingleAlarmMessage(singleAlarmMessage);
			al.setTimer(new ActionTimer(1000 * alarmInterval));

			log.info("Alarm activated " + device.getName() + " e=" + e.getName());
		}));
	}

	@Override
	public void checkAlarms(IDevice device) {
		log.debug("check Alarms " + device.getDeviceName());
		getActiveEntityAlarms(device).stream().forEach(IEntityAlarm::check);
	}

	@Override
	public List<IEntityAlarm> getAlarmsWithAlarmNotificationRequired(IDevice device) {
		return getActiveEntityAlarms(device).stream().filter(IEntityAlarm::isAlarmStarted).collect(Collectors.toList());
	}

	@Override
	public List<IEntityAlarm> getAlarmsWithAlarmFinished(IDevice device) {
		return getActiveEntityAlarms(device).stream().filter(IEntityAlarm::isAlarmFinished)
				.collect(Collectors.toList());
	}

	@Override
	public void checkAlarmsSendNotifications(IDevice device) {
		checkAlarms(device);
		sendAlarmNotifications(device);
		sendAlarmFinishedNotifications(device);
	}

	private void sendAlarmNotifications(IDevice device) {
		List<IEntityAlarm> alarmsWithStarted = getAlarmsWithAlarmNotificationRequired(device);

		if (!alarmsWithStarted.isEmpty()) {
			
			log.debug("AlarmsStarted "+device.getDeviceName() + " total =" + alarmsWithStarted.size());

			StringBuilder buf = new StringBuilder(buildMessage(MSG_DEVICE_ALARM_STARTED, device));
			buf.append("\n\n");
			
			alarmsWithStarted.forEach( alarm -> buf.append( alarm.getAlarmStartedText()));
			
			boolean sent = true;
			
			try {
				sent = sender.sendMessageToAllUsers(Severity.WARN,  buf.toString()) && sent;
			} catch (Exception e) {
				log.error(e);
				sent = false;
			}
			
			for (IEntityAlarm alarm : alarmsWithStarted) {
				alarm.setAlarmStartedSent(sent);
			}
		
		}

	}

	private void sendAlarmFinishedNotifications(IDevice device) {
		List<IEntityAlarm> alarmsWithFinished = getAlarmsWithAlarmFinished(device);

		if (!alarmsWithFinished.isEmpty()) {
			log.debug("AlarmsFinished "+device.getDeviceName() + " total =" + alarmsWithFinished.size());

			StringBuilder buf = new StringBuilder(buildMessage(MSG_DEVICE_ALARM_FINISHED, device));
			buf.append("\n\n");
			
			alarmsWithFinished.forEach( alarm -> buf.append(alarm.getAlarmFinishedText()) );
			
			boolean sent = true;
			
			try {
				sent = sender.sendMessageToAllUsers(Severity.INFO,  buf.toString()) && sent;
			} catch (Exception e) {
				log.error(e);
				sent = false;
			}
			
			for (IEntityAlarm alarm : alarmsWithFinished) {
				alarm.setAlarmFinishedSent(sent);
			}
		}
	}

}
