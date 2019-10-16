package com.balabas.smarthouse.server.model;

import java.util.HashSet;
import java.util.Set;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import com.fasterxml.jackson.annotation.JsonIgnore;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Group implements SmartHouseItem {

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
        
        public static GroupType getGroupTypeByName(String name){
        	for(GroupType gt: GroupType.values()){
                if(gt.name().equalsIgnoreCase(name)){
                    return gt;
                }
            }
            return GroupType.CUSTOM;
        }
    }
    
    private String deviceId;
    
    private String name;
    
    @JsonIgnore
    private String description;
    
    private Set<DeviceEntity> entities;
  
    private GroupType groupType;
    
    @JsonIgnore
    private UpdateTimer timer;
    
    public Group(String deviceId, String name){
    	this.deviceId = deviceId;
        this.name = name;
        this.entities = new HashSet<>();
        
        this.groupType = GroupType.getGroupTypeByName(name);
        
        if(!this.groupType.equals(GroupType.CUSTOM)){
            timer = new UpdateTimer(this.groupType.refreshInterval);
        }
    }
    
    public void setDataUpdateCompleted(){
        timer.setDataReceived();
    }
    
    public DeviceEntity getEntity(String name) {
    	return entities.stream().filter(entity->entity.name.equals(name)).findFirst().orElse(null);
    }
    
    public boolean isTimeToUpdate(){
		boolean waits = this.getTimer().isWaitsForDataUpdate();
		boolean dataTooOld = this.getTimer().isNextUpdateTimeReached();
		
		return waits || dataTooOld;
	}
    
}
