package com.balabas.smarthouse.server.entity.alarmV2.checker;

import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractDevice;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;
import com.balabas.smarthouse.server.util.DateTimeUtil;

import lombok.Getter;

@Component
public class AlarmV2CheckerDeviceDataIsTooOld extends AlarmV2CheckerAbstractDevice implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_DEVICE_DISCONNECTED;
	
	private final Long DEFAULT_OLD_MILLISECONDS = 10000L;

	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		boolean result = false;

		String param = alarm.getParameter();

		Long oldSeconds = DEFAULT_OLD_MILLISECONDS; 
		
		if (!StringUtils.isEmpty(param)) {
			oldSeconds = Long.valueOf(param);
		}
		
		if (oldSeconds != null && oldSeconds > 1) {
			long lastUpdated = getItemAsDevice(alarm).getLastUpdated();

			long diff = (Math.abs(DateTimeUtil.now() - lastUpdated)) / 1000;

			if (lastUpdated > 10000 && diff > oldSeconds) {
				result = true;
			}
		}

		return result;
	}

}
