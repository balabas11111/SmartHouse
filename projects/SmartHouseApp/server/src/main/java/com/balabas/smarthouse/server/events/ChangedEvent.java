package com.balabas.smarthouse.server.events;

import com.balabas.smarthouse.server.model.NameAble;

import lombok.Getter;

public class ChangedEvent<T extends NameAble> {

    public enum EventType{
        UPDATED,
        ADDED,
        REMOVED
    }
    
    @Getter
    private T target;
    
    @Getter
    private EventType eventType;
    
    public ChangedEvent(T target, EventType eventType ){
        this.target = target;
        this.eventType = eventType;
    }
    
    public String getTargetName(){
        return getTarget().getName();
    }
    
    public Class<?> getTargetClass(){
        return getTarget().getClass();
    }
}
