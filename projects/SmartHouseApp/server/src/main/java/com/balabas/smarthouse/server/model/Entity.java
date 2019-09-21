package com.balabas.smarthouse.server.model;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import org.json.JSONObject;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Entity implements NameAble, ValueContainer {

    private String name;
    private JSONObject data;
    
    private Map<String,String> values;
    
    private Set<SensorItem> sensorItems;
    
    public Entity(String name, JSONObject data){
        this.name = name;
        setData(data);
        
        if(data.has(SensorItem.SENSOR_ITEMS_KEY)){
            this.sensorItems = new HashSet<>(); 
        }
    }
    
    public void setData(JSONObject data){
        this.data = data;
        values = processValues(values, data);
    }
    
    public boolean hasSensorItems(){
        return sensorItems!=null && sensorItems.size()>0;
    }
}
