package com.balabas.smarthouse.server.alarm;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.balabas.smarthouse.server.model.ValueContainer;

public class AbstractAlarmValueContainerMinMaxValue<T extends ValueContainer> extends AbstractAlarm<T, Float>  {

	private Map<String, Float> minValues = new HashMap<>();
	
	private Map<String, Float> maxValues = new HashMap<>();
	
	public void removeMinValue(String key) {
		minValues.remove(key);
	}
	
	public void removeMaxValue(String key) {
		maxValues.remove(key);
	}
	
	public boolean putMinValue(String key, Float value) {
		boolean ok = value!=null && item!=null && item.hasKey(key);
		
		if(ok) {
			minValues.put(key, value);
		}
		
		return ok;
	}
	
	public boolean putMaxValue(String key, Float value) {
		boolean ok = value!=null && item!=null && item.hasKey(key);
		
		if(ok) {
			maxValues.put(key, value);
		}
		
		return ok;
	}
	
	@Override
	protected boolean doCheckItem() {
		Boolean passed = true;
		
		maxValues.entrySet().stream()
			.forEach(e-> executeItemKeyCheck(e, true));
		
		minValues.entrySet().stream()
			
			.forEach(e-> executeItemKeyCheck(e, false));
		
		return passed;
	}
	
	private boolean executeItemKeyCheck(Entry<String, Float> alarmEntry, boolean itemGreaterThanAlarm){
		boolean result = false;
		
		try{
			Float itemValue = doCast(item.getValue(alarmEntry.getKey()));
			
			int res = Float.compare(itemValue, alarmEntry.getValue());
			
			res = (itemGreaterThanAlarm)?res:res*-1;
			
			result = res >= 0;
			
			if(result){
				String compareStr = (itemGreaterThanAlarm)?">":"<";
				String message = String.format("поле %s значение %s %s", alarmEntry.getKey(), compareStr, Float.toString(alarmEntry.getValue()));
				putErrorMessage(message);
			}
			
		}catch(Exception e){}
		
		return result;
	}

	@Override
	protected Float doCast(String value) throws Exception {
		return Float.valueOf(value);
	}
	
}
