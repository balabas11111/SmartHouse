package com.balabas.smarthouse.server.entity.model.descriptor;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EMPTY_STR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_EDITORS;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_LEVEL;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_PERSISTOR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_PUSH_VALUE_FROM_SERVER;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_SUBSTANCE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_TYPE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_CLASS_VIEV;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.ENTITY_FIELD_NAME;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.FALSE;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.HIGH;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.TRUE;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.EntityAbstract;
import com.balabas.smarthouse.server.exception.BadDataException;

import lombok.Builder;
import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Builder
public class EntityDescriptor extends EntityAbstract implements IEntityDescriptor {

	@Getter
	private EntityClassType entityClassType;
	@Getter
	private EntityClassEditors entityClassEditor;
	@Getter
	private EntityClassSubstance entityClassSubstance;
	@Getter
	private EntityClassLevel entityClassLevel;
	@Getter
	private EntityClassPersistor entityClassPersistor;
	@Getter
	private EntityClassView entityClassView;
	@Getter
	private Long timeToLive;
	@Getter
	private boolean pushFromServer;
	
	protected boolean validate() {
		
		return true;
	}
	
	public static EntityDescriptor fromJsonStr(String dataStr) throws BadDataException {
		EntityDescriptor result = null;		
		try {
		
			JSONObject dJson = new JSONObject(dataStr);
			
			if (dJson.length() == 0) {
				throw new BadDataException("Empty Entity descriptor :", dataStr);
			}
			
			result = EntityDescriptor.builder()
					.entityClassEditor(EntityClassEditors.from(dJson.optString(ENTITY_CLASS_EDITORS, null)))
					.entityClassLevel(EntityClassLevel.from(dJson.optString(ENTITY_CLASS_LEVEL, null)))
					.entityClassPersistor(EntityClassPersistor.from(dJson.optString(ENTITY_CLASS_PERSISTOR, null)))
					.entityClassSubstance(EntityClassSubstance.from(dJson.optString(ENTITY_CLASS_SUBSTANCE, null)))
					.entityClassType(EntityClassType.from(dJson.optString(ENTITY_CLASS_TYPE, null)))
					.entityClassView(EntityClassView.from(dJson.optString(ENTITY_CLASS_VIEV, null)))
					
					.pushFromServer( booleanFromString(dJson.optString(ENTITY_CLASS_PUSH_VALUE_FROM_SERVER, FALSE)))
					.build();

			result.setDescription(dJson.optString(ENTITY_FIELD_DESCRIPTION, EMPTY_STR));
			result.setName(dJson.optString(ENTITY_FIELD_NAME, EMPTY_STR));
			
			return result;
		
		}catch(Throwable e) {
			log.error(e);
		}
		
		if (result == null || !result.validate()) {
			throw new BadDataException("Bad Entity descriptor :", dataStr);
		}
		
		return result;
	}
	
	public static boolean booleanFromString(String value) {
		if (TRUE.equalsIgnoreCase(value)) {
			return true;
		}
		if (HIGH.equalsIgnoreCase(value)) {
			return true;
		}
		if (value != null && !value.isEmpty() ) {
			if(value.startsWith(HIGH)) {
				return true;
			}
		}
		return false;
	}
	
}
