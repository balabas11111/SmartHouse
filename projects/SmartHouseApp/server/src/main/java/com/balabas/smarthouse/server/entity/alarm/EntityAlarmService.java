package com.balabas.smarthouse.server.entity.alarm;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class EntityAlarmService implements IEntityAlarmService, InitializingBean {

	@Getter
	@Value("${smarthouse.server.repository.path}")
	private String storagePath = "repository/alarm/";

	@Getter
	@Value("${smarthouse.server.alarm.interval:30}")
	private Long alarmInterval;

	List<IEntityAlarm> alarms;

	@Override
	public void afterPropertiesSet() throws Exception {
		loadAlarms();
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
	public IEntityAlarm getAlarm(IDevice device, IEntity entity) {
		return alarms.stream().filter(a -> a.getDevice().getName().equals(device.getName())
				&& a.getEntity().getName().equals(entity.getName())).findFirst().orElse(null);
	}

	@Override
	public void registerAlarm(IEntityAlarm alarm) {
		IEntityAlarm existing = getAlarm(alarm.getDevice(), alarm.getEntity());

		if (existing != null) {
			alarms.remove(existing);
		}

		alarms.add(alarm);
	}

	@Override
	public void activateAlarms(IDevice device) {
		device.getEntities().stream().forEach(e -> {
			IEntityAlarm alarm = getAlarm(device, e);
			alarm.setDevice(device);
			alarm.setEntity(e);
			
			alarm.setTimer(new ActionTimer(1000 * alarmInterval));
		});
	}

	@Override
	public void checkAlarms(IDevice device) {
		log.info("check Alarms");
		getActiveEntityAlarms(device).stream().forEach(IEntityAlarm::check);
	}

	@Override
	public List<IEntityAlarm> getAlarmsWithNotificationRequired(IDevice device) {
		return getActiveEntityAlarms(device).stream().filter(IEntityAlarm::notificationRequired)
				.collect(Collectors.toList());
	}

}
