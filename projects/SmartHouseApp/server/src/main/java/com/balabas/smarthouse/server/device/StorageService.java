package com.balabas.smarthouse.server.device;

import org.apache.tomcat.jni.Error;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.nio.file.Path;
import java.util.List;

public interface StorageService {

	void init() throws Error, IOException;

	void store(MultipartFile file) throws IOException;

	Path load(String filename) throws IOException;

	void deleteAll() throws IOException;

	String loadAsString(String filename) throws IOException;

	List<DeviceFile> loadAllFiles() throws IOException;

	void delete(String fileName) throws IOException;

}