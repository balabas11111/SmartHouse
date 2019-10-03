package com.balabas.smarthouse.server.events;

import com.balabas.smarthouse.server.model.SmartHouseItem;

import lombok.Getter;

public class ChangedEvent<T extends SmartHouseItem> {

    public enum DeviceEventType{
    	REGISTERED,
    	REREGISTERED,
    	ADDED,
        UPDATED,
        REMOVED,
        DATA_UPDATE_DISPATCHED,
        DATA_RECEIVED,
        DATA_EXPIRED
    }
    
    @Getter
    private T target;
    
    @Getter
    private DeviceEventType eventType;
    
    public ChangedEvent(T target, DeviceEventType eventType ){
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
