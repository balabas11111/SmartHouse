package com.balabas.smarthouse.server.alarm;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.balabas.smarthouse.server.model.ValueContainer;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;

public class AbstractAlarmValueContainerMinMaxValue<T extends ValueContainer> extends AbstractAlarm<T, Float> {

	@Getter
	private Map<String, Float> minValues = new HashMap<>();

	@Getter
	private Map<String, Float> maxValues = new HashMap<>();

	public void removeMinValue(String key) {
		minValues.remove(key);
	}

	public void removeMaxValue(String key) {
		maxValues.remove(key);
	}

	public boolean putMinValue(String key, Float value) {
		return putValueMap(key, value, minValues);
	}

	public boolean putMaxValue(String key, Float value) {
		return putValueMap(key, value, maxValues);
	}

	private boolean putValueMap(String key, Float value, Map<String, Float> vals) {
		if (!checkPutValue(key, value)) {
			return false;
		}
		vals.put(key, value);
		return true;
	}

	@Override
	protected boolean doCheckItem() {
		boolean passed = maxValues.entrySet().stream().anyMatch(e -> executeItemKeyCheck(e, true));

		passed = minValues.entrySet().stream().anyMatch(e -> executeItemKeyCheck(e, false)) || passed;

		return passed;
	}

	private boolean executeItemKeyCheck(Entry<String, Float> alarmEntry, boolean itemGreaterThanAlarm) {
		boolean result = false;

		try {
			String itemValueStr = item.getValue(alarmEntry.getKey());
			Float itemValue = doCast(itemValueStr);

			int res = Float.compare(itemValue, alarmEntry.getValue());

			res = (itemGreaterThanAlarm) ? res : res * -1;

			result = res >= 0;

			if (result) {
				String compareStr = (itemGreaterThanAlarm) ? ">" : "<";
				String message = String.format("поле %s значение %s %s %s", alarmEntry.getKey(), itemValueStr,
						compareStr, Float.toString(alarmEntry.getValue()));
				putErrorMessage(message);
			}

		} catch (Exception e) {
		}

		return result;
	}

	@Override
	protected Float doCast(String value) throws Exception {
		return Float.valueOf(value);
	}

	@Override
	@JsonIgnore
	public String getMessageView() {
		StringBuilder buf = new StringBuilder();
		
		buf.append(item.getDescription());
		buf.append(" : ");
		maxValues.entrySet().stream()
			.map(entry -> entry.getKey() + " > "+entry.getValue() + " ; ")
			.forEach(str -> buf.append(str));
		
		return buf.toString();
	}

}
