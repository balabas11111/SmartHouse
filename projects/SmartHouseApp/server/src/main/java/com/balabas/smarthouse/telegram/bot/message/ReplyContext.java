package com.balabas.smarthouse.telegram.bot.message;

import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Message;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboard;

import com.balabas.smarthouse.server.view.Action;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class ReplyContext {

	private Integer messageId;
	private Integer replyToMessageId;
	private Integer replyToMessageIdIfUnknown;
	private Long chatId;
	private String text;
	
	private Action action;
	
	public SendMessage createMsg() {
		return ReplyContext.createMsg(null, this, true);
	}
	
	public SendMessage createMsg(ReplyKeyboard markup) {
		return ReplyContext.createMsg(markup, this, true);
	}
	
	public static ReplyContext fromMessage(Message msg) {
		return ReplyContext.builder()
					.chatId(msg.getChatId()).build();
	}
	
	public static SendMessage createMsg(ReplyKeyboard markup, ReplyContext context, boolean enableHtml) {
		return
				createMsg(markup, context.getReplyToMessageId(), context.getChatId(), context.getText(),
						enableHtml);
	}
	
	public static SendMessage createMsg(ReplyKeyboard markup, Long chatId, String text) {
		return createMsg(markup, null, chatId, text, true);
	}
	
	public static SendMessage createMsg(ReplyKeyboard markup, Integer replyToMessageId, Long chatId, String text, boolean enableHtml) {
		SendMessage sendMessage = new SendMessage();
        sendMessage.enableMarkdown(true);

        if(markup!=null) {
        	sendMessage.setReplyMarkup(markup);
        }
        if(replyToMessageId!=null){
            sendMessage.setReplyToMessageId(replyToMessageId);
        }
        sendMessage.setChatId(chatId);
        sendMessage.setText(text);
        sendMessage.enableHtml(enableHtml);
        
        return sendMessage;
	}
}
