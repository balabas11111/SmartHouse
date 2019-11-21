package com.balabas.smarthouse.server.entity.behaviour;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IEntity;

@Service
public class EntityBehaviourService implements IEntityBehaviourService {
	
	@Autowired
	private Set<IEntityBehaviour> behaviours;
	
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
		
		return behaviour.orElseGet(null);
	}
	
	private void putBehaviourToCache(IEntity entity, IEntityBehaviour behaviour) {
		String key = entity.getEntityKey();
		
		if(!cache.containsKey(key) && behaviour!=null) {
			cache.put(key, behaviour);
		}
	}
	
}
