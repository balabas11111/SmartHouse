package com.balabas.smarthouse.server.entity.behaviour;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
@Service
public class EntityBehaviourService implements IEntityBehaviourService {
	
	@Autowired
	private Set<IEntityBehaviour> behaviours;
	
	@Autowired
	private IEntityAlarmService alarmService;
	
	private Map<String, IEntityBehaviour> cache = new HashMap<>();
	
	
	
	@Override
	public IEntityBehaviour getEntityBehaviour(IEntity entity) {
		return cache.getOrDefault(entity.getEntityKey(), null);
	}

	@Override
	public IEntityBehaviour cacheEntityBehaviourIfFound(IEntity entity) {
		if(behaviours == null) {
			return null;
		}
		Optional<IEntityBehaviour> behaviour = behaviours.stream().filter(b -> b.isTarget(entity)).findFirst(); 
		
		behaviour.ifPresent( b -> putBehaviourToCache(entity, b));
		
		return behaviour.orElse(null);
	}
	
	private void putBehaviourToCache(IEntity entity, IEntityBehaviour behaviour) {
		String key = entity.getEntityKey();
		
		if(!cache.containsKey(key) && behaviour!=null) {
			cache.put(key, behaviour);
		}
	}

	@Override
	public boolean isValueCorrect(IEntity entity, String value) {
		IEntityBehaviour behaviour = getEntityBehaviour(entity);
		
		if(behaviour == null) {
			return true;
		} else {
			return behaviour.isValueCorrect(value);
		}
		
	}

	@Override
	public boolean processReceivedValueForAlarm(IEntityField entityField, String receivedValue) {
		IEntityBehaviour behaviour = getEntityBehaviour(entityField.getEntity());
		
		if(behaviour == null) {
			return false;
		} else {
			behaviour.processReceivedValueForAlarm(alarmService, entityField, receivedValue);
			return true;
		}
	}
	
}
