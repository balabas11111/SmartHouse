package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Service;

import lombok.Getter;

@Service
public class ItemAlarmStateService implements IItemAlarmStateService {

	@Getter
	private Map<String, Map<String, List<String>>> stateDescriptions = new HashMap<>();

}
