package com.balabas.smarthouse.server.service;

import java.util.List;

import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.DeviceChangedEvent;
import com.balabas.smarthouse.server.events.EntityChangedEvent;
import com.balabas.smarthouse.server.events.GroupChangedEvent;
import com.balabas.smarthouse.server.model.Device;

@Log4j2
@Service
public class DataUpdateEventServiceImpl implements DataUpdateEventService {

    @Override
    public void processEvents(List<ChangedEvent<?>> events) {
        if(events.size()>0){
            log.info("Total "+events.size()+" events received");
            
            for(ChangedEvent<?> event: events){
                log.info("Target "+
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
        log.info("process DeviceChangedEvent");
    }
    
    public void processEvent(GroupChangedEvent event){
        log.info(event.printDetails());
    }
    
    public void processEvent(EntityChangedEvent event){
        log.info("process EntityChangedEvent");
    }

}
