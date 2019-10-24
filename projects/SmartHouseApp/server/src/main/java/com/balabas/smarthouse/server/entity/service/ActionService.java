package com.balabas.smarthouse.server.entity.service;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IEntityRepository;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.view.Action;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.view.Action.ID_TYPE_DEVICE;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENABLED_VALUE;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_ENTITY_FIELD;
import static com.balabas.smarthouse.server.view.Action.ID_TYPE_GROUP;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView.EDC_CLASS_VIEW_INPUT;
import static com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView.EDC_CLASS_VIEW_LABEL;

import java.util.Map;

import javax.transaction.Transactional;

@Service
@Log4j2
public class ActionService implements IActionService {

	@Autowired
	IDeviceManageService deviceService;

	@Autowired
	IEntityRepository entityRepository;

	@Override
	public Action getDeviceGroupEntityNameActionByIdAction(Action action) {
		if (StringUtils.isEmpty(action.getIdType())) {
			if (!StringUtils.isEmpty(action.getEntityName()) && action.getTargetId() == null) {
				action.setTargetId(deviceService.getManagedDeviceByName(action.getDeviceName())
						.getEntity(action.getEntityName()).getId());
			}

			return action;
		}

		String deviceName = "";
		String groupName = "";
		String entityName = "";

		Long targetId = action.getTargetId();

		if (targetId == null) {
			throw new IllegalArgumentException("Empty action id");
		}

		switch (action.getIdType()) {
		case ID_TYPE_DEVICE:
			deviceName = deviceService.getDeviceById(targetId).getName();
			break;
		case ID_TYPE_GROUP:
			Group group = deviceService.getGroupById(targetId);

			deviceName = group.getDevice().getName();
			groupName = group.getName();
			break;
		case ID_TYPE_ENTITY:
			Entity entity = deviceService.getEntityById(targetId);

			deviceName = entity.getGroup().getDevice().getName();
			groupName = entity.getGroup().getName();
			entityName = entity.getName();
		case ID_TYPE_ENTITY_FIELD:
			IEntityField<?> entityField = deviceService.getEntityFieldById(targetId);

			deviceName = entityField.getEntity().getGroup().getDevice().getName();
			groupName = entityField.getEntity().getGroup().getName();
			entityName = entityField.getEntity().getName();
			break;
		case ID_TYPE_ENABLED_VALUE:
			IEntityFieldEnabledValue<?> entityFieldEnabledValue = deviceService.getEntityFieldEnabledValueById(targetId);

			deviceName = entityFieldEnabledValue.getEntityField().getEntity().getGroup().getDevice().getName();
			groupName = entityFieldEnabledValue.getEntityField().getEntity().getGroup().getName();
			entityName = entityFieldEnabledValue.getEntityField().getEntity().getName();
			break;
		default:
			throw new IllegalArgumentException("unknown id type");

		}

		if (StringUtils.isEmpty(deviceName) && StringUtils.isEmpty(groupName) && StringUtils.isEmpty(entityName)) {
			throw new IllegalArgumentException("required values empty");
		}

		Action result = Action.builder().action(action.getAction()).description(action.getDescription())
				.data(action.getData()).deviceName(deviceName).groupName(groupName).entityName(entityName)
				.targetId(action.getTargetId()).build();

		result.rebuildCallbackData();

		return result;
	}

	@Override
	@Transactional
	public boolean executeDeviceAction(Action action) throws ResourceNotFoundException {
		log.debug("Action received :" + action.getCallbackData());
		action = getDeviceGroupEntityNameActionByIdAction(action);

		Map<String, Object> params = (new JSONObject(action.getData())).toMap();

		Entity entity = deviceService.getEntityById(action.getTargetId());

		entity.getEntityFields().stream().forEach(ef -> {
			if (params.containsKey(ef.getName()) && !ef.isReadOnly() && ef.getClazz().equals(String.class)
					&& (EDC_CLASS_VIEW_LABEL.equals(ef.getViewClass())
							|| EDC_CLASS_VIEW_INPUT.equals(ef.getViewClass()))) {
				// field is string and should be saved on server. No need to send it to device

				try {
					Object val = params.get(ef.getName());
					ef.setValueStr(val.toString());

					if (ef.getName().equals(entity.getDescriptionField())) {
						entity.setDescription(val.toString());
					}

					params.remove(ef.getName());
				} catch (Exception e) {
					log.error(e);
					throw new IllegalArgumentException("Bad value field " + ef.getName());
				}
			}
		});

		if (params.size() > 0) {
			deviceService.sendDataToDevice(action.getDeviceName(), action.getGroupName(), action.getEntityName(), params);
			return true;
		}

		return false;
	}

}
