package com.balabas.smarthouse.server.events;

import java.util.ArrayList;
import java.util.List;

import lombok.Getter;

import com.balabas.smarthouse.server.model.Group;

public class GroupChangedEvent extends ChangedEvent<Group> {

    @Getter 
    private List<EntityChangedEvent> events = new ArrayList<>();
    
    public GroupChangedEvent(Group target, DeviceEventType eventType) {
        super(target, eventType);
    }
    
    public GroupChangedEvent(Group target, DeviceEventType eventType, List<EntityChangedEvent> events) {
        super(target, eventType);
        this.events = events;
    }

    public String printDetails(){
        StringBuilder str = new StringBuilder();
        
        str.append(this.getClass().getSimpleName());
        str.append(" [ name=");
        str.append(getTargetName());
        str.append(",  tClass=");
        str.append(getTargetClass().getSimpleName());
        str.append(",  event=");
        str.append(this.getEventType());
        str.append(" ] events={");
        
        int index = 0;
        
        if(events!=null && events.size()>0){
            for(EntityChangedEvent e: events){
                str.append("{ t=");
                str.append(e.getTargetName());
                str.append(", c=");
                str.append(e.getTargetClass().getSimpleName());
                str.append(", t=");
                str.append(e.getEventType());
                str.append("}");
                if(index!=events.size()-1){
                    str.append(", ");
                }
                index++;
            }
        }
        
        str.append("}");
        
        return str.toString();
    }
    
    
}
