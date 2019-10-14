package com.balabas.smarthouse.server.model;

import java.util.HashMap;
import java.util.Map;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.descriptor.IEntityDescriptor;
import com.balabas.smarthouse.server.entity.model.descriptor.IEntityDescriptorMap;

@ToString
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
    
    @Getter @Setter
    protected Map<String, String> values = new HashMap<>();
    
    @Getter @Setter
    protected IEntityDescriptorMap descriptor;
    
    @Getter @Setter
    protected JSONObject data;
    
    public void setEntityRenderer(String entityRenderer){
    	this.entityRenderer = EntityClass.getByKey(entityRenderer);
    }
    
    public void setDescription(String description){
    	this.description = description;
    }
    
    public boolean hasRenderer() {
    	return !this.entityRenderer.equals(EntityClass.DEFAULT);
    }
    
    public boolean hasKey(String key){
        return values.containsKey(key);
    }
    
    public String getValue(String key){
        return values.getOrDefault(key, null);
    }
    
    public void setValue(String key,String value){
        values.put(key, value);
    }
    
}
