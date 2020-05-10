package com.balabas.smarthouse.server.entity.alarmV2.service;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEntity;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.repository.AlarmStateChangeEntityRepository;
import com.balabas.smarthouse.server.util.DateTimeUtil;

public interface IAlarmStateChangeEntityService {

	AlarmStateChangeEntityRepository getAlarmStateChangeEntityRepository();
	
	void log(String string);
	
	default void saveAlarmStateChange(IItemEvent event) {
		if(event.getOldState()!=null && !event.getOldState().equals(event.getNewState())) {
			AlarmStateChangeEntity entity = buildEntity(event, event.getOldState(), null);
			getAlarmStateChangeEntityRepository().save(entity);
			
			entity = buildEntity(event, event.getNewState(), event.getDescription());
			getAlarmStateChangeEntityRepository().save(entity);
			
			log("State change saved old=" + event.getOldState() + " new=" + event.getNewState());
		}
	}
	
	default AlarmStateChangeEntity buildEntity(IItemEvent event, AlarmState alarmState, String description) {
		AlarmStateChangeEntity entity = new AlarmStateChangeEntity();
		
		entity.setDateTime(DateTimeUtil.getDate());
		entity.setAlarmState(alarmState);
		entity.setValueInt(alarmState.getValueInt());
		entity.setTargetAlarmId(event.getAlarm().getId());
		entity.setType(event.getItem().getItemType());
		
		return entity;
	}
	
	default List<AlarmStateChangeEntity> getAlarmStateChangeEntitiesForPeriodList(Long targetId, Date date1, Date date2) {
		return getAlarmStateChangeEntityRepository().getAlarmStateChangeEntitiesForPeriod(targetId, date1, date2);
	}
	
	default Map<AlarmState, List<AlarmStateChangeEntity>> getAlarmStateChangeEntitiesForPeriod(Long targetId, Date date1, Date date2) {
		Map<AlarmState, List<AlarmStateChangeEntity>> result = new HashMap<AlarmState, List<AlarmStateChangeEntity>>();
		
		for(AlarmStateChangeEntity entity : getAlarmStateChangeEntitiesForPeriodList(targetId, date1, date2)) {
			if(!result.containsKey(entity.getAlarmState())) {
				result.put(entity.getAlarmState(), new ArrayList<>());
			}
			
			result.get(entity.getAlarmState()).add(entity);
		}
		
		for(AlarmState state : AlarmState.getList()) {
			if(!AlarmState.ANY.equals(state)) {
				if(!result.containsKey(state)) {
					result.put(state, new ArrayList<>());
				}
			}
		}
		
		return result;
	}
}
