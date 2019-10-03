package com.balabas.smarthouse.server.events;

import java.util.Map;
import java.util.Map.Entry;

import lombok.Getter;

public class ValueChangeOperation{
    @Getter
    private boolean added;
    @Getter
    private boolean removed;
    @Getter
    private boolean updated;
    
    @Getter
    private String key;
    @Getter
    private String oldValue;
    @Getter
    private String newValue;
    
    public ValueChangeOperation(String key, String oldValue, String newValue) {
        this.key = key;
        this.oldValue = oldValue;
        this.newValue = newValue;
                         
        if(oldValue == null && newValue!=null){
            added = true;
        }else if(oldValue != null && newValue== null){
            removed = true;
        }else if(newValue!=null){
            updated = !oldValue.equals(newValue);
        }
        
    }
    
    public static ValueChangeOperation build(String key, String newValue, Map<String, String> values, boolean apply) {
    	ValueChangeOperation oper = new ValueChangeOperation(key, (values.containsKey(key))?values.get(key):null, newValue);
    	if(apply){
    		oper.applyTo(values);
    	}
        return oper;
    }
    
    public static ValueChangeOperation buildAndApply(Entry<String,Object> entry, Map<String, String> values){
        return build(entry.getKey(), entry.getValue().toString(), values, true);
    }
    
    public boolean applyTo(Map<String,String> map){
        if(removed){
            map.remove(key);
            return true;
        }
        if(added || updated){
            map.put(key, newValue);
            return true;
        }
        return false;
    }
    
    public boolean isValueChanged(){
        return added || removed || updated;
    }
}
