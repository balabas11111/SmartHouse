package com.balabas.smarthouse.telegram.bot.service;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.view.Action;

@Service
public class CurrentEditActionService {

	private Map<Long, Action> currentEditActions = Collections.synchronizedMap(new HashMap<>());

	public boolean has(Long chatId) {
		return currentEditActions.containsKey(chatId);
	}

	public void put(Long chatId, Action action) {
		currentEditActions.put(chatId, action);
	}

	public Action get(Long chatId) {
		return currentEditActions.getOrDefault(chatId, null);
	}

	public Action remove(Long chatId) {
		return currentEditActions.remove(chatId);
	}
}
