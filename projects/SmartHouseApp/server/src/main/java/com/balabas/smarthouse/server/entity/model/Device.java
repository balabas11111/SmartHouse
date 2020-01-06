package com.balabas.smarthouse.server.entity.model;

import java.util.Date;
import java.util.Set;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import javax.persistence.CascadeType;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true, exclude = "groups")
@Table(name = "devices")
@javax.persistence.Entity
@SuppressWarnings("rawtypes")
public class Device extends ItemAbstract implements IDevice {

	private String firmware;
	private String ip;
	private String dataUrl;

	@Enumerated(EnumType.STRING)
	private State state;

	private Date registrationDate;

	@OneToMany(mappedBy = "device", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<Group> groups;

	@Transient
	private boolean initialized;
/*
	@Transient
	private ActionTimer timer;
*/
	
	public Device() {
		//this.timer = new ActionTimer(ItemType.DEVICE.getRefreshInterval());
	}

	@Override
	public Group getGroup(String groupName) {
		if (getGroups() == null) {
			return null;
		}
		return getGroups().stream().filter(g -> g.getName().equalsIgnoreCase(groupName)).findFirst().orElse(null);
	}

	@JsonIgnore
	@Override
	public Set<Entity> getEntities() {
		return getGroups().stream().flatMap(group -> group.getEntities().stream()).collect(Collectors.toSet());
	}

	@Override
	public Entity getEntity(String entityName) {
		return getEntityByFilter(e -> e.getName() != null && e.getName().equals(entityName));
	}

	@Override
	public Entity getEntity(Long entityId) {
		return getEntityByFilter(e -> e.getId() != null && e.getId().equals(entityId));
	}
	
	@Override
	public IEntityField getEntityField(Long entityId, Long entityFieldId) {
		return getEntity(entityId).getEntityField(entityFieldId);
	}
	/*
	@Override
	public Entity getEntityByRemoteId(int remoteId) {
		return getGroups().stream().flatMap(group -> group.getEntities().stream())
				.filter(e -> e.getRemoteId() == remoteId).findFirst().orElse(null);
	}
*/
	@Override
	public boolean isRegistered() {
		return state != null && state.compareTo(State.REGISTERED) >= 0;
	}
	
	@Override
	public boolean isInBadState() {
		return state != null && (State.TIMED_OUT.equals(state) || State.DISCONNECTED.equals(state) || State.FAILED.equals(state) || State.BAD_DATA.equals(state));
	}
	
	@Override
	public boolean isInOkState() {
		return state != null && (State.INIT_DATA_RECEIVED.equals(state) || State.UPDATED.equals(state) || State.OK.equals(state));
	}
	

	private Entity getEntityByFilter(Predicate<? super Entity> entityPredicate) {
		if (getGroups() == null) {
			return null;
		}
		return getGroups().stream().flatMap(group -> group.getEntities().stream())
				.filter(entityPredicate).findFirst().orElse(null);
	}

	@Override
	public String getStateEmoji() {
		if(getState() == null ) {
			return Emoji.EMPTY_EMOJI.toString();
		}
		return getState().getStateEmoji(false).toString();
	}

	@Override
	public String getStateDescription() {
		return getState().getStateDescription(false);
	}

}
