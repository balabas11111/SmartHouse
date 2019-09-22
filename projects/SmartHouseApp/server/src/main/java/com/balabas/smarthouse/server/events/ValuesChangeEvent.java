package com.balabas.smarthouse.server.events;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import lombok.Getter;

import com.balabas.smarthouse.server.model.ValueContainer;

public class ValuesChangeEvent {

    @Getter
    private ValueContainer target;
    
    private Map<String, String> keyOldValueMap= new HashMap<>();

    public ValuesChangeEvent(ValueContainer target) {
        this.target = target;
    }
    
    boolean hasValues(){
        return keyOldValueMap.size()>0;
    }
    
    boolean isValueChanged(String key){
        return keyOldValueMap.containsKey(key);
    }
    
    public String getOldValue(String key){
        if(!isValueChanged(key)){
            return null;
        }
        
        return keyOldValueMap.get(key);
    }
    
    public String getNewValue(String key){
        if(!isValueChanged(key)){
            return null;
        }
        
        return keyOldValueMap.get(key);
    }
    
    public void putFromChanger(ValueChanger changer){
        if(target.hasKey(changer.getKey())){
            keyOldValueMap.put(changer.getKey(), changer.getOldValue());
        }
    }
    
    public static ValuesChangeEvent build(ValueContainer target, List<ValueChanger> changers){
        if(changers== null || changers.isEmpty()){
            return null;
        }
        
        ValuesChangeEvent result = new ValuesChangeEvent(target);
        
        changers.stream().filter(chg->chg.isValueChanged())
            .forEach(chg->{
                    result.putFromChanger(chg);
                });
        
        return result;
    }
    
}
