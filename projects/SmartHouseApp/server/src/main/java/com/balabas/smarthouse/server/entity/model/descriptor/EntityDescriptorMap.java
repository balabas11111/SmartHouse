package com.balabas.smarthouse.server.entity.model.descriptor;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_ENTITY_FIELDS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ID;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_NAME;


import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

import org.json.JSONObject;

import lombok.AllArgsConstructor;
import lombok.Getter;

@AllArgsConstructor
public class EntityDescriptorMap implements IEntityDescriptorMap {

	@Getter
	private int remoteId;
	@Getter
	private String name;
	@Getter
	private Map<String, IEntityDescriptor> descriptors;
	
	public static IEntityDescriptorMap fromJson(JSONObject json, String nameOver) {
		if(json == null || json.isEmpty()) {
			return null;
		}
		Map<String, IEntityDescriptor> descrs = new HashMap<>();
		
		Optional.ofNullable(json.optJSONObject(EDC_ENTITY_FIELDS))
		.ifPresent( fieldsJson -> descrs.putAll(EntityDescriptor.fromJson(fieldsJson)));
		
		int id = json.optInt(EDC_FIELD_ID, -1);
		String name = json.optString(EDC_FIELD_NAME, nameOver);
		
		if(id<0 || descrs.isEmpty()) {
			return null;
		}
		
		return new EntityDescriptorMap(id, name, descrs);
	}
}
