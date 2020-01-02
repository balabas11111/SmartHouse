package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@Service
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2Repository alarmRepository;
	
	@Autowired
	IAlarmTypeProvider alarmTypeProvider;
	
	@Override
	public boolean checkForAlarm(IItemAbstract item) {
		// TODO Auto-generated method stub
		return false;
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
}
