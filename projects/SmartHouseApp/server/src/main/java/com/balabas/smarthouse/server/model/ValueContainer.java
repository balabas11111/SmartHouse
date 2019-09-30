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

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;

public class ValueContainer implements JsonDataContainer, NameAble{

	@Getter
	protected EntityClass entityRenderer = EntityClass.DEFAULT;
	
    @Getter
    protected String name;
    
    @Getter
    protected String description;
    
    @Getter
    protected Map<String, String> values = new HashMap<>();
    
    @Getter
    protected JSONObject data;
    
    public String getValue(String key){
        return values.getOrDefault(key, null);
    }
    
    public boolean hasRenderer() {
    	return !this.entityRenderer.equals(EntityClass.DEFAULT);
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
        	String key = entry.getKey();
        	
        	if(ENTITY_FIELD_ITEM_CLASS.equals(key)) {
        		this.entityRenderer = EntityClass.getByKey(entry.getValue().toString());
        	}
        	if(ENTITY_FIELD_DESCRIPTION.equals(key)) {
        		this.description = entry.getValue().toString();
        	}
            if(!key.equals(ENTITY_FIELD_SENSOR_ITEMS)
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
