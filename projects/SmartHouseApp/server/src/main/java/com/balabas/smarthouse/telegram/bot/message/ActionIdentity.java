package com.balabas.smarthouse.telegram.bot.message;

import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_UNKNOWN;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_DEVICE_VALUE_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SETUP;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_ALARMS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_EDIT_DEVICE_SELECT_LIST;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_HELP;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_MAIN_COMMANDS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_METRICS;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_VIEW_ALL_DEVICES;

import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_HOME;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_ROOT;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_SETUP;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_ALARM;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_EDIT;
import static com.balabas.smarthouse.telegram.bot.message.BotMessageConstants.COMMAND_HELP;

import java.util.Arrays;
import java.util.List;

import com.balabas.smarthouse.server.view.Action;
import com.google.common.collect.Lists;

public class ActionIdentity {

	public static List<ActionIdentity> identities = Arrays.asList(
			new ActionIdentity(Arrays.asList(ItemTextHelper.getMetricsButton()),
					Arrays.asList(COMMAND_HOME, COMMAND_ROOT), ACTION_TYPE_VIEW_ALL_METRICS),
			new ActionIdentity(Arrays.asList(ItemTextHelper.getDevicesListButton()),
					Lists.newArrayList(), ACTION_TYPE_VIEW_ALL_DEVICES),
			new ActionIdentity(Arrays.asList(ItemTextHelper.getSetupButton()), 
					Arrays.asList(COMMAND_SETUP), ACTION_TYPE_SETUP),
			new ActionIdentity(null, 
					Arrays.asList(COMMAND_ALARM), ACTION_TYPE_EDIT_ALARMS),
			new ActionIdentity(null, 
					Arrays.asList(COMMAND_EDIT), ACTION_TYPE_EDIT_DEVICE_SELECT_LIST),
			new ActionIdentity(null, Arrays.asList(COMMAND_HELP),
					ACTION_TYPE_VIEW_HELP));

	List<String> dataStart;
	List<String> dataEquals;
	String actionName;

	public ActionIdentity(List<String> dataStart, List<String> dataEquals, String actionName) {
		this.dataStart = dataStart;
		this.dataEquals = dataEquals;
		this.actionName = actionName;
	}

	boolean accepts(String data) {
		boolean accept = false;

		if (data == null) {
			return false;
		}

		if (dataStart != null) {
			for (String start : dataStart) {
				if (data.startsWith(start)) {
					return true;
				}
			}
		}

		if (dataEquals != null) {
			for (String eq : dataEquals) {
				if (eq.equalsIgnoreCase(data)) {
					return true;
				}
			}
		}

		return accept;
	}

	public Action getAction() {
		return Action.fromColumnList(actionName);
	}

	public static Action getByData(String data) {

		for (ActionIdentity id : identities) {
			if (id.accepts(data)) {
				return id.getAction();
			}
		}

		return Action.fromColumnList(ACTION_TYPE_UNKNOWN);
	}
}
