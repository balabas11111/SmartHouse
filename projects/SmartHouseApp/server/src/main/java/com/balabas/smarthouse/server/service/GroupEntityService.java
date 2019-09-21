package com.balabas.smarthouse.server.service;

import java.util.Optional;
import java.util.Set;

import org.json.JSONObject;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;

public interface GroupEntityService {

    boolean parseJsonToModel(Device device, JSONObject json);
    
    Set<Group> getGroups(String deviceId);
    
    Optional<Group> getGroupByName(String deviceId, String name);
    
}
