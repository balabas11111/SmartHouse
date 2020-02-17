package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;

@Service
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
	}
	
	@Override
	public IAlarmStateChangeEventProcessor getAlarmStateChangedEventProcessor(IAlarmStateChangeEvent event) {
		return stateChangedProcessors.getOrDefault(event.getChangeAction().getTargetBeanName(), null);
	}

	@Override
	public AlarmV2Checker getAlarmV2checker(String name) {
		return alarmv2Checkers.getOrDefault(name, null);
	}

}
