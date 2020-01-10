package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.ICalculatedEntityFieldRepository;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.google.common.collect.Lists;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
@SuppressWarnings("rawtypes")
public class CalculatedEntityFieldService implements ICalculatedEntityFieldService {

	@Autowired
	private IEntityFieldCalculatorService entityFieldCalculatorService;

	@Autowired
	private IDeviceManageService deviceService;
	
	@Autowired
	private ICalculatedEntityFieldRepository calculatedEntityFieldRepository;

	@Getter
	List<ICalculatedEntityField> calculatedEntityFields = new ArrayList<ICalculatedEntityField>();

	@Override
	public void apply(IEntityField sourceEntityField) {
		List<ICalculatedEntityField> calcFields = getCalculatedEntityFieldsBySourceField(sourceEntityField);

		List<IEntityField> changedFields = Lists.newArrayList();

		for (ICalculatedEntityField cef : calcFields) {
			if (cef.getCalculator() == null) {
				cef.setCalculator(entityFieldCalculatorService.getCalculator(cef.getCalculatorName()));
			}
			IEntityField targetField = deviceService.getEntityFieldById(cef.getTargetEntityField().getId());

			cef.setTargetEntityField(targetField);

			Set<IEntityField> fields = new LinkedHashSet<IEntityField>();
			for (IEntityField f : cef.getSourceEntityFields()) {
				fields.add(deviceService.getEntityFieldById(f.getId()));

			}

			cef.setSourceEntityFields(fields);

			try {
				Object valueOld = sourceEntityField.getValue();
				cef.apply(sourceEntityField);

				boolean changed = false;

				if (valueOld != null && valueOld.equals(sourceEntityField.getValue())) {
					changed = false;
				} else if (sourceEntityField.getValue() != null) {
					changed = sourceEntityField.getValue().equals(valueOld);
				} else if (valueOld != null) {
					changed = valueOld.equals(sourceEntityField.getValue());
				}

				if (changed) {
					IEntityField resultField = deviceService.getEntityFieldById(targetField.getId());
					if (resultField != targetField) {
						resultField.setValueStr(targetField.getValueStr());
					}
					changedFields.add(resultField);
				}

			} catch (BadValueException e) {
				e.printStackTrace();
			}
		}
		log.info("total calculatedFields changed =" + changedFields.size());
		deviceService.saveEntityFieldValues(changedFields);
	}

	@Override
	public List<ICalculatedEntityField> getCalculatedEntityFieldsBySourceField(IEntityField sourceEntityField) {
		return calculatedEntityFields.stream().filter(coll -> coll.isImpacted(sourceEntityField))
				.collect(Collectors.toList());
	}

	@Override
	public ICalculatedEntityField getCalculatedEntityField(Long id) {
		return calculatedEntityFields.stream().filter( cef -> id.equals(cef.getId())).findFirst().orElse(null);
	}

	@Override
	public Set<ICalculatedEntityFieldCalculator> getCalculators() {
		return entityFieldCalculatorService.getCalculators();
	}

	@Override
	public Set<IEntityField> getAllSourceFields() {
		return deviceService.getEntityFields(ef -> !ef.isVirtualized());
	}
	
	@Override
	public Set<IEntityField> getAllTargetFields() {
		return deviceService.getEntityFields(IItemAbstract::isVirtualized);
	}

	@Override
	public ICalculatedEntityField createNewCalculatedEntityField(String name, String description) {
		ICalculatedEntityField result = new CalculatedEntityField();
		
		result.setName(name);
		result.setDescription(description);
		
		return result;
	}

	@Override
	public ICalculatedEntityFieldCalculator getCalculator(String name) {
		return entityFieldCalculatorService.getCalculator(name);
	}

	@Override
	public void save(ICalculatedEntityField calcEntityField) {
		ICalculatedEntityField result = calculatedEntityFieldRepository.save((CalculatedEntityField)calcEntityField);
		
		int index = deviceService.getItemIndexById(calculatedEntityFields, result.getId());
			
		if(index>-1) {
			calculatedEntityFields.set(index, result);
		} else {
			calculatedEntityFields.add(result);
		}
	}

	@Override
	public ICalculatedEntityField getCalculatedEntityFieldById(Long id) {
		return calculatedEntityFields.stream().filter(cef -> cef.getId().equals(id))
				.findFirst().orElse(null);
	}
	
}
