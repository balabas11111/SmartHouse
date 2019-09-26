package com.balabas.smarthouse.server.service;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.Map.Entry;

import lombok.extern.log4j.Log4j2;

import org.json.JSONObject;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.ChangedEvent.DeviceEventType;
import com.balabas.smarthouse.server.events.DeviceChangedEvent;
import com.balabas.smarthouse.server.events.EntityChangedEvent;
import com.balabas.smarthouse.server.events.GroupChangedEvent;
import com.balabas.smarthouse.server.events.ValuesChangeEvent;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.NameAble;
import com.balabas.smarthouse.server.model.SensorItem;

@Service
public class GroupEntityUpdateServiceImpl implements GroupEntityUpdateService {

    @Override
    public List<ChangedEvent<?>> parseJsonToModel(Device device, JSONObject deviceJson) {
        List<ChangedEvent<?>> events = new ArrayList<>();
        
        if (device.getGroups()==null) {
            device.setGroups(new HashSet<Group>());
        }
        device.setData(deviceJson);
        Set<Group> deviceGroups = device.getGroups();
        
        if (!deviceJson.isEmpty()) {

            if(!device.isInitialDataReceived()){
                events.add(new DeviceChangedEvent(device, DeviceEventType.ADDED));
            }
            
            for(String groupName: JSONObject.getNames(deviceJson)) {
                
                JSONObject groupJson = null;
                
                try{
                    groupJson = deviceJson.getJSONObject(groupName);
                }catch(Exception e){
                    continue;
                }
                
                Optional<Group> groupOpt = getGroupByName(deviceGroups, groupName);

                Group group = null;

                if (!groupOpt.isPresent()) {
                    group = new Group(groupName, groupJson);
                    deviceGroups.add(group);
                    
                    if(device.isInitialDataReceived()){
                        events.add(new GroupChangedEvent(group, DeviceEventType.ADDED));
                    }
                } else {
                    group = groupOpt.get();
                    group.setData(groupJson);
                }

                if (!groupJson.isEmpty()) {
                    Set<Entity> groupEntities = group.getEntities();
                    List<EntityChangedEvent> entityEvents = new ArrayList<>();

                    for(String entityName: JSONObject.getNames(groupJson)) {
                        
                        JSONObject entityJson = null;
                        
                        try{
                            entityJson = groupJson.getJSONObject(entityName);
                        }catch(Exception e){
                            continue;
                        }
                        
                        Optional<Entity> entityOpt = Optional
                                .ofNullable(getItemByName(groupEntities,
                                        entityName));
                        Entity entity = null;
                        
                        if (!entityOpt.isPresent()) {
                            entity = new Entity( entityName);
                            groupEntities.add(entity);
                            
                            if(device.isInitialDataReceived()){
                                events.add(new EntityChangedEvent(entity, DeviceEventType.ADDED));
                            }
                        } else {
                            entity = entityOpt.get();
                        }
                        
                        List<ValuesChangeEvent> valueEvents = new ArrayList<>();
                        
                        valueEvents.add(entity.
                                setValuesFromJsonBuildEvent(entityJson, device.isInitialDataReceived()));
                        
                        if(entityJson.has(SensorItem.SENSOR_ITEMS_KEY)){
                            JSONObject sensorItemsJson = entityJson.getJSONObject(SensorItem.SENSOR_ITEMS_KEY);
                            
                            Set<SensorItem> sensorItems = entity.getSensorItems();
                            
                            for(Entry<String,Object> siEntry: sensorItemsJson.toMap().entrySet()){
                            
                                if(!isJsonObject(siEntry.getValue())){
                                    continue;
                                }
                                String siName = siEntry.getKey();
                                
                                Optional<SensorItem> siOpt = Optional
                                        .ofNullable(getItemByName(sensorItems,
                                                siName));
                                
                                SensorItem si = null;
                                JSONObject siJson = sensorItemsJson.getJSONObject(siName);
                                
                                if(!siOpt.isPresent()){
                                    si = new SensorItem(siName);
                                    sensorItems.add(si);
                                }else{
                                    si = siOpt.get();
                                }
                                
                                valueEvents.add(si.setValuesFromJsonBuildEvent(siJson, device.isInitialDataReceived()));
                            }
                        }
                        
                        EntityChangedEvent entityValuesChangeEvent = 
                                EntityChangedEvent.build(entity, DeviceEventType.UPDATED, valueEvents);
                        
                        if(entityValuesChangeEvent!=null){
                            entityEvents.add(entityValuesChangeEvent);
                        }
                    }
                    
                    if(!entityEvents.isEmpty()){
                        GroupChangedEvent groupEvent = new GroupChangedEvent(group, DeviceEventType.UPDATED, entityEvents);
                        if(groupEvent!=null){
                            events.add(groupEvent);
                        }
                        
                    }
                }
                
                group.setDataUpdateCompleted();
            }
        
            device.setDataUpdateCompleted();
        }else{
            device.setDataUpdateFailed();
        }

        
        
        
        return events;
    }

    @Override
    public Optional<Group> getGroupByName(Set<Group> groups, String name) {

        Group group = (groups != null) ? getItemByName(groups, name) : null;

        Optional<Group> result = Optional.ofNullable(group);

        return result;
    }

    private static boolean isJsonObject(Object obj) {
        return (obj instanceof JSONObject);
    }

    private static <T extends NameAble> T getItemByName(Set<T> items,
            String name) {
        for (T item : items) {
            if (item.getName().equals(name)) {
                return item;
            }
        }
        return null;
    }

}
