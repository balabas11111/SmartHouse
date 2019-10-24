package com.balabas.smarthouse.telegram.bot.message.actions;

import java.util.List;

import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;

public interface IActionExecutor {

	String getActionName();
	
	List<SendMessage> execute(Action action, ReplyContext context);
}
