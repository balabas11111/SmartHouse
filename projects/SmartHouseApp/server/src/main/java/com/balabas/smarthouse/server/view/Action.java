package com.balabas.smarthouse.server.view;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.json.JSONObject;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
@Builder
public class Action {

	public static final String CALLBACK_SPLITTER = ";";
	public static final String CALLBACK_TYPE_ACTION = "Act";

	public static final String ACTION_TYPE_UNKNOWN                  = "NUL";
	public static final String ACTION_TYPE_RESTART_APPLICATION      = "rea";
	public static final String ACTION_TYPE_VIEW_DEVICE_LIST         = "vdl";
	public static final String ACTION_TYPE_VIEW_HELP                = "vhp";
	public static final String ACTION_TYPE_SETUP                    = "ats";
	public static final String ACTION_TYPE_EDIT_ALARMS              = "aea";
	public static final String ACTION_TYPE_SELECT_DEV_EDIT_ALARMS   = "sea";
	public static final String ACTION_TYPE_EDIT_DEVICE_SELECT_LIST  = "eds";
	public static final String ACTION_TYPE_VIEW_GROUPS_OF_DEVICE    = "vgd";
	public static final String ACTION_TYPE_VIEW_ALL_DEVICES         = "vad";
	public static final String ACTION_TYPE_VIEW_ENTITIES_OF_GROUP   = "veg";
	public static final String ACTION_TYPE_EDIT_ENTITIES_OF_DEVICE  = "eeg";

	public static final String ACTION_TYPE_EDIT_ALARMS_OF_DEVICE         = "ead";
	public static final String ACTION_TYPE_EDIT_ALARMS_OF_ENTITY         = "eae";
	public static final String ACTION_TYPE_EDIT_ALARMS_OF_ENTITY_FIELD   = "aef";

	public static final String ACTION_TYPE_CREATE_ALARM_OF_ENTITY        = "cae";
	public static final String ACTION_ALARM_OF_ENTITY_CHANGE_ACTIVATION  = "dae";
	public static final String ACTION_ALARM_OF_ENTITY_CHANGE_SOUND       = "ecs";
	public static final String ACTION_TYPE_EDIT_ALARM_INTERVAL_OF_ENTITY = "eai";
	public static final String ACTION_TYPE_REMOVE_ALARM_INTERVAL_ENTITY  = "rai";
	public static final String ACTION_SAVE_ALARM_INTERVAL_OF_ENTITY      = "sAi";

	public static final String ACTION_DELETE_ENTITY_FIELD_ALARM          = "dfa";
	public static final String ACTION_EDIT_ENTITY_FIELD_ALARM            = "efa";
	public static final String ACTION_ADD_ENTITY_FIELD_ALARM             = "afa";
	public static final String ACTION_SAVE_NEW_ENTITY_FIELD_ALARM        = "nfa";
	public static final String ACTION_SAVE_ENTITY_FIELD_ALARM            = "sfa";

	public static final String ACTION_TYPE_EDIT_DEVICE_DESCRIPTION = "edd";
	public static final String ACTION_TYPE_EDIT_ENTITITY = "een";
	public static final String ACTION_TYPE_EDIT_ENTITITY_FIELD = "eef";
	public static final String ACTION_TYPE_VIEW_ENTITITY = "ven";
	public static final String ACTION_TYPE_SEND_DATA_TO_DEVICE = "sdd";
	public static final String ACTION_TYPE_SEND_DATA_TO_DEVICE_EDIT_FIELDS = "des";
	public static final String ACTION_TYPE_SAVE_DEVICE_PROPERTY = "sdp";
	public static final String ACTION_TYPE_GET_DATA_FROM_DEVICE = "gdd";

	public static final String ACTION_DATA_FIELD_NAME = "adfn";
	public static final String ACTION_DATA_FIELD_DESCRIPTION = "adfd";
	public static final String ACTION_DATA_FIELD_VALUE = "adfv";
	public static final String ACTION_DATA_FIELD_CLASS = "adfc";
	
	public static final String ID_TYPE_DEVICE = "_dev";
	public static final String ID_TYPE_GROUP = "_grp";
	public static final String ID_TYPE_ENTITY = "_ent";
	public static final String ID_TYPE_ENTITY_FIELD = "_fld";
	public static final String ID_TYPE_ENABLED_VALUE = "_val";
	public static final String ID_TYPE_ENTITY_ALARM = "_eal";
	public static final String ID_TYPE_ENTITY_ALARM_FIELD = "_eaf";

