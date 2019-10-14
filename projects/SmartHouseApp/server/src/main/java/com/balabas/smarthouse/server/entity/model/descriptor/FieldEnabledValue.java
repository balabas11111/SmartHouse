package com.balabas.smarthouse.server.entity.model.descriptor;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.ToString;
import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_ACTION_DESCR;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityClassConstants.EDC_FIELD_EMOJI;

import java.util.Map;

@Log4j2
@ToString
@AllArgsConstructor
public class FieldEnabledValue implements IFieldEnabledValue {

	@Getter
	private String key;
	
	@Getter
	private String description;
	
	@Getter
	private String actionDescription;
	
	@Getter
	private Emoji emoji;
	
	@SuppressWarnings("unchecked")
	public static IFieldEnabledValue fromJson(String key, Object source) {
		if(source == null) {
			return null;
		}
		try {
			Map<String,String> obj = (Map<String,String>) source;
			
			String d = obj.getOrDefault(EDC_FIELD_DESCRIPTION, null);
			String ad = obj.getOrDefault(EDC_FIELD_ACTION_DESCR, null);
			Emoji em = Emoji.getByCode(obj.getOrDefault(EDC_FIELD_EMOJI, null));
			
			return new FieldEnabledValue(key, d, ad, em);
		} catch(Exception e) {
			log.error(e);
			return null;
		}
	}
}
