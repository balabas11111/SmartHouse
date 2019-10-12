package com.balabas.smarthouse.server.events;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import lombok.Getter;
import lombok.ToString;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceEntity;

@ToString(callSuper = true)
public class EntityEvent extends ChangedEvent<DeviceEntity> {

    @Getter
    private List<ValuesChangeEvent> events =new ArrayList<>();
    
    @Getter
    private Device device;
    
    public EntityEvent(Device device, DeviceEntity target, EventType eventType) {
    	super(target, eventType);
    	this.device = device;
    }
    
    public EntityEvent(DeviceEntity target, EventType eventType, List<ValuesChangeEvent> events) {
        super(target, eventType);
        this.events = events;
    }
    
    public void addEvent(ValuesChangeEvent event){
        if(event!=null && event.hasValues()){
            events.add(event);
        }
    }
    
    public static EntityEvent build(DeviceEntity target, EventType eventType, List<ValuesChangeEvent> events){
        List<ValuesChangeEvent>  nonNulls = events.stream().filter(ev->(ev!=null && ev.hasValues())).collect(Collectors.toList());
        
        if(nonNulls.isEmpty() && eventType.equals(EventType.UPDATED)){
            return null;
        }
        
        return new EntityEvent(target, eventType, nonNulls);
    }

}
