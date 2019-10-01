package com.balabas.smarthouse.server.service;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.Map.Entry;

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
import com.google.common.collect.Lists;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;

@Service
public class GroupEntityUpdateServiceImpl implements GroupEntityUpdateService {

    @Override
    public List<ChangedEvent<?>> parseJsonToModel(Device device, JSONObject deviceJson) {
        List<ChangedEvent<?>> events = new ArrayList<>();
        
        if (device.getGroups()==null) {
            device.setGroups(new HashSet<Group>());
        }
        device.setData(deviceJson);
        
        if (!deviceJson.isEmpty()) {

        	processDeviceInfo(device);
        	
            if(!device.isInitialDataReceived()){
                events.add(new DeviceChangedEvent(device, DeviceEventType.ADDED));
            }
            
            for(String groupName: JSONObject.getNames(deviceJson)) {
            	if(ENTITY_FIELD_DESCRIPTION.equals(groupName)) {
            		continue;
            	}
                
                Group group = getGroupOrCreateNew(device, groupName, events);
                
                if(group==null) {
                	continue;
                }

                if (!group.getData().isEmpty()) {
                    List<EntityChangedEvent> entityEvents = new ArrayList<>();

                    for(String entityName: JSONObject.getNames(group.getData())) {
                    	
                    	if(ENTITY_FIELD_DESCRIPTION.equals(entityName)) {
                    		group.setDescription(group.getData().getString(ENTITY_FIELD_DESCRIPTION));
                    		continue;
                    	}
                        
                    	JSONObject entityJson = null;
                        
                        try{
                            entityJson = group.getData().getJSONObject(entityName);
                        }catch(Exception e){
                           continue;
                        }
                    	
                        Entity entity = getEntityOrCreateNew(device.isInitialDataReceived(), group, entityName, events);
                        
                        if(entity == null) {
                        	continue;
                        }
                        
                        List<ValuesChangeEvent> valueEvents = new ArrayList<>();
                        
                        valueEvents.add(entity.
                                setValuesFromJsonBuildEvent(entityJson, device.isInitialDataReceived()));
                        
                        valueEvents.addAll(processSensorItems(device, entity, entityJson));
                        
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
    
    private void processDeviceInfo(Device device){
    	JSONObject deviceJson = device.getData();
    	
    	if(deviceJson.has(DEVICE_FIELD_DEVICE)){
    		JSONObject info = deviceJson.getJSONObject(DEVICE_FIELD_DEVICE).getJSONObject(DEVICE_FIELD_DEVICE_INFO);
    		
    		String deviceDescr = info.optString(ENTITY_DEVICE_DEVICE_DESCRIPTION);
    		String deviceFirmware = info.optString(ENTITY_DEVICE_DEVICE_FIRMWARE);
    		
    		if(!deviceDescr.isEmpty() && !deviceDescr.equals(device.getDescription())){
    			device.setDeviceDescr(deviceDescr);
    		}
    		if(!deviceFirmware.isEmpty() && !deviceFirmware.equals(device.getDeviceFirmware())){
    			device.setDeviceFirmware(deviceFirmware);
    		}
    		
    	}
    }
    
    private Entity getEntityOrCreateNew(boolean devInitFlag, Group group, String entityName, List<ChangedEvent<?>> events) {
        Entity entity = group.getEntity(entityName);
        
        if (entity == null) {
            entity = new Entity( group.getDeviceId(), group.getName(), entityName);
            group.getEntities().add(entity);
            
            if(devInitFlag){
                events.add(new EntityChangedEvent(entity, DeviceEventType.ADDED));
            }
        } 
        
        return entity;
    }
    
    private Group getGroupOrCreateNew(Device device, String groupName, List<ChangedEvent<?>> events) {
    	JSONObject deviceJson = device.getData();
    	JSONObject groupJson = null;
        
        try{
            groupJson = deviceJson.getJSONObject(groupName);
        }catch(Exception e){
            return null;
        }
        
        Group group = device.getGroup(groupName);

        if (group == null) {
            group = new Group(device.getDeviceId(), groupName, groupJson);
            device.getGroups().add(group);
            
            if(device.isInitialDataReceived()){
                events.add(new GroupChangedEvent(group, DeviceEventType.ADDED));
            }
        } else {
            group.setData(groupJson);
        }
        
        return group;
    }
    
    private List<ValuesChangeEvent> processSensorItems(Device device, Entity entity, JSONObject entityJson) {
    	
    	List<ValuesChangeEvent> valueEvents = Lists.newArrayList();
    	
    	if(entityJson.has(ENTITY_FIELD_SENSOR_ITEMS)){
            JSONObject sensorItemsJson = entityJson.getJSONObject(ENTITY_FIELD_SENSOR_ITEMS);
            
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
    	
    	return valueEvents;
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
