package com.balabas.smarthouse.server.events;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import lombok.Getter;

import com.balabas.smarthouse.server.model.Entity;

public class EntityChangedEvent extends ChangedEvent<Entity> {

    @Getter
    private List<ValuesChangeEvent> events =new ArrayList<>();
    
    public EntityChangedEvent(Entity target, EventType eventType) {
        super(target, eventType);
    }
    
    public EntityChangedEvent(Entity target, EventType eventType, List<ValuesChangeEvent> events) {
        super(target, eventType);
        this.events = events;
    }
    
    public void addEvent(ValuesChangeEvent event){
        if(event!=null && event.hasValues()){
            events.add(event);
        }
    }
    
    public static EntityChangedEvent build(Entity target, EventType eventType, List<ValuesChangeEvent> events){
        List<ValuesChangeEvent>  nonNulls = events.stream().filter(ev->(ev!=null && ev.hasValues())).collect(Collectors.toList());
        
        if(nonNulls.isEmpty() && eventType.equals(EventType.UPDATED)){
            return null;
        }
        
        return new EntityChangedEvent(target, eventType, nonNulls);
    }

}
