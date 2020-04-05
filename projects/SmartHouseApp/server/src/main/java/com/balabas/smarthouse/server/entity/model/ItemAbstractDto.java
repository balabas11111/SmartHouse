package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
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
	
	
}
