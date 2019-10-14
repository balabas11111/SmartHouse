package com.balabas.smarthouse.server.service;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.Map.Entry;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityDescriptorMap;
import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.EntityEvent;
import com.balabas.smarthouse.server.events.GroupEvent;
import com.balabas.smarthouse.server.events.ValueChangeOperation;
import com.balabas.smarthouse.server.events.ValuesChangeEvent;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.model.ValueContainer;
import com.balabas.smarthouse.server.model.SensorItem;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceConstants.FIELD_SPLITTER;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;

import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.ADDED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.INITIAL_DATA_RECEIVED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.UPDATED;

@Log4j2
@Service
@SuppressWarnings("rawtypes")
public class DeviceJsonAdapterBaseImpl implements DeviceJsonAdapter {

    @Override
    public List<ChangedEvent> adapt(String data, Device device) {
    	log.debug("Adapt data " + data);
    	JSONObject deviceJson = new JSONObject(data);
        List<ChangedEvent> events = new ArrayList<>();
        
        if (device.getGroups()==null) {
            device.setGroups(new HashSet<Group>());
        }
        device.setData(deviceJson);
        
        if (!deviceJson.isEmpty()) {

        	processDeviceInfo(device);
        	
            for(String groupName: JSONObject.getNames(deviceJson)) {
            	if(ENTITY_FIELD_DESCRIPTION.equals(groupName)) {
            		continue;
            	}
                
                Group group = getGroupOrCreateNew(device, groupName, events);
                
                if(group!=null) {

	                if (!group.getData().isEmpty()) {
	                    List<EntityEvent> entityEvents = new ArrayList<>();
	
	                    for(String entityName: JSONObject.getNames(group.getData())) {
	                    	
	                    	if(ENTITY_FIELD_DESCRIPTION.equals(entityName)) {
	                    		group.setDescription(group.getData().getString(ENTITY_FIELD_DESCRIPTION));
	                    		continue;
	                    	}
	                        
	                    	processEntityJson(!device.isInitialDataReceived(), device, group, entityName, entityEvents);
	                    }
	                    
	                    if(!entityEvents.isEmpty()){
	                    	events.addAll(entityEvents);
	                    	
	                        GroupEvent groupEvent = new GroupEvent(group, UPDATED, entityEvents);
	                        if(groupEvent!=null){
	                            events.add(groupEvent);
	                        }
	                        
	                    }
	                }
	                
	                group.setDataUpdateCompleted();
                }
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
    		JSONObject deviceDeviceJson = deviceJson.getJSONObject(DEVICE_FIELD_DEVICE);
    				
    		if(deviceDeviceJson.has(DEVICE_FIELD_DEVICE_INFO)){
				JSONObject info = deviceDeviceJson.getJSONObject(DEVICE_FIELD_DEVICE_INFO);
	    		
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
    }
    
    private Group getGroupOrCreateNew(Device device, String groupName, List<ChangedEvent> events) {
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
                events.add(new GroupEvent(group, INITIAL_DATA_RECEIVED));
            }
        } else {
            group.setData(groupJson);
        }
        
        return group;
    }
    
    private void processEntityJson(boolean noEventProduceApplyOnly, Device device, Group group, String entityName, List<EntityEvent> events){
    	JSONObject entityJson = null;
        
        try{
            entityJson = group.getData().getJSONObject(entityName);
        }catch(Exception e){
           return;
        }
        
        DeviceEntity entity = getEntityOrCreateNew(noEventProduceApplyOnly, device, group, entityName, events);
        
        List<ValuesChangeEvent> valueEvents = new ArrayList<>();
        
        valueEvents.add(putJsonToValueContainer(noEventProduceApplyOnly, entity, entityJson));
        
        valueEvents.addAll(processSensorItemsValues(noEventProduceApplyOnly, entity, entityJson));
        
        EntityEvent entityValuesChangeEvent = 
                EntityEvent.build(entity, UPDATED, valueEvents);
        
        if(entityValuesChangeEvent!=null){
        	events.add(entityValuesChangeEvent);
        }
    }
    
    private DeviceEntity getEntityOrCreateNew(boolean noEventProduceApplyOnly, Device device, Group group, String entityName, List<EntityEvent> events) {
        DeviceEntity entity = group.getEntity(entityName);
        
        if (entity == null) {
            entity = new DeviceEntity( group.getDeviceId(), group.getName(), entityName);
            group.getEntities().add(entity);
            
            events.add(new EntityEvent(device, entity, ADDED));
            events.add(new EntityEvent(device, entity, INITIAL_DATA_RECEIVED));
        } 
        
        return entity;
    }
    
    private ValuesChangeEvent putJsonToValueContainer(boolean noEventProduceApplyOnly, ValueContainer entity, JSONObject entityJson) {
    	List<ValueChangeOperation> changeOperations = new ArrayList<>();
        
        entity.setData(entityJson);
        
        Map<String, Object> jsonMap = entityJson.toMap();
        
        for(Entry<String,Object> entry: jsonMap.entrySet()){
        	String key = entry.getKey();
        	
        	if((entity instanceof DeviceEntity) && ENTITY_FIELD_SWG.equals(key)) {
        		JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);
        		
        		log.info("Entity descriptor : " +descriptorJson);
        		
        		Optional.ofNullable(EntityDescriptorMap.fromJson(descriptorJson, entity.getName()))
        		.ifPresent(entity::setDescriptor);
        	} else
        	if(ENTITY_FIELD_ITEM_CLASS.equals(key)) {
        		entity.setEntityRenderer(entry.getValue().toString());
        	} else
        	if(ENTITY_FIELD_DESCRIPTION.equals(key)) {
        		entity.setDescription(entry.getValue().toString());
        	} else
            if(!key.equals(ENTITY_FIELD_SENSOR_ITEMS)
                    && !(entry.getValue() instanceof JSONObject)
                    && !(entry.getValue() instanceof JSONArray)){
                
                ValueChangeOperation operation = ValueChangeOperation.buildAndApply(entry, entity.getValues());
                if(!noEventProduceApplyOnly && operation.isValueChanged()){
                	changeOperations.add(operation);
                }
            }
        }
        
        if(changeOperations== null || changeOperations.isEmpty()){
            return null;
        }
        
        ValuesChangeEvent event = new ValuesChangeEvent(entity);
        
        changeOperations.stream().filter(ValueChangeOperation::isValueChanged)
            .forEach(operation-> event.putOperation(operation));
        
        return event;
    }
    
