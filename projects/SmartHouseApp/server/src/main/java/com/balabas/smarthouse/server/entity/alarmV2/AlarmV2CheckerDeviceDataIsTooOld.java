package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.util.DateTimeUtil;

import lombok.Getter;

@Component
public class AlarmV2CheckerDeviceDataIsTooOld extends AlarmV2CheckerAbstractDevice implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_DEVICE_DISCONNECTED;

	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		boolean result = true;

		String param = alarm.getParameter();

		if (!StringUtils.isEmpty(param)) {
			Integer oldSeconds = Integer.valueOf(param);

			if (oldSeconds != null && oldSeconds > 1) {
				long lastUpdated = getItemAsDevice(alarm).getLastUpdated();

				long diff = (Math.abs(DateTimeUtil.now() - lastUpdated)) / 1000;

				if (lastUpdated != 0 && diff > oldSeconds) {
					result = false;
				}
			}
		}

		return result;
	}

}
