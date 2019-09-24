package com.balabas.smarthouse.server.model;

import java.util.HashSet;
import java.util.Set;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import org.json.JSONObject;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Group implements NameAble, JsonDataContainer {

    public enum GroupType{
        DEVICE(1000*120),
        SENSORS(1000*120),
        SETTINGS(1000*580),
        SERVICES(1000*60),
        CUSTOM(-1);
        
        long refreshInterval;
        
        GroupType(long refreshInterval){
            this.refreshInterval = refreshInterval;
        }
        
        long getRefreshInterval(){
            return this.refreshInterval;
        }
    }
    
    private String name;
    
    private Set<Entity> entities;
    
    private JSONObject data;
    
    private GroupType groupType;
    
    private UpdateTimer timer;
    
    public Group(String name,JSONObject data){
        this.name = name;
        this.entities = new HashSet<>();
        this.data = data;
        
        this.groupType = getGroupTypeByName(name);
        
        if(!this.groupType.equals(GroupType.CUSTOM)){
            timer = new UpdateTimer(this, this.groupType.refreshInterval);
        }
    }
    
    public void setDataUpdateCompleted(){
        timer.setDataReceived();
    }
    
    private static GroupType getGroupTypeByName(String name){
        for(GroupType gt: GroupType.values()){
            if(gt.name().equalsIgnoreCase(name)){
                return gt;
            }
        }
        return GroupType.CUSTOM;
    }
}
