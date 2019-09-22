package com.balabas.smarthouse.server.model;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class SensorItem extends ValueContainer {

    public static final String SENSOR_ITEMS_KEY = "si";
    
    public SensorItem(String name){
        this.name = name;
    }
}
