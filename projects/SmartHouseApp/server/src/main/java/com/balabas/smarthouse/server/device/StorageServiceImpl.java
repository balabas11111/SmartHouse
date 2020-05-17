package com.balabas.smarthouse.server.device;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.OpenOption;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import javax.annotation.PostConstruct;

import org.apache.tomcat.jni.Error;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class StorageServiceImpl implements StorageService {

	@Value("${smarthouse.server.files.upload.storage:files}")
	private String storagePath = "/";

	@Override
	@PostConstruct
	public void init() throws Error, IOException {

		File file = new File(storagePath);

		if (file == null || !file.exists()) {
			Path path = Paths.get(storagePath);
			try {
				Files.createDirectory(path);
			} catch (Exception e) {
				log.error(e);
			}

			file = path.toFile();
		}

		if (file != null && file.exists() && file.isDirectory()) {
			log.info("Storage Directory = " + file.getAbsolutePath());
		} else {
			log.error("Error access to storage");
		}

		log.info("service loaded");

	}

	@Override
	public void store(MultipartFile file) throws IOException {
		try {
			Files.write(getPath(file.getOriginalFilename(), false), file.getBytes());

		} catch (Exception e) {
			log.error(e);
		}
	}

	@Override
	public List<DeviceFile> loadAllFiles() throws IOException {
		Path rootDir = getPath("", false);

		String pathStr = rootDir.toFile().getAbsolutePath();

		log.info("load path " + pathStr);

		List<Path> paths = Files.list(rootDir).collect(Collectors.toList());
		
		List<DeviceFile> files = paths.stream().map(path -> {return new DeviceFile("", path.toFile().getAbsolutePath(), 100);}).collect(Collectors.toList());

		return files;
	}

	@Override
	public Path load(String filename) throws IOException {
		return getPath(filename, false);
	}

	@Override
	public String loadAsString(String fileName) throws IOException {
		Path path;
		StringBuilder data = new StringBuilder();
		Stream<String> lines = null;
		try {
			path = Paths.get(Thread.currentThread().getContextClassLoader().getResource(fileName).toURI());
			lines = Files.lines(path);
		} catch (URISyntaxException | IOException e) {
			log.error("Error in reading propertied file " + e);
			throw new RuntimeException(e);
		}

		lines.forEach(line -> data.append(line));
		lines.close();
		return data.toString();
	}

	@Override
	public void deleteAll() throws IOException {
		Files.list(getPath("", false)).forEach(path -> {
			try {
				Files.delete(path);
			} catch (IOException e) {
				e.printStackTrace();
			}
		});

	}
	
	@Override
	public void delete(String fileName) throws IOException {
			try {
				Files.delete(getPath(fileName, false));
			} catch (IOException e) {
				e.printStackTrace();
			}

	}

	public Set<OpenOption> fuseOpenFlagsToNioOpenOptions() {
		Set<OpenOption> result = new HashSet<>();

		result.add(StandardOpenOption.READ);
		result.add(StandardOpenOption.WRITE);
		result.add(StandardOpenOption.APPEND);
		result.add(StandardOpenOption.TRUNCATE_EXISTING);

		return result;
		/*
		 * if (flags.contains(OpenFlags.O_RDONLY) || flags.contains(OpenFlags.O_RDWR)) {
		 * result.add(StandardOpenOption.READ); } if (flags.contains(OpenFlags.O_WRONLY)
		 * || flags.contains(OpenFlags.O_RDWR)) { result.add(StandardOpenOption.WRITE);
		 * } if (flags.contains(OpenFlags.O_APPEND)) {
		 * result.add(StandardOpenOption.APPEND); } if
		 * (flags.contains(OpenFlags.O_TRUNC)) {
		 * result.add(StandardOpenOption.TRUNCATE_EXISTING); } return result;
		 */
	}

	private Path getPath(String fileName, boolean createDir) throws IOException {

		Path path = Paths.get(getPathStr(fileName));

		if (createDir && !path.toFile().exists()) {
			Files.createDirectory(path);
			log.info("folder created " + path.getFileName());
		}

		return path;
	}

	private String getPathStr(String fileName) {
		return storagePath + "/" + fileName;
	}

}