	public static final String validActions[] = { ACTION_TYPE_VIEW_DEVICE_LIST, ACTION_TYPE_EDIT_DEVICE_SELECT_LIST,
			ACTION_TYPE_VIEW_GROUPS_OF_DEVICE, ACTION_TYPE_VIEW_ENTITIES_OF_GROUP, ACTION_TYPE_VIEW_ENTITITY };
	@Getter
	private String action;
	@Getter
	@Setter
	private String description;
	@Getter
	private String data;
	@Getter
	private String deviceName;
	@Getter
	private String groupName;
	@Getter
	private String entityName;
	@Getter
	private String idType;
	@Getter
	@Setter
	private Long targetId;
	@Getter
	@Setter
	private Long targetId2;
	@Getter
	@Setter
	private String serverName;

	@Getter
	private String callbackData;

	@Getter
	private boolean valid;

	public Action() {
		this.valid = false;
	}

	public Action(String... cols) {
		this.action = getColValueOrNull(1, cols);
		this.data = getColValueOrNull(2, cols);
		this.deviceName = getColValueOrNull(3, cols);
		this.groupName = getColValueOrNull(4, cols);
		this.entityName = getColValueOrNull(5, cols);
		this.idType = getColValueOrNull(6, cols);

		String tId = getColValueOrNull(7, cols);
		if (!StringUtils.isEmpty(tId)) {
			this.targetId = Long.valueOf(tId);
		}

		this.callbackData = buildCallBackData();
		this.valid = isActionValid();
	}

	public Action(String action, String data, String deviceName, String groupName, String entityName) {

		this.action = action;
		this.deviceName = deviceName;
		this.groupName = groupName;
		this.entityName = entityName;
		this.data = data;

		this.callbackData = buildCallBackData();

		this.valid = isActionValid();
	}

	public Action(String action, String data, String description, String idType, Long targetId) {

		this.action = action;
		this.data = data;
		this.description = description;
		this.idType = idType;
		this.targetId = targetId;

		this.callbackData = buildCallBackData();

		this.valid = isActionValid();
	}
	
	public Action(String action, String data, String description, String idType, Long targetId, Long targetId2) {

		this.action = action;
		this.data = data;
		this.description = description;
		this.idType = idType;
		this.targetId = targetId;
		this.targetId2 = targetId2;

		this.callbackData = buildCallBackData();

		this.valid = isActionValid();
	}

	public void setData(String data) {
		this.data = clear(data);
		this.callbackData = buildCallBackData();
	}

	public void setDeviceName(String deviceId) {
		this.deviceName = clear(deviceId);
		this.callbackData = buildCallBackData();
	}

	public void setGroupName(String groupId) {
		this.groupName = clear(groupId);
		this.callbackData = buildCallBackData();
	}

	public void setEntityName(String entityId) {
		this.entityName = clear(entityId);
		this.callbackData = buildCallBackData();
	}

	public void setValueByKeyRemoveKey(String fieldValue) {
		setValueByKeyRemoveKey(ACTION_DATA_FIELD_NAME, fieldValue);
	}
	
	public void setValueByKeyRemoveKey(String keyToSelectAndRemove, String fieldValue) {
		JSONObject dataJson = new JSONObject(getData());

		String fieldName = dataJson.getString(keyToSelectAndRemove);
		dataJson.remove(keyToSelectAndRemove);

		dataJson.put(fieldName, fieldValue);

		this.setData(dataJson.toString());
	}
	
	public String getDataJsonFieldValue() {
		return getDataJsonField(ACTION_DATA_FIELD_VALUE);
	}
	
	public String getDataJsonField(String fieldName) {
		return new JSONObject(getData()).optString(fieldName, null);
	}
	
	public static String createActionDataFieldByFieldValue() {
		return createActionDataFieldByFieldValue(ACTION_DATA_FIELD_VALUE);
	}
	
	public static String createActionDataFieldByFieldValue(String fieldValue) {
		
		return new JSONObject()
				.put(ACTION_DATA_FIELD_NAME, fieldValue)
				.toString();
	}
	
	public static String createActionDataFieldByFieldValueAndClassName(String className) {
		return createActionDataFieldByFieldValueAndClassName(
				ACTION_DATA_FIELD_VALUE, className);
	}
	
	public static String createActionDataFieldByFieldValueAndClassName(
			String fieldValue, String className) {
		
		return new JSONObject()
				.put(ACTION_DATA_FIELD_NAME, fieldValue)
				.put(ACTION_DATA_FIELD_CLASS, className)
				.toString();
	}

	public static Action fromDevice(String action, IDevice device, String data) {
		return new Action(action, data, device.getName());
	}

