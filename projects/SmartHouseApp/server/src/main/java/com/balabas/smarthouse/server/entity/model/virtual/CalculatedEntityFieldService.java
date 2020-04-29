package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.repository.ICalculatedEntityFieldRepository;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.balabas.smarthouse.server.util.DateTimeUtil;

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
	private List<ICalculatedEntityField> calculatedEntityFields = new ArrayList<ICalculatedEntityField>();

	@Getter
	private IEntityFieldService entityFieldService;

	@Value("${smarthouse.server.entityfields.recalculate.interval.ms:10000}")
	private Long recalculateInterval;

	@PostConstruct
	@Transactional
	public void afterPropertiesSet() throws Exception {

		calculatedEntityFieldRepository.findAll().forEach(d -> {
			calculatedEntityFields.add(d);
		});

		log.info("---Loaded persisted calculatedEntityField---");
	}

	@Override
	public void dispatchEntityFieldValueChange(IEntityField sourceEntityField,
			Map<Long, IEntityField> uniqueChangedTargets) {
		List<ICalculatedEntityField> calcFields = getCalculatedEntityFieldsBySourceField(sourceEntityField);

		// List<IEntityField> changedFields = Lists.newArrayList();

		if (calcFields.size() > 0) {
			// log.info("Total calcFields " + calcFields.size());

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
					cef.apply(sourceEntityField);

					IEntityField resultField = deviceService.getEntityFieldById(targetField.getId());
					if (resultField != targetField) {
						resultField.setValueStr(targetField.getValueStr());
					}

					if (resultField != null && resultField.isVirtualized()) {
						IEntity entity = resultField.getEntity();
						if (entity != null) {
							IDevice targetDevice = entity.getDevice();
							if (targetDevice != null && targetDevice.isVirtualized()) {
								targetDevice.setDeviceLastUpdateTimeNow();
							}
						}
					}
					uniqueChangedTargets.put(resultField.getId(), resultField);
					// changedFields.add(resultField);

				} catch (BadValueException e) {
					e.printStackTrace();
				}
			}
		}
	}

	@Override
	public List<ICalculatedEntityField> getCalculatedEntityFieldsBySourceField(IEntityField sourceEntityField) {
		return calculatedEntityFields.stream().filter(coll -> coll.isImpacted(sourceEntityField))
				.collect(Collectors.toList());
	}

	@Override
	public ICalculatedEntityField getCalculatedEntityField(Long id) {
		return calculatedEntityFields.stream().filter(cef -> id.equals(cef.getId())).findFirst().orElse(null);
	}

	@Override
	public Set<ICalculatedEntityFieldCalculator> getCalculators() {
		return entityFieldCalculatorService.getCalculators();
	}

	@Override
	public Set<IEntityField> getAllSourceFields() {
		return deviceService.getCurrentEntityFields(ef -> !ef.isVirtualized());
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

		result.setCalculator(entityFieldCalculatorService.getDefaultCalculator());

		return result;
	}

	@Override
	public ICalculatedEntityFieldCalculator getCalculator(String name) {
		return entityFieldCalculatorService.getCalculator(name);
	}

	@Override
	public void attachCalculator(ICalculatedEntityField calcEntityField) {
		if (calcEntityField.getCalculator() == null) {
			calcEntityField.setCalculator(this.getCalculator(calcEntityField.getCalculatorName()));
		}
	}

	@Override
	public IEntityFieldValue getValue(ICalculatedEntityField calcEntityField,
			Map<String, IEntityFieldValue> entFieldMap) {
		attachCalculator(calcEntityField);

		return calcEntityField.apply(entFieldMap);
	}

	@Override
	public void save(ICalculatedEntityField calcEntityField) {
		ICalculatedEntityField result = calculatedEntityFieldRepository.save((CalculatedEntityField) calcEntityField);

		int index = deviceService.getItemIndexById(calculatedEntityFields, result.getId());

		if (index > -1) {
			calculatedEntityFields.set(index, result);
		} else {
			calculatedEntityFields.add(result);
		}
	}

	@Override
	public ICalculatedEntityField getCalculatedEntityFieldById(Long id) {
		return calculatedEntityFields.stream().filter(cef -> cef.getId().equals(id)).findFirst().orElse(null);
	}

	@Override
	public void generateAllValues(CalculatedEntityField calcEntityField) {
		log.info("Calculated field values generation started");
		Date date1 = DateTimeUtil.getDate();

		Set<IEntityField> source = calcEntityField.getSourceEntityFields();

		Date oldest = null;

		for (IEntityField entityField : source) {
			Date dateTmp = entityFieldService.getOldestEntityFieldValue(entityField);

			if (oldest == null || dateTmp.getTime() > oldest.getTime()) {
				oldest = dateTmp;
			}

		}

		long now = DateTimeUtil.now();
		long current = oldest.getTime();

		long totalSaved = 0;

		while (current < now) {
			IEntityFieldValue value = getValue(calcEntityField,
					entityFieldService.getNearestValueToDate(source, current));

			if (value != null) {
				value.setDate(new Date(current));
				totalSaved += entityFieldService.saveIfDifferent(value);
			}

			current += recalculateInterval;
		}

		log.info("Calc field created values count=" + totalSaved + " in time secs=" + DateTimeUtil.getDiffSecs(date1));
	}

	@Override
	public void delete(Long id) {
		if(id==null) {
			return;
		}
		
		ICalculatedEntityField calcField = getCalculatedEntityField(id);
		calculatedEntityFields.remove(calcField);
		
		calculatedEntityFieldRepository.delete((CalculatedEntityField)calcField);
	}

}
