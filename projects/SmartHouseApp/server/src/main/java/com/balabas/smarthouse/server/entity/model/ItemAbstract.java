package com.balabas.smarthouse.server.entity.model;

import java.util.Optional;

import javax.persistence.Column;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public class ItemAbstract implements IItemAbstract {

	public static final String PROP_ID = "id";
	public static final String PROP_NAME = "name";

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter
	@Setter
	protected Long id = 0L;
	@Getter
	@Setter
	protected String name;
	@Getter
	@Setter
	protected String description;
	@Setter
	@Enumerated(EnumType.STRING)
	protected Emoji emoji;
	@Getter
	@Setter
	@Column(columnDefinition = "boolean default false")
	protected Boolean virtualized;

	@Override
	public void setDescriptionIfEmpty(String description) {
		if (StringUtils.isEmpty(getDescription())) {
			setDescription(description);
		}
	}

	@Override
	public String getItemClassId() {
		StringBuilder builder = new StringBuilder();

		builder.append(this.getClass().getSimpleName());

		return builder.toString();
	}

	@Override
	public Emoji getEmoji() {
		return getEmojiOrDefault(Emoji.EMPTY_EMOJI);
	}

	public static int compareByName(IItemAbstract ia1, IItemAbstract ia2) {
		return ia1.getName().compareTo(ia2.getName());
	}

	@Override
	public Emoji getEmojiOrDefault(Emoji defaultEmoji) {
		Optional<Emoji> em = Optional.ofNullable(this.emoji);

		if (!em.isPresent() || (em.isPresent() && Emoji.EMPTY_EMOJI.equals(em.get()))) {
			return defaultEmoji;
		}
		return em.get();
	}

	@Override
	public String getDescriptionByDescriptionField() {
		String result = getDescription();
		
		if(StringUtils.isEmpty(result)) {
			result = getName();
		}
		return result;
	}

	@Override
	public String getEmojiDescriptionByDescriptionField() {
		return getEmoji() + " " + getDescriptionByDescriptionField();
	}

	@Override
	public String getEmojiDescriptionByDescriptionFieldWithParent() {
		return getEmojiDescriptionByDescriptionField();
	}

	@Override
	public boolean isVirtualized() {
		return Boolean.TRUE.equals(virtualized);
	}

	@Override
	public void setParent(IItemAbstract parent) {
	}

	public static boolean existsAndIsNotNew(IItemAbstract item) {
		return item != null && !isNew(item);
	}

	public static boolean isNew(IItemAbstract item) {
		return item.getId() == null || item.getId().equals(0L);
	}
	
}
