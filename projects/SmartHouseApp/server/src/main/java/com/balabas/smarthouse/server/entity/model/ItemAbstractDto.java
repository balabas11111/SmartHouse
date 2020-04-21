package com.balabas.smarthouse.server.entity.model;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

@AllArgsConstructor
@ToString
@Builder
public class ItemAbstractDto {

	@Getter @Setter
	private Long id;
	
	@Getter @Setter
	private Emoji emoji;
	
	@Getter @Setter
	private String name;
	
	@Getter @Setter
	private String description;
	
	@Getter @Setter
	private ItemType itemType; 
	
	public boolean isIdEqual(Long otherId) {
		return id.equals(otherId);
	}
	
	public static ItemAbstractDto fromItem(IItemAbstract item) {
		return new ItemAbstractDto(item.getId(), item.getEmoji(), item.getName(), item.getDescription(), item.getItemType());
	}
	
	public static ItemAbstractDto fromItemByPath(IItemAbstract item) {
		return new ItemAbstractDto(item.getId(), item.getEmoji(), item.getName(), item.getParentNamesChain(), item.getItemType());
	}
	
	public static ItemAbstractDto fromShortName(String name) {
		
		if(name==null || StringUtils.isEmpty(name) || name.length()<3) {
			return new ItemAbstractDto();
		}
		ItemType itemType = ItemType.getItemTypeByShortName(name.substring(0, 2));
		Long id = Long.parseLong(name.substring(2));
		
		return ItemAbstractDto.builder().id(id).itemType(itemType).build();
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

	public ItemAbstractDto() {
	}
	
}
