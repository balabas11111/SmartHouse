package com.balabas.smarthouse.server.model;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.json.JSONArray;
import org.json.JSONObject;

public interface ValueContainer {

    Map<String, String> getValues();
    
    default Map<String, String> processValues(Map<String, String> oldValues, JSONObject data){
        Map<String,String> result = (oldValues==null)?new HashMap<>():oldValues;

        Map<String, Object> jsonMap = data.toMap();
        
        for(Entry<String,Object> entry: jsonMap.entrySet()){
            if(!entry.getKey().equals(SensorItem.SENSOR_ITEMS_KEY)
                    && !(entry.getValue() instanceof JSONObject)
                    && !(entry.getValue() instanceof JSONArray)){
                result.put(entry.getKey(), entry.getValue().toString());
            }
        }
        
        return result;
    }
}
