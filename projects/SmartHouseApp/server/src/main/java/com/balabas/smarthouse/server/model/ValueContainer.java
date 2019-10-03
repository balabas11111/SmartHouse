package com.balabas.smarthouse.server.model;

import java.util.HashMap;
import java.util.Map;

import lombok.Getter;
import lombok.Setter;

import org.json.JSONObject;

import com.balabas.smarthouse.server.events.ValueChangeOperation;

public class ValueContainer implements JsonDataContainer, SmartHouseItem{

	@Getter 
	protected EntityClass entityRenderer = EntityClass.DEFAULT;
	
	@Getter
	protected String deviceId;
	
    @Getter
    protected String name;
    
    @Getter
    protected String description;
    
    @Getter
    protected String parentName = null;
    
    @Getter
    protected Map<String, String> values = new HashMap<>();
    
    @Getter @Setter
    protected JSONObject data;
    
    public void setEntityRenderer(String entityRenderer){
    	this.entityRenderer = EntityClass.getByKey(entityRenderer);
    }
    
    public void setDescription(String description){
    	this.description = description;
    }
    
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
    
    public ValueChangeOperation setValueWithChanger(String key,String value){
        return ValueChangeOperation.build(key, value, this.values, true);
    }
    
}
