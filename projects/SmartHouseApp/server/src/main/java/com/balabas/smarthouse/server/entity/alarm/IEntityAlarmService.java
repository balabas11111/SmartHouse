package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.List;
import java.util.Map;

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

	List<IEntityAlarm> getAlarmsWithAlarmNotificationRequired(IDevice device);

	List<IEntityAlarm> getAlarmsWithAlarmFinished(IDevice device);

	List<IEntityAlarm> getActiveEntityAlarms();

	IEntityAlarm save(IEntityAlarm alarm);

	List<EntityAlarm> loadAlarmsForDevice(IDevice device);

	IEntityAlarm getAlarmById(Long alarmId);

	int getAlarmIndexById(Long alarmId);

	void reattachAlarms(IEntityAlarm alarm, Entity entity);

	Map<Integer, Class> getEnabledAlarmsForField(IEntityField entityField);

	List<IEntity> getEntitiesWithPossibleAlarms(IDevice device);

	List<IEntityField> getEntityFieldsWithPossibleAlarms(IEntity entity);

	IEntityFieldAlarm getEntityAlarmFieldById(Long entityFieldId);

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

}
