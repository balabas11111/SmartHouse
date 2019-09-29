package com.balabas.smarthouse.server.service;

import java.util.List;

import org.json.JSONObject;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.Device;

public interface GroupEntityUpdateService {

    List<ChangedEvent<?>> parseJsonToModel(Device device, JSONObject json);
    
}
