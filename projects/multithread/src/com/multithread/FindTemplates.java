package com.multithread;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.List;

public class FindTemplates {

	public static final String PATH = "C:\\projects\\WorldCheckOne-AWS\\wc1-applications";
	public static final String TMPLT_NAME = "wc1-ecs-components.yaml";
	public static final String TMPLT_NAME2 = "wc1-alarms.yaml";
	public static final String COMMAND = "aws cloudformation validate-template --profile=a-worldcheck-preprod --template-body file://";
	public static final String COMMAND2 = " > log.txt";
	
	public static void main(String[] args) {
		FindTemplates app = new FindTemplates();
		app.processFolder(PATH);
		app.templates.stream().forEach(System.out::println);
		System.out.println();
	}
	
	public List<String> templates = new ArrayList<>();
	
	public void processFolder(String folder) {
		String[] subfolders = getSubfolders(folder);
		
		if(subfolders.length == 0) {
			String[] files = getTemplates(folder);
			if(files.length>0) {
				for(String fileName : files) {
					templates.add(COMMAND + folder + "\\" + fileName + COMMAND2);
				}
			}
		} else {
			for(String str : subfolders) {
				processFolder(folder + "\\" +str);
			}
		}
	}
	
	public String[] getTemplates(String folder) {
		File file = new File(folder);
		String[] files = file.list(new FilenameFilter() {
		  @Override
		  public boolean accept(File current, String name) {
			  return TMPLT_NAME.equals(name) || TMPLT_NAME2.equals(name);
		  }
		});
		
		return files;
	}
	
	public String[] getSubfolders(String folder) {
		File file = new File(folder);
		String[] directories = file.list(new FilenameFilter() {
		  @Override
		  public boolean accept(File current, String name) {
		    return new File(current, name).isDirectory();
		  }
		});
		
		return directories;
	}
}
