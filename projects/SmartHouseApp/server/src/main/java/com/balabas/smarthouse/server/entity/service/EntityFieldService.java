package com.balabas.smarthouse.server.entity.service;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import javax.persistence.EntityManager;
import javax.persistence.Query;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValueBoolean;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueNumber;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldEnabledValueRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldIncorrectValueRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldValueRepository;
import com.balabas.smarthouse.server.util.DateTimeUtil;
import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Service
@SuppressWarnings({ "rawtypes", "unchecked" })
@Log4j2
public class EntityFieldService implements IEntityFieldService {

	@Getter
	@Value("${smarthouse.server.fields.data.changed.if.diff:0.001}")
	private Float minAllowedDiff;
	
	@Autowired
	IEntityFieldRepository entityFieldRepository;
	
	@Autowired
	IEntityFieldValueRepository entityFieldValueRepository;

	@Autowired
	IEntityFieldEnabledValueRepository entityFieldEnabledValueRepository;

	@Autowired
	IEntityFieldIncorrectValueRepository entityFieldIncorrectValueRepository;
	
	@Autowired
	EntityManager entityManager;

	private static class DateContainer {
		private Date start;
		private Date end;
		
		public DateContainer(Date date1, Date date2) {
			super();
			if(date2.getTime() < date1.getTime()) {
				start = date2;
				end = date1;
			} else {
				start = date1;
				end= date2;
			}
		}
	}
	
	@Override
	public void save(IEntityFieldValue entityFieldValue) {
		if (entityFieldValue.getEntityField() != null && entityFieldValue.getEntityField().getId() != null) {
			try {
				entityFieldValueRepository.saveEntityFieldValue(entityFieldValue);
			} catch (Exception e) {
				log.error(e);
			}
		} else {
			log.error("Error save entityFieldValue id  is null name = "
					+ Optional.ofNullable(entityFieldValue.getEntityField().getName()).orElse("null"));
		}
	}
	
	@Override
	public void saveAll(List<IEntityFieldValue> values) {
		values.stream().forEach(this::save);
	}
	
	@Override
	public List<IEntityFieldValue> getEntityFieldValuesByDateRange(IEntityField entityField, Date date1, Date date2) {
		DateContainer cont = new DateContainer(date1, date2);
		return entityFieldValueRepository.getEntityFieldValuesByDateRange(entityField.getId(), cont.start, cont.end);
	}
	
	@Override
	public List<IEntityFieldValue> getEntityFieldValuesLessThanDates(IEntityField entityField, Date date1, Date date2) {
		DateContainer cont = new DateContainer(date1, date2);
		List<IEntityFieldValue> result = new ArrayList<IEntityFieldValue>();
		
		result.add(getEntityFieldValuesLessThanDate(entityField, cont.start));
		result.add(getEntityFieldValuesLessThanDate(entityField, cont.end));
		
		return result;
	}
	
	@Override
	public IEntityFieldValue getEntityFieldValuesLessThanDate(IEntityField entityField, Date date) {
		Query query = entityManager.createQuery("SELECT efv FROM EntityFieldValue efv WHERE efv.entityField.id=:id AND efv.date <= :date order by date desc");
		
		query.setParameter("id", entityField.getId());
		query.setParameter("date", date);
		
		query.setFirstResult(0);
		query.setMaxResults(1);
		
		return (IEntityFieldValue) query.getSingleResult();
	}
	
	@Override
	public Boolean isEntityFieldValuesListGrows(List<IEntityFieldValue> values) {
		if (values == null || values.size()<=1) {
			return null;
		}
		Float first = ((Number) values.get(0).getValue()).floatValue();
		Float last = ((Number) values.get(values.size()-1).getValue()).floatValue();
		
		if(!isValueChanged(last, first)) {
			return null;
		}
		
		return last > first;
	}

