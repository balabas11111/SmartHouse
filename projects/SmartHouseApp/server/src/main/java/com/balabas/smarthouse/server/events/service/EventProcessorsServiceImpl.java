package com.balabas.smarthouse.server.events.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.DeviceChangedEvent;
import com.balabas.smarthouse.server.events.EntityChangedEvent;
import com.balabas.smarthouse.server.events.GroupChangedEvent;

@Log4j2
@Service
@SuppressWarnings("rawtypes") 
public class EventProcessorsServiceImpl implements EventProcessorsService {

	@Autowired
	List<EventProcessorBase<DeviceChangedEvent>> deviceEventsProcessors;
	
	@Autowired(required = false)
	List<EventProcessorBase<GroupChangedEvent>> groupEventsProcessors;
	
	@Autowired(required = false)
	List<EventProcessorBase<EntityChangedEvent>> entityEventsProcessors;
	
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
        if(event.getClass().equals(DeviceChangedEvent.class)){
            processEvent((DeviceChangedEvent) event);
        }else if(event.getClass().equals(GroupChangedEvent.class)){
            processEvent((GroupChangedEvent) event);
        }else if(event.getClass().equals(EntityChangedEvent.class)){
            processEvent((EntityChangedEvent) event);
        }
    }
    
    public void processEvent(DeviceChangedEvent event){
    	if(deviceEventsProcessors!=null && !deviceEventsProcessors.isEmpty()){
    		deviceEventsProcessors.stream()
    			.forEach(proc-> proc.process(event));
    	}else{
    		log.warn("DeviceChangedEvent : " + event.toString());
    	}
    }
    
    public void processEvent(GroupChangedEvent event){
    	if(groupEventsProcessors!=null && !groupEventsProcessors.isEmpty()){
    		groupEventsProcessors.stream()
				.forEach(proc-> proc.process(event));
    	}else{
    		//log.warn("GroupChangedEvent : " + event.printDetails());
    	}
    }
    
    public void processEvent(EntityChangedEvent event){
    	if(entityEventsProcessors!=null && !entityEventsProcessors.isEmpty()){
    		entityEventsProcessors.stream()
				.forEach(proc-> proc.process(event));
    	}else{
    		log.warn("EntityChangedEvent "+event.toString());
    	}
        
    }

}
