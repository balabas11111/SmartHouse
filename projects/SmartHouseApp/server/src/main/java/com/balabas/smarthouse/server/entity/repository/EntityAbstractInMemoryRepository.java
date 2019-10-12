package com.balabas.smarthouse.server.entity.repository;

import java.io.File;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.ResolvableType;

import com.balabas.smarthouse.server.entity.model.EntityField;
import com.balabas.smarthouse.server.entity.model.IEntityAbstract;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@SuppressWarnings("rawtypes")
public abstract class EntityAbstractInMemoryRepository<T extends IEntityAbstract> implements IEntityAbstractRepository<T> {

	@Getter
	@JsonIgnore
	Class clazz;
	
	@Getter
	@Value("${smarthouse.server.repository.path:repository}")
	private String storagePath;
	
	private Set<T> list;
	
	public EntityAbstractInMemoryRepository() {
		this.clazz = ResolvableType.forClass(EntityField.class).getClass();
	}
	
	protected File getStorageFile(boolean create) throws IOException {
		File file = new File(getStoragePath() + this.getClass().getSimpleName() + ".json");
		log.info("Storage =" + file.getAbsolutePath());
		if(create && !file.exists()) {
			file.createNewFile();
		}
		return file;
	}
	
	@Override
	public void save() throws IOException {
		File file = getStorageFile(true);

		ObjectMapper mapper = new ObjectMapper();
		mapper.writeValue(file, list);

		log.info("Alarms saved");
	}
	
	@Override
	public void load() throws IOException {
		File file = getStorageFile(false);
		if (file.exists()) {
			ObjectMapper mapper = new ObjectMapper();

			Set<T> loaded = mapper.readValue(file,
					new TypeReference<Set<T>>() {
					});

			this.list = loaded;
		} else {
			this.list = new HashSet<T>();
		}
	}
}