    private List<ValuesChangeEvent> processSensorItemsValues(boolean noEventProduceApplyOnly, DeviceEntity entity, JSONObject entityJson) {
    	
    	List<ValuesChangeEvent> valueEvents = Lists.newArrayList();
    	
    	if(entityJson.has(ENTITY_FIELD_SENSOR_ITEMS)){
            JSONObject sensorItemsJson = entityJson.getJSONObject(ENTITY_FIELD_SENSOR_ITEMS);
            
            if(entity.getSensorItems()==null){
            	entity.setSensorItems(new HashSet<SensorItem>());
            }
            
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
                    si = new SensorItem(siName, entity.getName());
                    sensorItems.add(si);
                }else{
                    si = siOpt.get();
                }
                
                
                String removePreffix = siName + FIELD_SPLITTER;
                
                //get SensorItem value if presented in Entity instead of sensorItem
                entity.getValues().entrySet().stream()
                		.filter(entry -> entry.getKey().startsWith(removePreffix))
                		.forEach(entry -> putCleanedSensorItemToJson(entry, removePreffix, siJson));
                		
                
                valueEvents.add(putJsonToValueContainer(noEventProduceApplyOnly, si, siJson));
                
                for(Entry<String,Object> siInnerEntry: siJson.toMap().entrySet()){
                	if(!ENTITY_FIELD_DESCRIPTION.equals(siInnerEntry.getKey())){
                		String expectedKey = siName + FIELD_SPLITTER + siInnerEntry.getKey();
                		String expectedValue = siInnerEntry.getValue().toString();
                		if(!entity.hasKey(expectedKey)){
                			entity.setValue(expectedKey, expectedValue);
                		}
                		if(!entity.getData().has(expectedKey)){
                			entity.getData().put(expectedKey, expectedValue);
                		}
                	}
                }
            }
        }
    	
    	return valueEvents;
    }
    
    private void putCleanedSensorItemToJson(Entry<String,String> entry, String removePreffix, JSONObject siJson){
    	String key = entry.getKey();
    	String cleanedKey = key.substring(key.lastIndexOf(removePreffix) + removePreffix.length());
    	
    	siJson.put(cleanedKey, entry.getValue());
    }

    private static boolean isJsonObject(Object obj) {
        return (obj instanceof JSONObject) || (obj instanceof Map<?, ?>);
    }

    private static <T extends SmartHouseItem> T getItemByName(Set<T> items,
            String name) {
        for (T item : items) {
            if (item.getName().equals(name)) {
                return item;
            }
        }
        return null;
    }

}
