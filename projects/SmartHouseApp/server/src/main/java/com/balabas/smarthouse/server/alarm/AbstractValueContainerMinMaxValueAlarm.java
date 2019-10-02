package com.balabas.smarthouse.server.alarm;

import java.util.HashMap;
import java.util.Map;

import com.balabas.smarthouse.server.model.ValueContainer;

public abstract class AbstractValueContainerMinMaxValueAlarm<T extends ValueContainer> extends AbstractAlarm<T>  {

	private Map<String, String> minValues = new HashMap<>();
	
	private Map<String, String> maxValues = new HashMap<>();
	
	public void removeMinValue(String key) {
		minValues.remove(key);
	}
	
	public void removeMaxValue(String key) {
		maxValues.remove(key);
	}
	
	public boolean putMinValue(String key, String value) {
		boolean ok = value!=null && item!=null && item.hasKey(key);
		
		if(ok) {
			minValues.put(key, value);
		}
		
		return ok;
	}
	
	public boolean putMaxValue(String key, String value) {
		boolean ok = value!=null && item!=null && item.hasKey(key);
		
		if(ok) {
			maxValues.put(key, value);
		}
		
		return ok;
	}
	
}
