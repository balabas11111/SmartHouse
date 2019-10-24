package com.balabas.smarthouse.telegram.bot.message.actions;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;
import com.balabas.smarthouse.telegram.bot.message.SendMessageBuilder;

@Service
public class ActionExecuteService implements IActionExecuteService {

	private Map<String, IActionExecutor> executors = Collections.synchronizedMap(new HashMap<>());
    
	@Autowired
	protected SendMessageBuilder messageBuilder;
	
    @Autowired(required = false)
    public void setExecutors(Set<IActionExecutor> exec) {
        exec.stream().forEach( e -> executors.put(e.getActionName(), e));
    }
	
	@Override
	public IActionExecutor getExecutor(Action action) {
		if(executors.containsKey(action.getAction())) {
			return executors.get(action.getAction());
		}
		return null;
	}

	@Override
	public List<SendMessage> executeAction(Action action, ReplyContext context) {
		IActionExecutor executor = getExecutor(action);
		if(executor != null) {
			return executor.execute(action, context);
		}
		
		context.setText(null);
		return Collections.singletonList(messageBuilder.createUnknown(context));
	}

}
