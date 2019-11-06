package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Optional;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;
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
import com.balabas.smarthouse.server.entity.repository.IEntityFieldAlarmRepository;
import com.balabas.smarthouse.server.entity.service.IMessageSender;
import com.balabas.smarthouse.server.service.ISoundPlayer;
import com.google.common.collect.Maps;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceMessageConstants.buildMessage;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_STARTED;
import static com.balabas.smarthouse.server.DeviceMessageConstants.MSG_DEVICE_ALARM_FINISHED;
import static com.balabas.smarthouse.server.entity.alarm.EntityAlarm.NO_MESSAGE_SEND_REPEATS;

@Service
@Log4j2
@SuppressWarnings({ "unchecked", "rawtypes" })
public class EntityAlarmService implements IEntityAlarmService {

	@Getter
	@Value("${smarthouse.server.log.alarmcheck:false}")
	private boolean logAlarmCheck;

	@Getter
	@Value("${smarthouse.server.alarm.interval:30}")
	private Integer defaultAlarmInterval;

	@Getter
	@Value("${smarthouse.server.alarm.singleAlarmMessage:true}")
	private boolean singleAlarmMessage;

	@Getter
	@Value("${smarthouse.server.alarm.sound.enabled:true}")
	private boolean soundEnabled;

	@Autowired
	private IMessageSender sender;

	@Autowired
	private IEntityAlarmRepository alarmRepository;

	@Autowired
	private IEntityFieldAlarmRepository alarmFieldRepository;

	@Autowired
	private ISoundPlayer soundPlayer;

	List<IEntityAlarm> alarms = Collections.synchronizedList(new ArrayList<>());

	Map<Integer, Class> entityFieldAllowedClasses = new HashMap<>();

	@PostConstruct
	public void loadAllowedClasses() {
		String scanPackage = this.getClass().getPackage().getName();
		int i = 0;

		ClassPathScanningCandidateComponentProvider provider = createComponentScanner(EntityFieldAlarm.class);
		for (BeanDefinition beanDef : provider.findCandidateComponents(scanPackage)) {
			try {
				Class clazz = Class.forName(beanDef.getBeanClassName());
				entityFieldAllowedClasses.put(Integer.valueOf(i), clazz);

				i++;
			} catch (ClassNotFoundException e) {
				log.error(e);
			}
		}

		log.info("total classes cached as allowed for entityField Alarm =" + (i - 1));
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
		return getAlarmsByPredicate(a -> a.isActive() && a.isAlarmed() && a.getDevice().getId().equals(device.getId()));
	}

	@Override
	public List<IEntityAlarm> getAlarms(IDevice device) {
		return getAlarmsByPredicate(a -> a.isActive() && a.getDevice().getId().equals(device.getId()));
	}

	@Override
	public IEntityAlarm getAlarm(IEntity entity) {
		return alarms.stream().filter(a -> a.getEntity().getId().equals(entity.getId())).findFirst().orElse(null);
	}

	@Override
	public IEntityAlarm getAlarmActive(IEntity entity) {
		return alarms.stream().filter(a -> a.isActive() && a.getEntity().getId().equals(entity.getId())).findFirst()
				.orElse(null);
	}

	@Override
	public IEntityAlarm getAlarmById(Long alarmId) {
		return alarms.stream().filter(a -> a.getId().equals(alarmId)).findFirst().orElse(null);
	}

	@Override
	public int getAlarmIndexById(Long alarmId) {

		if (alarms.size() > 0) {
			for (int i = 0; i < alarms.size(); i++) {
				IEntityAlarm ea = alarms.get(i);

				if (ea.getId().equals(alarmId)) {
					return i;
				}
			}
		}
		return -1;
	}

	@Override
	@Transactional
	public List<EntityAlarm> loadAlarmsForDevice(IDevice device) {
		List<EntityAlarm> als = alarmRepository.findAlarmsForDevice(device.getId());

		als.stream().filter(al -> getAlarmIndexById(al.getId()) == -1).forEach(alarms::add);

		if (als.size() > 0) {
			log.info("Device Alarms loaded " + device.getName());
		}
		return als;
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

		reattachAlarms(save(alarm), (Entity) entity);
	}

	@Override
	public void reattachAlarms(IDevice device) {
		if (device.isInitialized()) {
			device.getEntities().stream()
					.forEach(e -> Optional.ofNullable(getAlarm(e)).ifPresent(a -> reattachAlarms(a, e)));
		}
	}

