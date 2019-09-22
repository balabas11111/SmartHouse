package com.balabas.smarthouse.server.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import lombok.Getter;

import org.json.JSONArray;
import org.json.JSONObject;

import com.balabas.smarthouse.server.events.ValueChanger;
import com.balabas.smarthouse.server.events.ValuesChangeEvent;

public class ValueContainer implements JsonDataContainer, NameAble{

    @Getter
    protected String name;
    
    @Getter
    protected Map<String, String> values = new HashMap<>();
    
    @Getter
    protected JSONObject data;
    
    public String getValue(String key){
        return (!hasKey(key))?
                    null:values.get(key);
    }
    
    public boolean hasKey(String key){
        return values.containsKey(key);
    }
    
    public boolean setValue(String key,String value){
        return setValueWithChanger(key, value).isValueChanged();
    }
    
    public ValueChanger setValueWithChanger(String key,String value){
        return new ValueChanger(key, value, this.values);
    }
    
    public ValuesChangeEvent setValuesFromJsonBuildEvent(JSONObject data, boolean getChangers){
        List<ValueChanger> changers = setValuesFromJson(data, getChangers);
        
        return ValuesChangeEvent.build(this, changers);
    }
    
    public List<ValueChanger> setValuesFromJson(JSONObject data, boolean getChangers){
        List<ValueChanger> result = new ArrayList<>();
        
        this.data = data;
        
        Map<String, Object> jsonMap = data.toMap();
        
        for(Entry<String,Object> entry: jsonMap.entrySet()){
            if(!entry.getKey().equals(SensorItem.SENSOR_ITEMS_KEY)
                    && !(entry.getValue() instanceof JSONObject)
                    && !(entry.getValue() instanceof JSONArray)){
                
                ValueChanger changer = new ValueChanger(entry, this.values);
                if(getChangers && changer.isValueChanged()){
                    result.add(changer);
                }
            }
        }
        
        return result;
    }
    
}
