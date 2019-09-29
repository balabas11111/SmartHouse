package com.balabas.smarthouse.server.model;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

import org.json.JSONObject;

import com.balabas.smarthouse.server.events.ValueChanger;

import static com.balabas.smarthouse.server.model.ModelConstants.ENTITY_FIELD_SENSOR_ITEMS;

@Data
@NoArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class Entity extends ValueContainer  {

	private String deviceId;
	
	private String groupName;
	
    private Set<SensorItem> sensorItems;
    
    public Entity(String deviceId, String groupName, String entityName){
    	this.deviceId = deviceId;
    	this.groupName = groupName;
        this.name = entityName;
    }
    
    @Override
    public List<ValueChanger> setValuesFromJson(JSONObject data, boolean getChangers){
        checkToCreateSensorItems(data);
        
        return super.setValuesFromJson(data, getChangers);
    }
    
    public boolean hasSensorItems(){
        return sensorItems!=null && sensorItems.size()>0;
    }
    
    private void checkToCreateSensorItems(JSONObject data){
        boolean doCreate = this.sensorItems == null &&
                            data.has(ENTITY_FIELD_SENSOR_ITEMS);
        if(doCreate){
            this.sensorItems = new HashSet<>();
        }
    }
    
}
