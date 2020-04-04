package com.balabas.smarthouse.server.entity.model;

import java.util.ArrayList;
import java.util.List;
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
public abstract class ItemAbstract implements IItemAbstract {

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
	
	public static int compareByName(IItemAbstract ia1, IItemAbstract ia2) {
		return compareByString(ia1, ia2, ia1.getName(), ia2.getName(), false);
	}
	
	public static int compareByDescriptionField(IItemAbstract ia1, IItemAbstract ia2) {
		return compareByString(ia1, ia2, ia1.getDescriptionByDescriptionField(), ia2.getDescriptionByDescriptionField(), false);
	}
	
	public static int compareByDescription(IItemAbstract o1, IItemAbstract o2) {
		return compareByString(o1, o2, o1.getDescription(), o2.getDescription(), true);
	}

	public static int compareByString(IItemAbstract o1, IItemAbstract o2, String d1, String d2, boolean ignoreCase) {
		if (o1 == null && o2 == null) {
			return 0;
		}

		if (d1 == null && d2 == null) {
			return 0;
		}

		if (d1 != null && d2 != null) {
			return ignoreCase ? d1.compareToIgnoreCase(d2): d1.compareTo(d2);
		}
		if (d2 != null) {
			return -1;
		}
		if(d1!=null) {
			return 1;
		}

		return 0;
	}

	@Override
	public String getParentNamesChain() {
		return this.getClass().getSimpleName() + getName() + " " + getDescription();
	}
	
	@Override
	public List<IItemAbstract> getAllChildren() {
		return new ArrayList<IItemAbstract>();
	}
}
