package com.balabas.smarthouse.server.entity.service;

import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.repository.IGroupRepository;
import com.balabas.smarthouse.server.view.Action;

@Service
public class GroupService implements IGroupService {

	@Autowired
	private IEntityService entityService;
	
	@Autowired
	private IGroupRepository groupRepository;

	@Override
	public List<Action> getActionsForGroup(String actionName, IGroup group) {
		if (group == null || group.getEntities().isEmpty()) {
			return Collections.emptyList();
		}

		return group.getEntities().stream().flatMap(
				entity -> entityService.getActionsForEntity(actionName, entity).stream())
				.collect(Collectors.toList());
	}

	@Override
	public void deleteGroupsForDevice(Long deviceId) {
		groupRepository.deleteGroupByDeviceId(deviceId);
	}

	@Override
	public List<IGroup> loadVirtualized() {
		return groupRepository.loadVirtualized();
	}

	@Override
	public IGroup loadGroupById(Long id) {
		return groupRepository.loadGroupById(id);
	}
	
}
