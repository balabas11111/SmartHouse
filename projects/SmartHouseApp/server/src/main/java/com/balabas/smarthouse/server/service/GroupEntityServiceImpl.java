package com.balabas.smarthouse.server.service;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import lombok.extern.log4j.Log4j2;

import org.json.JSONObject;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.NameAble;
import com.balabas.smarthouse.server.model.SensorItem;

@Log4j2
@Service
public class GroupEntityServiceImpl implements GroupEntityService {

    private Map<String, Set<Group>> model =new HashMap<>();

    @Override
    public boolean parseJsonToModel(Device device, JSONObject json) {
        String deviceId = device.getDeviceId();
        
        if (!model.containsKey(deviceId)) {
            model.put(deviceId, new HashSet<Group>());
        }

        Set<Group> deviceGroups = model.get(deviceId);
        
        device.setGroups(deviceGroups);

        if (!json.isEmpty()) {
            String[] groupNames = JSONObject.getNames(json);

            for (String groupName : groupNames) {
                Optional<Group> groupOpt = getGroupByName(deviceId, groupName);
                JSONObject groupJson = json.getJSONObject(groupName);

                Group group = null;

                if (!groupOpt.isPresent()) {
                    group = new Group(groupName, groupJson);
                    deviceGroups.add(group);
                } else {
                    group = groupOpt.get();
                    group.setData(groupJson);
                }

                if (!groupJson.isEmpty()) {
                    String[] entityNames = JSONObject.getNames(groupJson);
                    Set<Entity> groupEntities = group.getEntities();

                    for (String entityName : entityNames) {
                        Optional<Entity> entityOpt = Optional
                                .ofNullable(getItemByName(groupEntities,
                                        entityName));
                        JSONObject entityJson = groupJson
                                .getJSONObject(entityName);

                        Entity entity = null;
                        
                        boolean hasItems = entityJson.has(SensorItem.SENSOR_ITEMS_KEY);

                        if (!entityOpt.isPresent()) {
                            entity = new Entity( entityName, entityJson);
                            groupEntities.add(entity);
                        } else {
                            entity = entityOpt.get();
                            entity.setData(entityJson);
                            if (!entity.hasSensorItems()
                                    && entity.getSensorItems() != null
                                    && hasItems) {
                                entity.setSensorItems(new HashSet<>());
                            }
                        }
                        
                        if(hasItems){
                            JSONObject sensorItemsJson = entityJson.getJSONObject(SensorItem.SENSOR_ITEMS_KEY);
                            
                            Set<SensorItem> sensorItems = entity.getSensorItems();
                            String[] sensorItemNames = JSONObject.getNames(sensorItemsJson);
                            
                            for(String siName: sensorItemNames){
                                Optional<SensorItem> siOpt = Optional
                                        .ofNullable(getItemByName(sensorItems,
                                                siName));
                                
                                SensorItem si = null;
                                JSONObject siJson = sensorItemsJson.getJSONObject(siName);
                                
                                if(!siOpt.isPresent()){
                                    si = new SensorItem(siName, siJson);
                                    sensorItems.add(si);
                                }else{
                                    si = siOpt.get();
                                    si.setData(sensorItemsJson.getJSONObject(siName));
                                }
                            }
                        }
                    }
                }
            }
        }
        log.info("Json parsed to model");
        return false;
    }

    @Override
    public Set<Group> getGroups(String deviceId) {
        if (model.containsKey(deviceId)) {
            return model.get(deviceId);
        }
        return null;
    }

    @Override
    public Optional<Group> getGroupByName(String deviceId, String name) {

        Set<Group> groups = getGroups(deviceId);
        Group group = (groups != null) ? getItemByName(groups, name) : null;

        Optional<Group> result = Optional.ofNullable(group);

        return result;
    }

    public static <T extends NameAble> T getItemByName(Set<T> items, String name) {
        for (T item : items) {
            if (item.getName().equals(name)) {
                return item;
            }
        }
        return null;
    }

}