	public static Action fromGroup(String action, IGroup group, String data) {
		return new Action(action, data, group.getDevice().getName(), group.getName());
	}

	public static Action fromEntity(String action, IEntity entity, String data) {
		return new Action(action, data, entity.getGroup().getDevice().getName(), entity.getGroup().getName(),
				entity.getName());
	}

	@SuppressWarnings("rawtypes")
	public static Action fromEntityFieldEnabledValue(String actionName,
			IEntityFieldEnabledValue entityFieldEnabledValue) {
		EntityField entityField = entityFieldEnabledValue.getEntityField();
		Entity entity = entityFieldEnabledValue.getEntityField().getEntity();

		String description = Optional.ofNullable(entityFieldEnabledValue.getEmoji()).orElse(Emoji.EMPTY_EMOJI)
				.toString() + entity.getDescription() + " : " + entityFieldEnabledValue.getActionDescription();

		return new Action(actionName, entityFieldEnabledValue.buildDataForCallBack(), description, ID_TYPE_ENTITY,
				entity.getId(), entityField.getId());
	}

	public static Action fromColumnList(String... cols) {
		List<String> list = new ArrayList<String>();

		if (cols == null || cols.length < 1 || !cols[0].equals(CALLBACK_TYPE_ACTION)) {
			list.add(CALLBACK_TYPE_ACTION);
		}
		if (cols != null) {
			for (String str : cols) {
				list.add(str);
			}
		}

		String[] strArr = list.toArray(new String[list.size()]);
		return new Action(strArr);
	}

	public static String callback(String... cols) {
		return (Action.fromColumnList(cols)).getCallbackData();
	}

	public static String callback(String actionName, String data, IDevice device) {
		return (Action.fromColumnList(actionName, data, device.getName())).getCallbackData();
	}

	public static String callback(String actionName, String data, IGroup group) {
		return (Action.fromColumnList(actionName, data, group.getDevice().getName(), group.getName()))
				.getCallbackData();
	}

	public static String callback(String actionName, String data, IEntity entity) {
		return (Action.fromColumnList(actionName, data, entity.getGroup().getDevice().getName(),
				entity.getGroup().getName(), entity.getName())).getCallbackData();
	}

	public static String callback(String actionName, IEntityAlarm entityAlarm) {
		return (Action.fromColumnList(actionName, entityAlarm.getId().toString(),
				entityAlarm.getWatchedItem().getDevice().getName(), entityAlarm.getWatchedItem().getGroup().getName(),
				entityAlarm.getWatchedItem().getName())).getCallbackData();
	}

	public static String callback(String actionName, String data, String actionType, Long targetId) {
		return (new Action(actionName, data, "", actionType, targetId)).getCallbackData();
	}

	@SuppressWarnings("rawtypes")
	public static String callback(String actionName, IEntityField entityField) {
		return Action.callback(actionName, entityField.getName(), entityField.getEntity());
	}

	@SuppressWarnings("rawtypes")
	public static String callback(String actionName, String data, IEntityField entityField) {
		return Action.callback(actionName, data, entityField.getEntity());
	}

	public static Action fromCallbackData(String str) {
		if (str == null || str.isEmpty()) {
			return new Action();
		}

		String[] col = str.split(CALLBACK_SPLITTER);

		if (col == null || col.length < 2 || !CALLBACK_TYPE_ACTION.equals(col[0])) {
			return new Action();
		}

		return new Action(col);
	}

	@Override
	public String toString() {
		return this.callbackData;
	}

	private String getColValueOrNull(int i, String... cols) {
		if (cols.length >= i + 1) {
			return cols[i];
		}
		return null;
	}

	private boolean isActionValid() {
		for (String va : validActions) {
			return va.equals(action);
		}
		return false;
	}

	private String buildCallBackData() {
		return buildCallbackData(CALLBACK_TYPE_ACTION, action, data, deviceName, groupName, entityName, idType,
				(targetId == null) ? "" : targetId.toString());
	}

	private static String buildCallbackData(String... arg) {
		StringBuilder result = new StringBuilder("");

		for (int i = 0; i < arg.length; i++) {
			result.append(arg[i] != null ? arg[i] : "");
			result.append(CALLBACK_SPLITTER);
		}

		return result.toString();
	}

	private static String clear(String str) {
		return (str == null) ? "" : str;
	}

	public void setActionRebuildData(String action) {
		this.action = action;
		this.callbackData = buildCallBackData();
	}

	public void rebuildCallbackData() {
		this.callbackData = buildCallBackData();
	}

}