	@Override
	public void reattachAlarms(IEntityAlarm alarm, Entity entity) {
		int messageInterval = alarm.getId() != null ? alarm.getMessageInterval() : defaultAlarmInterval;

		alarm.setEntity(entity);
		alarm.setLogAlarmCheck(logAlarmCheck);

		if (alarm.getTimer() == null) {
			alarm.setMessageInterval(messageInterval);
		}
		if (alarm.getAlarms() != null) {
			for (IEntityFieldAlarm efa : alarm.getAlarms()) {
				IEntityField ef = entity.getEntityField(efa.getWatchedItem().getId());

				efa.setWatchedItem(ef);
				efa.setActivated(true);
			}
		}

		// alarm.setActivated(true);

		log.info("Alarm reattached d=" + alarm.getEntity().getGroup().getDevice().getName() + " e="
				+ alarm.getEntity().getName());

		checkWithClear(alarm);
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
				if (soundEnabled && alarm.isSound()) {
					soundPlayer.playAlarmStarted();
				}
				alarm.setAlarmStartedSent(sent);
			}

		}
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
				if (soundEnabled && alarm.isSound()) {
					soundPlayer.playAlarmFinished();
				}
				alarm.setAlarmFinishedSent(sent);
			}
		}
	}

	@Override
	public IEntityAlarm save(IEntityAlarm alarm) {
		ActionTimer timer = alarm.getTimer();
		boolean sendAlarmStartedMessage = alarm.isSendAlarmStartedMessage();
		boolean sendAlarmFinishedMessage = alarm.isSendAlarmFinishedMessage();

		IEntityAlarm al = alarm;

		if (alarm.getAlarms() != null) {
			alarm.getAlarms().stream().forEach(efa -> efa.setEntityAlarm(al));
		}

		alarm = alarmRepository.save((EntityAlarm) alarm);

		alarm.setTimer(timer);
		alarm.setSendAlarmStartedMessage(sendAlarmStartedMessage);
		alarm.setSendAlarmFinishedMessage(sendAlarmFinishedMessage);

		int index = getAlarmIndexById(alarm.getId());

		if (index == -1) {
			alarms.add(alarm);
		} else {
			alarms.set(index, alarm);
		}

		log.info("EntityAlarm saved");

		return alarm;
	}

	@Override
	public IEntityAlarm load(Long id) {
		IEntityAlarm alarm = alarmRepository.findAlarmById(id);

		int index = getAlarmIndexById(alarm.getId());

		if (index == -1) {
			alarms.add(alarm);
		} else {
			ActionTimer timer = alarms.get(index).getTimer();
			alarm.setTimer(timer);
			alarms.set(index, alarm);
		}

		log.info("EntityAlarm loaded");

		return alarm;
	}

	@Override
	public Map<Integer, Class> getEnabledAlarmsForField(IEntityField entityField) {
		Map<Integer, Class> result = Maps.newHashMap();

		for (Entry<Integer, Class> entity : this.entityFieldAllowedClasses.entrySet()) {
			EntityFieldAlarm classNameAnnotation = AnnotationUtils.findAnnotation(entity.getValue(),
					EntityFieldAlarm.class);

			if (classNameAnnotation.target().isAssignableFrom(entityField.getClazz())) {
				result.put(entity.getKey(), entity.getValue());
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

	private boolean entityHasPossibleAlarms(IEntity entity) {
		for (IEntityField entityField : entity.getEntityFields()) {
			if (entityFieldHasPossibleAlarms(entityField)) {
				return true;
			}
		}
		return false;
	}

	private boolean entityFieldHasPossibleAlarms(IEntityField entityField) {
		return !DeviceConstants.ENTITY_FIELD_ID.equals(entityField.getName()) && entityField.isActive()
				&& !entityField.isCalculated() && getEnabledAlarmsForField(entityField).size() > 0;
	}

	private ClassPathScanningCandidateComponentProvider createComponentScanner(Class clazz) {
		ClassPathScanningCandidateComponentProvider provider = new ClassPathScanningCandidateComponentProvider(false);
		provider.addIncludeFilter(new AnnotationTypeFilter(clazz));
		return provider;
	}

	private List<IEntityAlarm> getAlarmsByPredicate(Predicate<? super IEntityAlarm> predicate) {
		return alarms.stream().filter(predicate).collect(Collectors.toList());
	}

	@Override
	public IEntityFieldAlarm getEntityAlarmFieldById(Long targetId) {
		return alarms.stream().flatMap(a -> a.getAlarms().stream()).filter(efa -> efa.getId().equals(targetId))
				.findFirst().orElse(null);
	}

	@Override
	public void changeEntityAlarmActivation(Long entityAlarmId) {
		IEntityAlarm entityAlarm = getAlarmById(entityAlarmId);
		boolean activated = !entityAlarm.isActivated();

		entityAlarm.setActivated(activated);

		save(entityAlarm);

		if (activated) {
			/*
			 * entityAlarm.setSendAlarmStartedMessage(false);
			 * entityAlarm.setSendAlarmFinishedMessage(false);
			 * 
			 * entityAlarm.getTimer().setActionForced(false);
			 */
			entityAlarm.check();
		}
	}

	@Override
	public void updateEntityAlarmMessageInterval(Integer messageInterval, Long entityAlarmId) {
		IEntityAlarm entityAlarm = getAlarmById(entityAlarmId);
		entityAlarm.setMessageInterval(messageInterval);
		save(entityAlarm);

	}

	@Override
	public void checkWithClear(IEntityAlarm entityAlarm) {
		entityAlarm.setSendAlarmStartedMessage(false);
		entityAlarm.setSendAlarmFinishedMessage(false);

		entityAlarm.getTimer().setActionForced(false);

		entityAlarm.check();
	}

	@Override
	public void changeEntityAlarmSound(Long entityAlarmId) {
		IEntityAlarm entityAlarm = getAlarmById(entityAlarmId);
		entityAlarm.setSound(!entityAlarm.isSound());

		save(entityAlarm);
	}

	@Override
	public void removeMessageIntervalOnEntityAlarm(Long entityAlarmId) {
		IEntityAlarm entityAlarm = getAlarmById(entityAlarmId);
		entityAlarm.setMessageInterval(NO_MESSAGE_SEND_REPEATS);
		save(entityAlarm);
	}

	@Override
	@Transactional
	public void removeEntityFieldAlarm(Long entityFieldAlarmId) {
		IEntityFieldAlarm entityFieldAlarm = getEntityAlarmFieldById(entityFieldAlarmId);
		IEntityAlarm entityAlarm = entityFieldAlarm.getEntityAlarm();

		alarmFieldRepository.deleteNativeById(entityFieldAlarmId);
		// alarmFieldRepository.deleteById(entityFieldAlarmId);

		load(entityAlarm.getId());
		log.info("Entity field alarm deleted id=" + entityFieldAlarmId);
	}

	@Override
	public Class getEntityFieldAllowedClassByIndex(Integer alarmClassIndex) {
		return entityFieldAllowedClasses.getOrDefault(alarmClassIndex, null);
	}

	@Override
	public void createNewEntityFieldAlarmInEntityAlarm(String newAlarmClassIndex, String value,
			IEntityField entityField) throws InstantiationException, IllegalAccessException, ClassNotFoundException {
		IEntity entity = entityField.getEntity();

		IEntityAlarm entityAlarm = getAlarm(entity);

		IEntityFieldAlarm entityFieldAlarm = (IEntityFieldAlarm) getEntityFieldAllowedClassByIndex(
				Integer.valueOf(newAlarmClassIndex)).newInstance();
		entityFieldAlarm.setWatchedItem(entityField);
		entityFieldAlarm.setValueStr(value);

		entityAlarm.putAlarm(entityFieldAlarm);

		save(entityAlarm);
	}

	@Override
	public void updateAlarmValueOfEntityAlarm(String val, Long entityFieldAlarmId) {
		IEntityFieldAlarm entityFieldAlarm = getEntityAlarmFieldById(entityFieldAlarmId);

		entityFieldAlarm.setValueStr(val);
		IEntityAlarm entityAlarm = entityFieldAlarm.getEntityAlarm();

		save(entityAlarm);
	}

	@Override
	public void createNewEntityAlarm(IEntity entity) {
		save(new EntityAlarm(entity));
	}

	@Transactional
	@Override
	public void deleteAlarmsByDeviceId(Long deviceId) {
		alarmFieldRepository.deleteEntityFieldAlarmByDeviceId(deviceId);
		alarmRepository.deleteEntityAlarmsByDeviceId(deviceId);
	}

}
