package com.balabas.smarthouse.telegram.bot.message;

import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.ReplyKeyboardMarkup;
import org.telegram.telegrambots.meta.api.objects.replykeyboard.buttons.KeyboardRow;

@Component
public class ReplyKeyboardBuilder {

	protected ReplyKeyboardMarkup createReplyKeyboardMarkup() {
		ReplyKeyboardMarkup replyKeyboardMarkup = new ReplyKeyboardMarkup();
		replyKeyboardMarkup.setSelective(true);
		replyKeyboardMarkup.setResizeKeyboard(true);
		replyKeyboardMarkup.setOneTimeKeyboard(false);

		return replyKeyboardMarkup;
	}

	public ReplyKeyboardMarkup getMainMenuReplyKeyboard() {
		ReplyKeyboardMarkup replyKeyboardMarkup = createReplyKeyboardMarkup();

		List<KeyboardRow> keyboard = new ArrayList<>();
/*
		keyboard.add(getMetricsListViewKeyboardRow());
		keyboard.add(getDevicesListViewKeyboardRow());
		keyboard.add(getRefreshDevicesListEditReplyKeyboardRow());
*/
		keyboard.add(getMetricsDevicesEditRow());
		replyKeyboardMarkup.setKeyboard(keyboard);

		return replyKeyboardMarkup;
	}

	public KeyboardRow getMetricsDevicesEditRow() {
		return getKeyboardRow(ItemTextHelper.getMetricsButton(), ItemTextHelper.getDevicesListButton(),
				ItemTextHelper.getSetupButton());
	}

	public KeyboardRow getMetricsListViewKeyboardRow() {
		return getKeyboardRow(ItemTextHelper.getMetricsButton());
	}

	public KeyboardRow getDevicesListViewKeyboardRow() {
		return getKeyboardRow(ItemTextHelper.getDevicesListButton());
	}

	public KeyboardRow getRefreshDevicesListEditReplyKeyboardRow() {
		return getKeyboardRow(ItemTextHelper.getSetupButton());
	}

	private KeyboardRow getKeyboardRow(String text) {
		KeyboardRow row = new KeyboardRow();
		row.add(text);

		return row;
	}

	private KeyboardRow getKeyboardRow(String... strings) {
		KeyboardRow row = new KeyboardRow();
		for (String str : strings) {
			row.add(str);
		}

		return row;
	}

}
