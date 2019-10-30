package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityAlarmService {

	List<IEntityAlarm> getEntityAlarmsWithAlarmDetected(IDevice device);

	IEntityAlarm getAlarm(IEntity entity);
	List<IEntityAlarm> getAlarms(IDevice device); 
	
	void registerAlarm(IEntityAlarm alarm);

	void saveAlarms() throws IOException;
	
	void activateAlarms(IDevice device);

	void checkAlarms(IDevice device);
	
	void checkAlarmsSendNotifications(IDevice device);

	List<IEntityAlarm> getAlarmsWithAlarmNotificationRequired(IDevice device);

	List<IEntityAlarm> getAlarmsWithAlarmFinished(IDevice device);

	List<IEntityAlarm> getActiveEntityAlarms();

	IEntityAlarm save(IEntityAlarm alarm);

	List<EntityAlarm> loadAlarmsForDevice(IDevice device);

	IEntityAlarm getAlarmById(Long alarmId);

	int getAlarmIndexById(Long alarmId);

	void activateAlarm(IEntityAlarm alarm, Entity entity);

	List<Class> getEnabledAlarmsForField(IEntityField entityField);

	List<IEntity> getEntitiesWithPossibleAlarms(IDevice device);

	List<IEntityField> getEntityFieldsWithPossibleAlarms(IEntity entity);

	IEntityFieldAlarm getEntityAlarmFieldById(Long entityFieldId);

	void changeEntityAlarmActivation(Long entityAlarmId);

	void removeMessageIntervalOnEntityAlarm(Long entityAlarmId);

	void removeEntityFieldAlarm(Long entityFieldAlarmId);

	void createNewEntityFieldAlarmInEntityAlarm(String newAlarmClassName, String value, Long entityAlarmId) throws InstantiationException, IllegalAccessException, ClassNotFoundException;

	void updateAlarmValueOfEntityAlarm(String val, Long entityFieldAlarmId);

	void createNewEntityAlarm(IEntity entity);

	IEntityAlarm getAlarmActive(IEntity entity);

}
