package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

@Service
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2Repository alarmRepository;
	
	@Autowired
	IAlarmTypeProvider alarmTypeProvider;
	
	@Autowired
	IDeviceManageService deviceService;
	
	Map<String, List<IAlarmV2>> alarmMap = new HashMap<String, List<IAlarmV2>>();
	
	@Override
	public boolean checkForAlarm(IItemAbstract item) {
		int result = 0;
		
		return result != 0;
	}

	@Override
	public List<Class<?>> getEnabledAlarms(IItemAbstract item) {
		return alarmTypeProvider.getEnabledAlarms(item);
	}

	@Override
	public void saveAlarm(IAlarmV2 alarm) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void deleteAlarm(IAlarmV2 alarm) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean reattachAlarms(IItemAbstract item) {
		// TODO Auto-generated method stub
		return false;
	}
	
	private void putAlarm(IAlarmV2 alarm) {
		
	}
	
	private void removeAlarm(IAlarmV2 alarm) {
		
	}
}
