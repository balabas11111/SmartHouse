package com.balabas.smarthouse.server.entity.model;

import java.util.Optional;

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
	@Getter @Setter
	protected Long id = 0L;
	@Getter @Setter
	protected String name;
	@Getter @Setter
	protected String description;
	@Setter
	@Enumerated(EnumType.STRING)
	protected Emoji emoji;

	@Override
	public void setDescriptionIfEmpty(String description) {
		if (StringUtils.isEmpty(this.description)) {
			setDescription(description);
		}
	}
	
	@Override
	public Emoji getEmoji() {
		return Optional.ofNullable(this.emoji).orElse(Emoji.EMPTY_EMOJI);
	}
}
