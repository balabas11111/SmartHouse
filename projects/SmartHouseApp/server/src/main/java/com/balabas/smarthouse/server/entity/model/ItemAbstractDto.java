package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmState;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

@AllArgsConstructor
@ToString
public class ItemAbstractDto {

	@Getter @Setter
	private Long id;
	
	@Getter @Setter
	private Emoji emoji;
	
	@Getter @Setter
	private String name;
	
	@Getter @Setter
	private String description;
	
	public boolean isIdEqual(Long otherId) {
		return id.equals(otherId);
	}
	
	public static ItemAbstractDto fromItem(IItemAbstract item) {
		return new ItemAbstractDto(item.getId(), item.getEmoji(), item.getName(), item.getDescription());
	}
	
	public static ItemAbstractDto fromItemByPath(IItemAbstract item) {
		return new ItemAbstractDto(item.getId(), item.getEmoji(), item.getName(), item.getParentNamesChain());
	}
	
	public IItemAbstract toItem(IItemAbstract item) {
		item.setId(id);
		item.setEmoji(emoji);
		item.setName(name);
		item.setDescription(description);
		
		return item;
	}
	
	public IItemAbstract toItemWithCheck(IItemAbstract item) {
		if(doChange(item.getId(), id)) {
			item.setId(id);
		}
		if(doChange(item.getEmoji(), emoji)) {
			item.setEmoji(emoji);
		}
		if(doChange(item.getName(), name)) {
			item.setName(name);
		}
		if(doChange(item.getDescription(), description)) {
			item.setDescription(description);
		}
		return item;
	}
	
	private boolean doChange(Object obj, Object objDto) {
		return (obj == null && objDto!=null) || (obj!=null && objDto!=null && !obj.equals(objDto));
	}

	public ItemAbstractDto(Emoji emoji, String name, String description) {
		super();
		this.emoji = emoji;
		this.name = name;
		this.description = description;
	}
	
	public ItemAbstractDto(AlarmState alarmState) {
		super();
		this.emoji = alarmState.getEmoji();
		this.name = alarmState.getName();
		this.description = alarmState.getDescription();
	}
	
	public String getHint() {
		return this.emoji.toString() + " " + this.description;
	}
	
}
