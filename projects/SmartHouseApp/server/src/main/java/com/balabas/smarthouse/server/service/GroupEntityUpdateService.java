package com.balabas.smarthouse.server.service;

import java.util.List;
import java.util.Optional;
import java.util.Set;

import org.json.JSONObject;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;

public interface GroupEntityUpdateService {

    List<ChangedEvent<?>> parseJsonToModel(Device device, JSONObject json);
    
    Optional<Group> getGroupByName(Set<Group> groups, String name);
    
}
