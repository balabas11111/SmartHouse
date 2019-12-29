package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.DeviceEntityAlarmHolder;

@SuppressWarnings("rawtypes")
public interface IEntityAlarmService {

	List<IEntityAlarm> getAlarms(IDevice device);
	List<IEntityAlarm> getEntityAlarmsWithAlarmDetected(IDevice device);
	List<IEntityAlarm> getEntityAlarmsWithFieldsAlarmsAttached(IDevice device);

	IEntityAlarm getAlarm(IEntity entity);
	 
	
	void registerAlarm(IEntityAlarm alarm);

	void saveAlarms() throws IOException;
	
	void reattachAlarms(IDevice device);

	void checkAlarms(IDevice device);
	
	void checkAlarmsSendNotifications(IDevice device);

	List<IAlarmRepeatable> getAlarmsWithAlarmNotificationRequired(IDevice device);

	List<IAlarmRepeatable> getAlarmsWithAlarmFinished(IDevice device);

	List<IEntityAlarm> getActiveEntityAlarms();

	IEntityAlarm save(IEntityAlarm alarm);

	List<EntityAlarm> loadAlarmsForDevice(IDevice device);

	IEntityAlarm getAlarmById(Long alarmId);

	int getAlarmIndexById(Long alarmId);

	void reattachAlarms(IEntityAlarm alarm, Entity entity);

	Map<Integer, Class> getEnabledAlarmsForField(IEntityField entityField);

	List<IEntity> getEntitiesWithPossibleAlarms(IDevice device);

	List<IEntityField> getEntityFieldsWithPossibleAlarms(IEntity entity);

	List<IEntityFieldAlarm> getEntityAlarmFieldById(Long entityFieldId);
	
	IEntityFieldAlarm getEntityAlarmFieldById(Long entityFieldId, Class<?> entityFieldAlarmClass);

	void changeEntityAlarmActivation(Long entityAlarmId);

	void removeMessageIntervalOnEntityAlarm(Long entityAlarmId);

	void removeEntityFieldAlarm(Long entityFieldAlarmId);

	void createNewEntityFieldAlarmInEntityAlarm(String newAlarmClassIndex, String value, IEntityField entityField) throws InstantiationException, IllegalAccessException, ClassNotFoundException;

	void updateAlarmValueOfEntityAlarm(String val, Long entityFieldAlarmId);

	void createNewEntityAlarm(IEntity entity);

	IEntityAlarm getAlarmActive(IEntity entity);

	void updateEntityAlarmMessageInterval(Integer messageInterval, Long entityAlarmId);

	IEntityAlarm load(Long id);

	Class getEntityFieldAllowedClassByIndex(Integer alarmClassIndex);

	void changeEntityAlarmSound(Long entityAlarmId);

	void checkWithClear(IEntityAlarm entityAlarm);
	
	void deleteAlarmsByDeviceId(Long deviceId);
	
	DeviceEntityAlarmHolder getDeviceAlarmsHolder(IDevice device);
	Map<String, DeviceEntityAlarmHolder> getDeviceAlarmsHoldersGroupped(List<Device> device);
	
	void createNewEntityFieldAlarmOrUpdateValueTmp(IEntityField entityField,
			String receivedValue, Class<?> entityFieldClass) throws InstantiationException, IllegalAccessException;
	
	//void createNewEntityFieldAlarmInEntityAlarm(IEntityAlarm entityAlarm, IEntityFieldAlarm entityFieldAlarm, IEntityField entityField, String value);
	
	void createNewEntityFieldAlarmInEntityAlarm(IEntityAlarm entityAlarm, Class<?> entityFieldAlarmClass,
			IEntityField entityField, String value) throws InstantiationException, IllegalAccessException;
	
	List<IEntityFieldAlarm> getEntityAlarmsWithAlarmDetected(Set<IEntityField> entityFields);

}
