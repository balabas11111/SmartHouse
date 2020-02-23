package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IDevice;

@Component
public class AlarmV2CheckerDeviceDataIsTooOld implements AlarmV2Checker {

	public Class<?> getItemClass() {
		return IDevice.class;
	}

	@Override
	public boolean check(IAlarmV2 alarm) {
		String param = alarm.getParameter();
		
		if(!StringUtils.isEmpty(param)) {
			Integer oldSeconds = Integer.valueOf(param);
			
			IDevice device = (IDevice)alarm.getItem();
			
			if(oldSeconds!=null && oldSeconds > 1) {
				long lastUpdated = device.getLastUpdated();
				
				long diff = (Math.abs((new Date()).getTime() - lastUpdated))/1000;
				
				if(lastUpdated!=0 && diff > oldSeconds) {
					alarm.setAlarmState(AlarmState.ALARM);
					return true;
				} else {
					alarm.setAlarmState(AlarmState.OK);
					return true;
				}
			}
		} else {
			alarm.setAlarmState(AlarmState.OK);
		}
		
		return false;
	}

	@Override
	public String getCheckerDescription() {
		return "Checks if device data is too old";
	}
	
}
