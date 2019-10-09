package com.balabas.smarthouse.server.events.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.DeviceEvent;
import com.balabas.smarthouse.server.events.EntityEvent;
import com.balabas.smarthouse.server.events.GroupEvent;

@Log4j2
@Service
@SuppressWarnings("rawtypes") 
public class EventProcessorsServiceImpl implements EventProcessorsService {

	@Autowired
	List<EventListenerBase<DeviceEvent>> deviceEventListeners;
	
	@Autowired(required = false)
	List<EventListenerBase<GroupEvent>> groupEventListeners;
	
	@Autowired(required = false)
	List<EventListenerBase<EntityEvent>> entityEventListeners;
	
    @Override
    public void processEvents(List<? extends ChangedEvent> events) {
        if(!events.isEmpty()){
            log.debug("Total "+events.size()+" events received");
            
            for(ChangedEvent event: events){
                log.debug("Target "+
                        event.getClass().getSimpleName()+" "+
                        event.getTargetClass().getSimpleName()+"("+event.getTargetName()+") eventType="+
                            event.getEventType());
                processEvent(event);
            }
        }
    }
    
    public void processEvent(ChangedEvent<?> event){
        if(event.getClass().equals(DeviceEvent.class)){
            processEvent((DeviceEvent) event);
        }else if(event.getClass().equals(GroupEvent.class)){
            processEvent((GroupEvent) event);
        }else if(event.getClass().equals(EntityEvent.class)){
            processEvent((EntityEvent) event);
        }
    }
    
    public void processEvent(DeviceEvent event){
    	if(deviceEventListeners!=null && !deviceEventListeners.isEmpty()){
    		deviceEventListeners.stream()
    			.forEach(proc-> proc.processEvent(event));
    	}else{
    		log.warn("DeviceChangedEvent : " + event.toString());
    	}
    }
    
    public void processEvent(GroupEvent event){
    	if(groupEventListeners!=null && !groupEventListeners.isEmpty()){
    		groupEventListeners.stream()
				.forEach(proc-> proc.processEvent(event));
    	}else{
    		//log.warn("GroupChangedEvent : " + event.printDetails());
    	}
    }
    
    public void processEvent(EntityEvent event){
    	if(entityEventListeners!=null && !entityEventListeners.isEmpty()){
    		entityEventListeners.stream()
				.forEach(proc-> proc.processEvent(event));
    	}else{
    		log.warn("EntityChangedEvent "+event.toString());
    	}
        
    }

}
