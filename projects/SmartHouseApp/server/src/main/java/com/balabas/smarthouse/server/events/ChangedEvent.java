package com.balabas.smarthouse.server.events;

import com.balabas.smarthouse.server.model.SmartHouseItem;

import lombok.Getter;

public class ChangedEvent<T extends SmartHouseItem> {

    public enum EventType{
    	REGISTERED,
    	REREGISTERED,
    	ADDED,
    	INITIAL_DATA_RECEIVED,
        UPDATED,
        REMOVED,
        DATA_UPDATE_DISPATCHED,
        DATA_RECEIVED,
        DATA_TIMED_OUT,
        DATA_PARSE_FAILED,
        DISCONNECTED
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
