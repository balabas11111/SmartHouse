package com.balabas.smarthouse.server.model;

import java.util.HashMap;
import java.util.Map;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import com.balabas.smarthouse.server.entity.model.descriptor.IDeviceEntityDescriptor;

@ToString
public class ValueContainer implements SmartHouseItem{

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
    protected IDeviceEntityDescriptor descriptor;
   
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
