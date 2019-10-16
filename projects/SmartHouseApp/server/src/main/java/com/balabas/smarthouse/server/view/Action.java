package com.balabas.smarthouse.server.view;

import java.util.ArrayList;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
public class Action {
	
	public static final String CALLBACK_SPLITTER = ";";
	public static final String CALLBACK_TYPE_ACTION = "Act";
	
	public static final String ACTION_TYPE_UNKNOWN                   = "NUL";
	public static final String ACTION_TYPE_VIEW_DEVICE_LIST          = "vdl";
	public static final String ACTION_TYPE_VIEW_GROUPS_OF_DEVICE     = "vgd";
	public static final String ACTION_TYPE_VIEW_ENTITIES_OF_GROUP    = "veg";
	public static final String ACTION_TYPE_VIEW_ENTITITY             = "ven";
	public static final String ACTION_TYPE_SEND_DATA_TO_DEVICE       = "sdd";
	public static final String ACTION_TYPE_GET_DATA_FROM_DEVICE       = "sdd";
	
	public static final String validActions[] = {
													ACTION_TYPE_VIEW_DEVICE_LIST,
													ACTION_TYPE_VIEW_GROUPS_OF_DEVICE,
													ACTION_TYPE_VIEW_ENTITIES_OF_GROUP,
													ACTION_TYPE_VIEW_ENTITITY
												};
	@Getter
	private String action;
	@Getter
	private String deviceId;
	@Getter
	private String groupId;
	@Getter
	private String entityId;
	@Getter
	private String data;
	@Getter @Setter
	private Object target;
	@Getter
	private String callbackData;

	@Getter
	private boolean valid = true;
	
	public Action() {
		this.valid = false;
	}
	
	public Action(String...cols) {
		this.action = getColValueOrNull(1, cols );
		this.data = getColValueOrNull(2, cols );
		this.deviceId = getColValueOrNull(3, cols );
		this.groupId = getColValueOrNull(4, cols );
		this.entityId = getColValueOrNull(5, cols );
			
		this.callbackData = buildCallBackData();
		this.valid = isActionValid();
	}
	
		
	public Action(String action, String data, String deviceId, String groupId, String entityId) {
		
		this.action = action;
		this.deviceId = deviceId;
		this.groupId = groupId;
		this.entityId = entityId;
		this.data = data;
		
		this.callbackData = buildCallBackData();
		
		this.valid = isActionValid();
	}
		
	public void setData(String data) {
		this.data = clear(data);
		this.callbackData = buildCallBackData();
	}
	
	public static Action fromDevice(String action, IDevice device, String data) {
		return new Action(action, data, device.getDeviceName());
	}
	
	public static Action fromGroup(String action, IGroup group, String data) {
		return new Action(action, data, group.getDeviceName(), group.getName());
	}
	
	public static Action fromEntity(String action, IEntity entity, String data) {
		return new Action(action, data, entity.getDeviceName(), entity.getGroupName(), entity.getName());
	}
	
	public static Action fromColumnList(String...cols) {
		List<String> list = new ArrayList<String>();
		
		if(cols==null || cols.length<1 || !cols[0].equals(CALLBACK_TYPE_ACTION)) {
			list.add(CALLBACK_TYPE_ACTION);
		}
		if(cols!=null) {
			for(String str:cols) {
				list.add(str);
			}
		}
		
		String[] strArr = list.toArray(new String[list.size()]);
		return new Action(strArr);
	}
	
	public static String callback(String...cols) {
		return (Action.fromColumnList(cols)).getCallbackData();
	}
		
	public static Action fromCallbackData(String str) {
		if(str == null || str.isEmpty()) {
			return new Action();
		}
		
		String[] col = str.split(CALLBACK_SPLITTER);
		
		if(col==null || col.length <2 || !CALLBACK_TYPE_ACTION.equals(col[0])) {
			return new Action();
		}
		
		return new Action(col);
	}
		
	@Override
	public String toString() {
		return this.callbackData;
	}
		
	private String getColValueOrNull(int i, String...cols) {
		if(cols.length>=i+1) {
			return cols[i];
		}
		return null;
	}
			
		
		private boolean isActionValid() {
			for(String va:validActions) {
				return va.equals(action);
			}
			return false;
		}
		
		private String buildCallBackData() {
			return buildCallbackData(CALLBACK_TYPE_ACTION, action, data, deviceId, groupId, entityId);
		}
		
		private static String buildCallbackData(String... arg){
			StringBuilder result = new StringBuilder("");
			
			for(int i=0; i<arg.length; i++){
				result.append(arg[i]!=null?arg[i]:"");
				result.append(CALLBACK_SPLITTER);
			}
			
			return result.toString();
		}
		
		private static String clear(String str) {
			return (str==null)?"":str;
		}

		public void setActionRebuildData(String action) {
			this.action = action;
			this.callbackData = buildCallBackData();
		}

}