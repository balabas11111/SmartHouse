package com.balabas.smarthouse.server.model;

import java.util.Map;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import org.json.JSONObject;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class SensorItem implements NameAble, ValueContainer {

    public static final String SENSOR_ITEMS_KEY = "si";
    
    private String name;
    private Map<String,String> values;
    private JSONObject data;
    
    public SensorItem(String name,JSONObject data){
        this.name = name;
        setData(data);
    }
    
    public void setData(JSONObject data){
        this.data = data;
        values = processValues(values, data);
    }
}
