package com.balabas.smarthouse.server.view.chart;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;

@SuppressWarnings("rawtypes")
public interface IMetrics extends IItemAbstract {

	Set<IEntityField> getEntityFields();
	void setEntityFields(Set<IEntityField> entityFields);
	
	@SuppressWarnings("unchecked")
	default Map<String, List<IEntityField>> getEntityFieldsAsMap(){
	List<IEntityField> entityFieldsTmp = new ArrayList(getEntityFields());
	List<IEntityField> entityFields = entityFieldsTmp 
	.stream().sorted(
			(e1, e2) -> { 
				String str1 = e1.getEmojiDescriptionByDescriptionFieldWithParent(); 
				String str2 = e2.getEmojiDescriptionByDescriptionFieldWithParent();
				return str1.compareTo(str2);
			})
			.collect(Collectors.toList());
	
	Map<String, List<IEntityField>> map = getEntityFieldsAsMapByParentDescription(entityFields);
	return map;
	}
	
	default Map<String, List<IEntityField>> getEntityFieldsAsMapByParentDescription(List<IEntityField> entityFields) {
		Map<String, List<IEntityField>> result = Maps.newLinkedHashMap();
		
		for(IEntityField field : entityFields) {
			String descr = field.getEntity().getEmojiDescriptionByDescriptionField();
			
			if(!result.containsKey(descr)) {
				result.put(descr, Lists.newArrayList());
			}
			
			result.get(descr).add(field);
		}
		
		return result;
	}
}
