package com.balabas.smarthouse.server.model;

import java.util.Set;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class Entity extends ValueContainer  {

	@Getter
	private String groupName;
	
	
    private Set<SensorItem> sensorItems;
    
    public Entity(String deviceId, String groupName, String entityName){
    	this.deviceId = deviceId;
    	this.groupName = groupName;
        this.name = entityName;
    }
    
    public boolean hasSensorItems(){
        return sensorItems!=null && !sensorItems.isEmpty();
    }
    
}
