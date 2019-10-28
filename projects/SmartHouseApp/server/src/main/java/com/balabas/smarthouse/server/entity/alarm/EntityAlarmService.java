package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Severity;
import com.balabas.smarthouse.server.entity.service.IMessageSender;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceMessageConstants.buildMessage;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_STARTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_FINISHED;

@Service
@Log4j2
public class EntityAlarmService implements IEntityAlarmService, InitializingBean {

	@Getter
	@Value("${smarthouse.server.alarm.interval:30}")
	private Long alarmInterval;

	@Getter
	@Value("${smarthouse.server.alarm.singleAlarmMessage:true}")
	private boolean singleAlarmMessage;

	@Autowired
	private IMessageSender sender;

	List<IEntityAlarm> alarms = Collections.synchronizedList(new ArrayList<>());

	@Override
	public void afterPropertiesSet() throws Exception {
		log.info("Alarm service started");
	}

	@Override
	public void loadAlarms() throws IOException {
	}

	@Override
	public void saveAlarms() throws IOException {
		alarms.stream().forEach(this::save);
	}

	@Override
	public List<IEntityAlarm> getActiveEntityAlarms() {
		return alarms.stream().filter(IEntityAlarm::isActive).collect(Collectors.toList());
	}

	@Override
	public List<IEntityAlarm> getEntityAlarmsWithAlarmDetected(IDevice device) {
		return getAlarmsByPredicate(
				a -> a.isActive() && a.isAlarmed() && a.getDevice().getId().equals(device.getId()));
	}

	@Override
	public List<IEntityAlarm> getAlarms(IDevice device) {
		return getAlarmsByPredicate(a -> a.isActive() && a.getDevice().getId().equals(device.getId()));
	}

	@Override
	public IEntityAlarm getAlarm(IEntity entity) {
		return alarms.stream().filter(a -> a.isActive() && a.getEntity().getId().equals(entity.getId()))
				.findFirst().orElse(null);
	}

	@Override
	public void registerAlarm(IEntityAlarm alarm) {
		if(alarm.getEntity() == null) {
			throw new IllegalArgumentException("Empty entity");
		}
		
		IEntityAlarm existing = getAlarm(alarm.getEntity());

		if (existing != null) {
			alarms.remove(existing);
		}

		alarms.add(alarm);

		log.info("Alarm registered " + alarm.getEntity().getName() + alarm.getEntity().getGroup().getDevice().getName());
	}

	@Override
	public void activateAlarms(IDevice device) {
		device.getEntities().stream().forEach(e -> Optional.ofNullable(getAlarm(e)).ifPresent(al -> {

			al.setActivated(true);

			al.setSingleAlarmMessage(singleAlarmMessage);
			al.setTimer(new ActionTimer(1000 * alarmInterval));

			log.info("Alarm activated " + device.getName() + " e=" + e.getName());
		}));
	}

	@Override
	public void checkAlarms(IDevice device) {
		log.debug("check Alarms " + device.getName());
		getAlarms(device).stream().forEach(IEntityAlarm::check);
	}

	@Override
	public List<IEntityAlarm> getAlarmsWithAlarmNotificationRequired(IDevice device) {
		return getAlarms(device).stream().filter(IEntityAlarm::isAlarmStarted).collect(Collectors.toList());
	}

	@Override
	public List<IEntityAlarm> getAlarmsWithAlarmFinished(IDevice device) {
		return getAlarms(device).stream().filter(IEntityAlarm::isAlarmFinished)
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

			log.debug("AlarmsStarted " + device.getName() + " total =" + alarmsWithStarted.size());

			StringBuilder buf = new StringBuilder(buildMessage(MSG_DEVICE_ALARM_STARTED, device));
			buf.append("\n\n");

			alarmsWithStarted.forEach(alarm -> buf.append(alarm.getAlarmStartedText()));

			boolean sent = true;

			try {
				sent = sender.sendMessageToAllUsers(Severity.WARN, buf.toString()) && sent;
			} catch (Exception e) {
				log.error(e);
				sent = false;
			}

			for (IEntityAlarm alarm : alarmsWithStarted) {
				alarm.setAlarmStartedSent(sent);
			}

		}

	}
	
	@Override
	public IEntityAlarm save(IEntityAlarm alarm) {
		
		return alarm;
	}
	
	@Override
	public List<IEntityAlarm> loadAlarmsForDevice(IDevice device) {
		
		return null;
	}

	private void sendAlarmFinishedNotifications(IDevice device) {
		List<IEntityAlarm> alarmsWithFinished = getAlarmsWithAlarmFinished(device);

		if (!alarmsWithFinished.isEmpty()) {
			log.debug("AlarmsFinished " + device.getName() + " total =" + alarmsWithFinished.size());

			StringBuilder buf = new StringBuilder(buildMessage(MSG_DEVICE_ALARM_FINISHED, device));
			buf.append("\n\n");

			alarmsWithFinished.forEach(alarm -> buf.append(alarm.getAlarmFinishedText()));

			boolean sent = true;

			try {
				sent = sender.sendMessageToAllUsers(Severity.INFO, buf.toString()) && sent;
			} catch (Exception e) {
				log.error(e);
				sent = false;
			}

			for (IEntityAlarm alarm : alarmsWithFinished) {
				alarm.setAlarmFinishedSent(sent);
			}
		}
	}

	private List<IEntityAlarm> getAlarmsByPredicate(Predicate<? super IEntityAlarm> predicate) {
		return alarms.stream().filter(predicate).collect(Collectors.toList());
	}

}
