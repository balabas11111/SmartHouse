package com.balabas.smarthouse.server.events;

import com.balabas.smarthouse.server.model.SmartHouseItem;

import lombok.Getter;

public class ChangedEvent<T extends SmartHouseItem> {

    public enum DeviceEventType{
        UPDATED,
        ADDED,
        REMOVED,
        POST_RESULT_RECEIVED
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
