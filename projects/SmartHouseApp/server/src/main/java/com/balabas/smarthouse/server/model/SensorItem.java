package com.balabas.smarthouse.server.model;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class SensorItem extends ValueContainer {

    public SensorItem(String name, String parentName){
        this.name = name;
        this.parentName = parentName;
    }
}
