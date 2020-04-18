package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class Alarmv2TypeProvider implements IAlarmv2TypeProvider {

	@Getter
	@Autowired(required = false)
	Map<String, AlarmV2Checker> alarmv2Checkers;
	
	@Getter
	@Autowired(required = false)
	Map<String, IAlarmStateChangeEventProcessor> stateChangedProcessors;
	
	@Override
	public List<AlarmV2Checker> getEnabledAlarms(IItemAbstract item) {
		return alarmv2Checkers.values().stream().filter(ch -> ch.isCorrectTarget(item)).collect(Collectors.toList());
	}
	
	@PostConstruct
	public void loadAllowedClasses() {
		log.info("total AlarmV2Checker " + alarmv2Checkers.size());
		log.info(alarmv2Checkers.keySet().stream().map(key -> key + ";").collect(Collectors.joining(", ", "{", "}")));
		log.info("total IAlarmStateChangeEventProcessor " + stateChangedProcessors.size());
		log.info(stateChangedProcessors.keySet().stream().map(key -> key + ";").collect(Collectors.joining(", ", "{", "}")));
	}
	
	@Override
	public IAlarmStateChangeEventProcessor getAlarmStateChangedEventProcessor(IItemEvent event) {
		String targetBeanName = event.getChangeAction().getTargetBeanName();
		
		IAlarmStateChangeEventProcessor result = getFromMapByKeyIgnoreCase(stateChangedProcessors, targetBeanName);
		
		return result; 
	}

	@Override
	public AlarmV2Checker getAlarmV2checker(String name) {
		return getFromMapByKeyIgnoreCase(alarmv2Checkers, name);
	}
	
	private <T> T getFromMapByKeyIgnoreCase(Map<String, T> map, String key) {
		T result = map.getOrDefault(key, null);
		
		if(result == null) {
			for(String keyMap : map.keySet()) {
				if(keyMap.equalsIgnoreCase(key)) {
					result =  map.getOrDefault(keyMap, null);
				}
			}
		}
		
		if(result == null) {
			System.out.println("Item not found");
		}
		return result;
	}

	@Override
	public AlarmV2Checker getAlarmChecker(IAlarmV2 alarm) {
		return getAlarmV2checker(alarm.getCheckerName());
	}

	@Override
	public List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass) {
		return alarmv2Checkers.values().stream().filter(ch -> ch.isCorrectTarget(targetItemClass)).collect(Collectors.toList());
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public Set<IAlarmStateChangeEventProcessor> getAlarmStateChangedEventProcessors() {
		return new LinkedHashSet(stateChangedProcessors.values());
	}

}
