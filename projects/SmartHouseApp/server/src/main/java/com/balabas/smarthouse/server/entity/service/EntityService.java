package com.balabas.smarthouse.server.entity.service;

import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IEntityRepository;
import com.balabas.smarthouse.server.util.SortingUtility;
import com.balabas.smarthouse.server.view.Action;

@Service
@SuppressWarnings("rawtypes")
public class EntityService implements IEntityService {

	@Autowired
	private IEntityFieldService entityFieldService;
	
	@Autowired
	private IEntityRepository entityRepository; 
	
	@Override
	public List<Action> getActionsForEntity(String actionName, IEntity entity) {
		return getEnabledEntityFieldWithCommandsForEntity(entity).stream()
				.map(ef -> Action.fromEntityFieldEnabledValue(actionName, ef)).collect(Collectors.toList());
	}
	
	@Override
	public List<IEntityFieldEnabledValue> getEnabledEntityFieldWithCommandsForEntity(IEntity entity) {
		List<IEntityField> entFields = SortingUtility.sort(entity.getEntityFields());

		if (entFields == null || entFields.isEmpty()) {
			return Collections.emptyList();
		}

		return entFields.stream().flatMap(ef -> entityFieldService.getCommandsForEntityFieldExceptCurrent(ef).stream()).collect(Collectors.toList());
	}

	@Override
	public void deleteEntitiesForDevice(Long deviceId) {
		entityRepository.deleteEntityByDeviceId(deviceId);
	}

	@Override
	public List<IEntity> loadVirtualized() {
		return entityRepository.loadVirtualized();
	}

	@Override
	public IEntity loadEntityById(Long id) {
		return entityRepository.loadEntityById(id);
	}

}