	@Override
	public List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField) {
		if (!Boolean.class.equals(entityField.getClazz()) || entityField.getEnabledValues() == null
				|| entityField.getEnabledValues().isEmpty() || entityField.isReadOnly() || !entityField.getEntity().getDevice().isInOkState()) {
			return Collections.emptyList();
		}

		IEntityFieldEnabledValue currentValue = entityField.getEntityFieldEnabledValueByCurrentValue();

		Set<IEntityFieldEnabledValue> evals = entityField.getEnabledValues();

		List<IEntityFieldEnabledValue> result = evals.stream()
				.filter(ev -> ev.getViewClass() != null && ev.getViewClass().isButton()
						&& (currentValue == null || (currentValue != null && ev.getClass().equals(currentValue.getClass()) && !ev.getId().equals(currentValue.getId()))))
				.collect(Collectors.toList());

		return result;
	}

	@Override
	public boolean isButton(IEntityField entityField) {
		IEntityFieldEnabledValue enVal = entityField.getEntityFieldEnabledValueByCurrentValue();

		return enVal != null && enVal.getViewClass() != null && Boolean.class.equals(entityField.getClazz())
				&& entityField.getViewClass().isButton();
	}

	@Override
	public List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId) {
		return entityFieldValueRepository.getEntityFieldValuesForEntityField(entityFieldId);
	}
	
	@Override
	public List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId, Date afterDate) {
		return entityFieldValueRepository.getEntityFieldValuesForEntityField(entityFieldId, afterDate);
	}
	
	@Override
	public List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId, Date afterDate, Date beforeDate) {
		return entityFieldValueRepository.getEntityFieldValuesForEntityField(entityFieldId, afterDate, beforeDate);
	}
	
	@Override
	public List<EntityFieldValue> getLastEntityFieldValuesForDevice(Long deviceId) {
		return entityFieldValueRepository.getLastEntityFieldValuesForDevice(deviceId);
	}

	@Override
	public List<EntityFieldValue> getLastEntityFieldValuesForEntity(Long entityId) {
		return entityFieldValueRepository.getLastEntityFieldValuesForEntity(entityId);
	}

	@Override
	public void deleteEntityFieldValuesForDevice(Long deviceId) {
		entityFieldValueRepository.deleteEntityFieldValuesForDevice(deviceId);
	}
	
	@Override
	public void deleteEntityFieldsForDevice(Long deviceId) {
		deleteEntityFieldValuesForDevice(deviceId);
		entityFieldEnabledValueRepository.deleteEntityFieldEnabledValuesForDevice(deviceId);
		entityFieldIncorrectValueRepository.deleteEntityFieldIncorrectValue(deviceId);
		entityFieldRepository.deleteEntityFieldsByDeviceId(deviceId);
	}
	
	@Override
	public Date selectMinEntityFieldValueDate(Long entityFieldId) {
		return entityFieldValueRepository.selectMinEntityFieldValueDate(entityFieldId);
	}
	
	@Override
	public Date selectMaxEntityFieldValueDate(Long entityFieldId) {
		return entityFieldValueRepository.selectMaxEntityFieldValueDate(entityFieldId);
	}

	@Override
	public Optional<IEntityField> getEntityFieldById(Long entityFieldId) {
		IEntityField result = entityFieldRepository.getEntityFieldbyId(entityFieldId);
		return Optional.ofNullable(result);
	}

	@Override
	public void saveAllValues(Collection<IEntityField> values) {
		saveAll(mapFieldToValue(values));
	}

	@Override
	public IEntityFieldValue getEntityFieldValue(IEntityField entityField) {
		IEntityFieldValue result = null; 
		
		if (Number.class.isAssignableFrom(entityField.getClazz())) {
		    result = new EntityFieldValueNumber(entityField, (Float)entityField.getValue());
		} else if (Boolean.class.isAssignableFrom(entityField.getClazz())) {
			result = new EntityFieldValueBoolean(entityField, (Boolean)entityField.getValue());
		}
		
		return result;
	}

	@Override
	public List<IEntityFieldValue> mapFieldToValue(Collection<IEntityField> changedSources) {
		return changedSources.stream().map(this::getEntityFieldValue).filter(t -> t!=null).collect(Collectors.toList());
	}

	@Override
	public List<Action> getActionsForEntityField(String actionName, IEntityField entityField) {
		boolean isVirtual = entityField.isVirtualized();
		return getCommandsForEntityField(entityField).stream()
				.map(ef -> Action.fromEntityFieldEnabledValue(actionName, ef, isVirtual)).collect(Collectors.toList());
	}

	@Override
	public Action getActionForEntityFieldBoolean(String actionName, IEntityField<Boolean> entityField,
			boolean requiredState) {
		boolean isVirtual = entityField.isVirtualized();
		List<IEntityFieldEnabledValue> enVals = getCommandsForEntityField(entityField);
		
		for(IEntityFieldEnabledValue value : enVals) {
			if(Boolean.class.equals(value.getClazz())) {
				EntityFieldEnabledValueBoolean valB = (EntityFieldEnabledValueBoolean) value;
				if(valB!=null && valB.getValue()!=null && valB.getValue().booleanValue() == requiredState) {
					return Action.fromEntityFieldEnabledValue(actionName, valB, isVirtual);
				}
			}
		}
		return null;
	}
	
	@Override
	public void insertEntityFieldIncorrectValue(IEntityField entityField, String value) {
		
		if(entityField !=null) {
			entityFieldIncorrectValueRepository.insertEntityFieldIncorrectValue(entityField.getId(), value, new Date());
			log.debug(entityField.getEntity().getDevice().getName() +" " + entityField.getEntity().getName() + " " +entityField.getName() + " " +entityField.getDescription() + " INCORRECT value " +value);
		} else {
			log.error("UNKNOWN wrong value for NULL entity field");
		}
	}

	@Override
	public boolean isValueChanged(Float val1, Float val2) {
		return Math.abs(val1 - val2) > getMinAllowedDiff();
	}

	@Override
	public IEntityFieldValue getNearestValueToDate(Set<IEntityField> entityFields, long current) {
		for(IEntityField entityField : entityFields) {
			Date date = new Date(current);
			
			EntityFieldValue older = entityFieldValueRepository.getEntityFieldValueForEntityFieldOlderThanDate(entityField.getId(), date); 
			EntityFieldValue newer = entityFieldValueRepository.getEntityFieldValueForEntityFieldNewerThanDate(entityField.getId(), date);

			long olderDiff = Math.abs(older.getDate().getTime() - date.getTime());
			long newerDiff = Math.abs(older.getDate().getTime() - date.getTime());
			
			return olderDiff < newerDiff ? older :newer;
			}
		return null;
	}

	@Override
	public Date getOldestEntityFieldValue(IEntityField entityField) {
		IEntityFieldValue older = entityFieldValueRepository.getEntityFieldValueForEntityFieldOlderThanDate(entityField.getId(), DateTimeUtil.getDate());
		if(older == null) {
			return DateTimeUtil.getDate();
		}
		return older.getDate();
	}

	@Override
	public long saveIfDifferent(IEntityFieldValue value) {
		IEntityFieldValue prevValue = getEntityFieldValuesLessThanDate(value.getEntityField(), value.getDate());
		
		if (isValueChanged(value.getValueAsFloat(), prevValue.getValueAsFloat()) ) {
			save(value);
			return 1;
		}
		
		return 0;
	}

}
