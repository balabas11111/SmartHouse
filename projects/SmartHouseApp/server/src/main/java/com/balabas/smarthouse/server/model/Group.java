package com.balabas.smarthouse.server.model;

import java.util.HashSet;
import java.util.Set;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.extern.log4j.Log4j2;

import org.json.JSONObject;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Group implements NameAble {

    public enum GroupType{
        DEVICE(1000*60),
        SENSORS(1000*30),
        SETTINGS(1000*240),
        SERVICES(1000*30),
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
    
    public Group(String name,JSONObject data){
        this.name = name;
        this.entities = new HashSet<>();
        this.data = data;
        
        this.groupType = getGroupTypeByName(name);
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
