package com.balabas.html.packager;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.naming.ConfigurationException;

import org.apache.commons.io.IOUtils;

public class FileUtility {
	
	private static List<String> extensions=new ArrayList<String>(){/**
         * 
         */
        private static final long serialVersionUID = 1L;

    {
	    add(".htm");
	    add(".html");
	    add(".css");
	    add(".js");
	    add(".txt");
	    add(".gz");
	}};
	
	public static void deleteFile(File file) throws IOException{
	    if(!file.exists()){
	        return;
	    }
	    if(file.isDirectory()){
	        deleteDirectory(file);
	        return;
	    }
	    
	    Files.delete(file.toPath());
	}
	
	public static boolean deleteDirectory(File directoryToBeDeleted) {
	    File[] allContents = directoryToBeDeleted.listFiles();
	    if (allContents != null) {
	        for (File file : allContents) {
	            deleteDirectory(file);
	        }
	    }
	    return directoryToBeDeleted.delete();
	}

	public static void printFiles(List<File> files,String header){
		System.out.println("---------------------"+header+"-------------------------");
		for(File f:files){
			System.out.println(f.getAbsolutePath());
		}
		System.out.println("----------------------------------------------");
	}
	
	public static void listMap(String directoryName, Map<String,File> files,
            List<String> excludeFolders,List<String> duplicates,boolean subfolders) throws ConfigurationException{
	    
	    File directory = new File(directoryName);

	    // Get all files from a directory.
	    File[] fList = directory.listFiles();
	    if(fList != null)
	        for (File file : fList) {      
	            if (file.isFile()) {

	                if(isHtmlExt(file) && !shouldExcluded(file,excludeFolders)){
	                    String key=ReplaceUtility.getPlaceHolderKey(file.getName());
	                    
	                    if(files.containsKey(key)){
	                        duplicates.add(key);
	                    }
	                    
	                    files.put(key,file);
	                    //System.out.println(key+" "+file);
	                }
	            } else if (file.isDirectory()  && subfolders) {
	                listMap(file.getAbsolutePath(), files,excludeFolders, duplicates,subfolders);
	            }
	        }
	}
	
	public static void listf(File directory, List<File> files,
	        List<String> excludeFolders,boolean readSubfolders) {

    // Get all files from a directory.
    File[] fList = directory.listFiles();
    if(fList != null)
        for (File file : fList) {      
            if (file.isFile()) {

            	if(isHtmlExt(file) && !shouldExcluded(file,excludeFolders)){
            		files.add(file);
            	}
            } else if (file.isDirectory() && readSubfolders) {
                listf(file, files,excludeFolders,readSubfolders);
            }
        }
    }
	
	public static boolean shouldExcluded(File file,List<String> excludeFolders){
		if(excludeFolders==null){
			return false;
		}
		if(file==null || file.getName()==null){
			return true;
		}
		
		for(String str:excludeFolders){
			if(file.getAbsolutePath().contains(str)){
				return true;
			}
		}
		
		return false;
	}
	
	public static boolean isHtmlExt(File file){
		if(file==null || file.getName()==null){
			return false;
		}
		
		String fileName=file.getName();
		
		for(String ext:extensions){
			if(fileName.endsWith(ext)){
				return true;
			}
		}
		
		return false;
	}
	
	 public static String checkFolderName(String folder){
        if(!folder.endsWith("\\")){
            folder=folder+"\\";
        }
        return folder;
    }
	
	public static String getFileAsString(File sourceFile) throws IOException{
	    return IOUtils.toString(new FileInputStream(sourceFile),"UTF-8");
	}
	
	public static Path saveStringToFile(Path path,String content) throws UnsupportedEncodingException, IOException{
        return Files.write(path, content.getBytes("UTF-8"));
	}

}
