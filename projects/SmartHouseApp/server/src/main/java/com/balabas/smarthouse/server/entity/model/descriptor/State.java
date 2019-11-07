package com.balabas.smarthouse.server.entity.model.descriptor;

public enum State {
	UNKNOWN(Emoji.HOURGLASS,"Не инициализировано"),
	CONSTRUCTED(Emoji.HOURGLASS,"Не инициализировано"),
	REGISTERED(Emoji.HOURGLASS,"Не инициализировано"),
	REREGISTERED(Emoji.HOURGLASS,"Не инициализировано"),
	CONNECTED(Emoji.CHECK_MARK,"ok"),
	INIT_DATA_RECEIVED(Emoji.CHECK_MARK,"ok"),
	UPDATED(Emoji.CHECK_MARK,"ok"),
	FAILED(Emoji.RED_CIRCLE,"Ошибка"),
	TIMED_OUT(Emoji.GHOST,"Не подключено"),
	DISCONNECTED(Emoji.SKULL,"Ошибка подключения"),
	OK(Emoji.CHECK_MARK,"ok"),
	BAD_DATA(Emoji.RED_CIRCLE,"Ошибка данных");
	
	private static final Emoji okEmoji =  Emoji.CHECK_MARK;
	
	Emoji emoji;
	
	String description;
	
	private State(Emoji emoji, String description) {
		this.emoji = emoji;
		this.description = description;
	}
	
	public Emoji getStateEmoji(boolean emptyIfOk) {
		boolean isOk = okEmoji.equals(emoji);
		
		if(isOk && emptyIfOk) {
			return Emoji.EMPTY_EMOJI;
		}
		
		return emoji;
	}
	
	public String getStateDescription(boolean emptyIfOk) {
		boolean isOk = okEmoji.equals(emoji);
		
		if(isOk && emptyIfOk) {
			return "";
		}
		
		return description;
	}
}
