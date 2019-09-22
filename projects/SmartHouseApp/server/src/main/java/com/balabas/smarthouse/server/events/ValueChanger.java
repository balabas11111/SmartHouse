package com.balabas.smarthouse.server.events;

import java.util.Map;
import java.util.Map.Entry;

import lombok.Getter;

public class ValueChanger{
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
    
    public ValueChanger(String key, String oldValue, String newValue) {
        this.key = key;
        this.oldValue = oldValue;
        this.newValue = newValue;
                         
        if(oldValue == null && newValue!=null){
            added = true;
        }else if(oldValue != null && newValue== null){
            removed = true;
        }else if(oldValue != null && newValue!=null){
            updated = !oldValue.equals(newValue);
        }
        
    }
    
    public ValueChanger(String key, String newValue, Map<String, String> values) {
        this(key, (values.containsKey(key))?values.get(key):null, newValue);
        applyTo(values);
    }
    
    public ValueChanger(Entry<String,Object> entry, Map<String, String> values){
        this(entry.getKey(), entry.getValue().toString(), values);
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
