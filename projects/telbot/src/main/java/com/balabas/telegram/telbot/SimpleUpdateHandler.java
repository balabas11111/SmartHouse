package com.balabas.telegram.telbot;


import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.UpdateHandler;
import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.ChosenInlineResult;
import io.fouad.jtb.core.beans.InlineKeyboardButton;
import io.fouad.jtb.core.beans.InlineKeyboardMarkup;
import io.fouad.jtb.core.beans.InlineQuery;
import io.fouad.jtb.core.beans.Message;
import io.fouad.jtb.core.beans.ReplyMarkup;
import io.fouad.jtb.core.builders.ApiBuilder;
import io.fouad.jtb.core.enums.ChatAction;
import io.fouad.jtb.core.enums.ParseMode;

public class SimpleUpdateHandler implements UpdateHandler {

	public void onCallbackQueryReceived(TelegramBotApi arg0, int arg1, CallbackQuery arg2) {
		System.out.println("onCallbackQueryReceived");
		if(!checkAuth(arg0,arg2)){
			return;
		}
		
		long chatId=getChatId(arg2);
		String data=arg2.getData();
		
		String result=data;
		
		sendTextMessage(arg0,result,chatId);
	}

	public void onChosenInlineResultReceived(TelegramBotApi arg0, int arg1, ChosenInlineResult arg2) {
		System.out.println("onChosenInlineResultReceived");
		System.out.println(arg0);		
	}

	public void onEditedMessageReceived(TelegramBotApi arg0, int arg1, Message arg2) {
		System.out.println("onEditedMessageReceived");
		System.out.println(arg0);		
	}

	public void onGetUpdatesFailure(Exception arg0) {
		System.out.println("onGetUpdatesFailure");
		System.out.println(arg0);		
	}

	public void onInlineQueryReceived(TelegramBotApi arg0, int arg1, InlineQuery arg2) {
		System.out.println("onInlineQueryReceived");
		System.out.println(arg0);		
	}

	public void onMessageReceived(TelegramBotApi arg0, int arg1, Message arg2) {
		System.out.println("onMessageReceived");
		if(!checkAuth(arg0,arg2)){
			return;
		}
		
		String messageText=arg2.getText();
		long chatId=getChatId(arg2);
		
		System.out.println("Message received="+arg2);
		
		boolean auth=false;
		
		if(arg2.getFrom().getId()==620255503){
			auth=true;
		}
		
		String result="Я ни знаю кто ты ваще. Не пеши мне больше";
		InlineKeyboardMarkup km=new InlineKeyboardMarkup();
		//0 text 1 keyboard
		int mode=0;
				
		if(auth){
			result="А че хотел то?";
			
			if("/start".equals(messageText)){
				result="запустил";
			}
			if("/settings".equals(messageText)){
				result="*Tune your brain*";
			}
			if("/help".equals(messageText)){
				result="<em>God will help</em> or just call to <b>balabas</b>";
			}
			if(messageText.startsWith("/whois")){
				if(messageText.contains("Pushkin")){
					result="<b>Мечков Павел Александрович - торчок</b>";
				}else
				if(messageText.contains("Roma")){
					result="Кросавчег)";
				}else
				if(messageText.contains("Kostik")){
					result="Кросавчег)";
				}else
				if(messageText.contains("Vitalik")){
					result="<b>Кросавчег и молодец)</b>";
				}else
				if(messageText.contains("Vika")){
					result="<b>Нечемнa дівчинка</b>";
				}else
				if(messageText.contains("Lyuba")){
					result="<b>Мама Люба)</b>";
				}else
				{
					result="не знаю такого...";
				}
				
				
			}
			if("/q".equals(messageText)){
				
				InlineKeyboardButton[][] buttons=new InlineKeyboardButton[1][2];
				
				buttons[0][0]=new InlineKeyboardButton("Hello",null,"@hello",null);
				buttons[0][1]=new InlineKeyboardButton("pressMe",null,"@pressMe",null);
				
				km.setInlineKeyboard(buttons);
				
				mode=1;
			}
		}
		
		switch(mode){
			case 0:
				 sendTextMessage(arg0,result,chatId);
			break;
			case 1:
				sendKeyboard(arg0,result,chatId,km);
			break;
		}
	}
	
	private boolean checkAuth(TelegramBotApi arg0,CallbackQuery arg2){
		return checkAuth(arg0,arg2.getMessage());
	}
	
	private boolean checkAuth(TelegramBotApi arg0,Message msg){
		String userName=msg.getFrom().getUsername();
		if(userName==null || userName!="balabas1111111111"){
			return true;
		}
		
		 sendTextMessage(arg0,"403 not authorized",getChatId(msg));
		return false;
	}
	
	private long getChatId(CallbackQuery q){
		return getChatId(q.getMessage());
	}
	
	private long getChatId(Message m){
		return m.getChat().getId();
	}
	
	private void sendTextMessage(TelegramBotApi arg0,String msg,long chatId){
		try {
			ApiBuilder.api(arg0).sendChatAction(ChatAction.TYPING).toChatId(chatId).execute();
			ApiBuilder.api(arg0).sendMessage(msg).toChatId(chatId).parseMessageAs(ParseMode.HTML).execute();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void sendKeyboard(TelegramBotApi arg0,String msg,long chatId,ReplyMarkup replyMarkup){
		try {
			ApiBuilder.api(arg0).sendMessage(msg).toChatId(chatId).applyReplyMarkup(replyMarkup).execute();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
