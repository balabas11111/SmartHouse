package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.google.common.collect.Lists;

@Service
@SuppressWarnings("rawtypes")
public class DataCollectorService implements IDataCollectorService {

	@Autowired
	private IDataTransformerService transformerService;
	
	@Autowired
	private IDeviceManageService deviceService;
	
	List<IDataCollector> collectors = new ArrayList<IDataCollector>();
	
	@Override
	public void apply(IEntityField entityField) {
		List<IDataCollector> colls = getCollectors(entityField);
		
		List<IEntityField> changedFields = Lists.newArrayList();
		
		for( IDataCollector dc : colls) {
			if(dc.getTransformer()==null) {
				dc.setTransformer(transformerService.getTransformer(dc.getTransformerName()));
			}
			IEntityField targetField  = deviceService.getEntityFieldById(dc.getEntityField().getId());
			
			dc.setEntityField(targetField);
			
			Set<IEntityField> fields = new LinkedHashSet<IEntityField>();
			for(IEntityField f : dc.getEntityFields()) {
				fields.add(deviceService.getEntityFieldById(f.getId()));
				
			}
			
			dc.setEntityFields(fields);
			
			try {
				Object valueOld = entityField.getValue();
				dc.apply(entityField);
				
				boolean changed = false;
				
				if(valueOld!=null && valueOld.equals(entityField.getValue())) {
					changed = false;
				} else if(entityField.getValue()!=null){
					changed = entityField.getValue().equals(valueOld);
				} else if(valueOld!=null) {
					changed = valueOld.equals(entityField.getValue());
				}
				
				if(changed) {
					IEntityField resultField = deviceService.getEntityFieldById(targetField.getId());
					resultField.setValueStr(targetField.getValueStr());
					changedFields.add(resultField);
				}
				
			} catch (BadValueException e) {
				e.printStackTrace();
			}
		}
		
		deviceService.saveEntityFieldValues(changedFields);
	}

	@Override
	public List<IDataCollector> getCollectors(IEntityField entityField) {
		return collectors.stream().filter(coll -> coll.isImpacted(entityField)).collect(Collectors.toList());
	}

}
