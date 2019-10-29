package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import javax.transaction.Transactional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.context.annotation.ClassPathScanningCandidateComponentProvider;
import org.springframework.core.annotation.AnnotationUtils;
import org.springframework.core.type.filter.AnnotationTypeFilter;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Severity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IEntityAlarmRepository;
import com.balabas.smarthouse.server.entity.service.IMessageSender;
import com.google.common.collect.Lists;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceMessageConstants.buildMessage;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_STARTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_FINISHED;

@Service
@Log4j2
@SuppressWarnings({ "unchecked", "rawtypes" })
public class EntityAlarmService implements IEntityAlarmService {

	@Getter
	@Value("${smarthouse.server.alarm.interval:30}")
	private Integer defaultAlarmInterval;

	@Getter
	@Value("${smarthouse.server.alarm.singleAlarmMessage:true}")
	private boolean singleAlarmMessage;

	@Autowired
	private IMessageSender sender;

	@Autowired
	private IEntityAlarmRepository alarmRepository;

	List<IEntityAlarm> alarms = Collections.synchronizedList(new ArrayList<>());

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
		return getAlarmsByPredicate(a -> a.isActive() && a.isAlarmed() && a.getDevice().getId().equals(device.getId()));
	}

	@Override
	public List<IEntityAlarm> getAlarms(IDevice device) {
		return getAlarmsByPredicate(a -> a.isActive() && a.getDevice().getId().equals(device.getId()));
	}

	@Override
	public IEntityAlarm getAlarm(IEntity entity) {
		return alarms.stream().filter(a -> a.isActive() && a.getEntity().getId().equals(entity.getId())).findFirst()
				.orElse(null);
	}

	@Override
	public IEntityAlarm getAlarmById(Long alarmId) {
		return alarms.stream().filter(a -> a.getId().equals(alarmId)).findFirst().orElse(null);
	}

	@Override
	public int getAlarmIndexById(Long alarmId) {
		int result = -1;

		for (IEntityAlarm ea : this.alarms) {
			result++;
			if (ea.getId().equals(alarmId)) {
				break;
			}
		}
		return result;
	}

	@Override
	public void registerAlarm(IEntityAlarm alarm) {
		IEntity entity = alarm.getEntity();

		if (entity == null) {
			throw new IllegalArgumentException("Empty entity");
		}

		IEntityAlarm existing = getAlarm(entity);

		if (existing == null) {
			log.info("Alarm registered " + entity.getName() + " d=" + entity.getGroup().getDevice().getName());
		} else {
			Optional.ofNullable(alarm.getAlarms()).orElse(Collections.emptyList()).stream().forEach(existing::putAlarm);

			log.warn("Alarm exists " + entity.getName() + " d=" + entity.getGroup().getDevice().getName());
		}

		activateAlarm(save(alarm), (Entity) entity);
	}

	@Override
	public void activateAlarms(IDevice device) {
		if (device.isInitialized()) {
			device.getEntities().stream()
					.forEach(e -> Optional.ofNullable(getAlarm(e)).ifPresent(a -> activateAlarm(a, e)));
		}
	}

	@Override
	public void activateAlarm(IEntityAlarm alarm, Entity entity) {
		int messageInterval = alarm.getId() != null ? alarm.getMessageInterval() : defaultAlarmInterval;

		alarm.setEntity(entity);

		if (alarm.getTimer() == null) {
			alarm.setMessageInterval(messageInterval);
			alarm.setTimer(new ActionTimer(1000 * messageInterval));
		}
		if (alarm.getAlarms() != null) {
			for (IEntityFieldAlarm efa : alarm.getAlarms()) {
				IEntityField ef = entity.getEntityField(efa.getWatchedItem().getId());

				efa.setWatchedItem(ef);
				efa.setActivated(true);
			}
		}

		alarm.setActivated(true);

		log.info("Alarm activated d=" + alarm.getEntity().getGroup().getDevice().getName() + " e="
				+ alarm.getEntity().getName());
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
		return getAlarms(device).stream().filter(IEntityAlarm::isAlarmFinished).collect(Collectors.toList());
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
		ActionTimer timer = alarm.getTimer();
		boolean activated = alarm.isActivated();

		IEntityAlarm al = alarm;

		if (alarm.getAlarms() != null) {
			alarm.getAlarms().stream().forEach(efa -> efa.setEntityAlarm(al));
		}

		alarm = alarmRepository.save((EntityAlarm) alarm);
		alarm.setTimer(timer);
		alarm.setActivated(activated);

		int index = getAlarmIndexById(alarm.getId());

		if (index == -1) {
			alarms.add(alarm);
		} else {
			alarms.set(index, alarm);
		}

		return alarm;
	}

	@Override
	@Transactional
	public List<EntityAlarm> loadAlarmsForDevice(IDevice device) {
		List<EntityAlarm> als = alarmRepository.findAlarmsForDevice(device.getId());

		als.stream().filter(al -> getAlarmIndexById(al.getId()) == -1).forEach(alarms::add);

		return als;
	}

	@Override
	public List<Class> getEnabledAlarmsForField(IEntityField entityField) {
		String scanPackage = this.getClass().getPackage().getName();
		// String scanPackage = "com.balabas.smarthouse.server.entity.alarm";
		List<Class> result = Lists.newArrayList();

		ClassPathScanningCandidateComponentProvider provider = createComponentScanner(EntityFieldAlarm.class);
		for (BeanDefinition beanDef : provider.findCandidateComponents(scanPackage)) {
			try {
				Class clazz = Class.forName(beanDef.getBeanClassName());
				EntityFieldAlarm classNameAnnotation = AnnotationUtils.findAnnotation(clazz, EntityFieldAlarm.class);

				if (classNameAnnotation.target().isAssignableFrom(entityField.getClazz())) {
					result.add(clazz);
				}
			} catch (ClassNotFoundException e) {
				log.error(e);
			}
		}
		return result;
	}

	@Override
	public List<IEntity> getEntitiesWithPossibleAlarms(IDevice device) {
		return device.getEntities().stream().filter(this::entityHasPossibleAlarms).collect(Collectors.toList());
	}

	@Override
	public List<IEntityField> getEntityFieldsWithPossibleAlarms(IEntity entity) {
		return entity.getEntityFields().stream().filter(this::entityFieldHasPossibleAlarms)
				.collect(Collectors.toList());
	}

	@Override
	public List<IEntityFieldAlarm> getEntityFieldAlarms(IEntityField entityField) {
		// TODO Auto-generated method stub
		return null;
	}

	private boolean entityHasPossibleAlarms(IEntity entity) {
		for (IEntityField entityField : entity.getEntityFields()) {
			if (entityFieldHasPossibleAlarms(entityField)) {
				return true;
			}
		}
		return false;
	}

	private boolean entityFieldHasPossibleAlarms(IEntityField entityField) {
		return !DeviceConstants.ENTITY_FIELD_ID.equals(entityField.getName())
				&& entityField.isActive()
				&& !entityField.isCalculated()
				&& getEnabledAlarmsForField(entityField).size() > 0;
	}

	private ClassPathScanningCandidateComponentProvider createComponentScanner(Class clazz) {
		ClassPathScanningCandidateComponentProvider provider = new ClassPathScanningCandidateComponentProvider(false);
		provider.addIncludeFilter(new AnnotationTypeFilter(clazz));
		return provider;
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
