package com.balabas.smarthouse.server.model;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

import org.json.JSONObject;

import com.balabas.smarthouse.server.events.ValueChanger;

@Data
@NoArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class Entity extends ValueContainer  {

    private Set<SensorItem> sensorItems;
    
    public Entity(String name){
        this.name = name;
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
                            data.has(SensorItem.SENSOR_ITEMS_KEY);
        if(doCreate){
            this.sensorItems = new HashSet<>();
        }
    }
    
}
