package com.balabas.html.packager;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

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
	
	public static void listf(String directoryName, List<File> files,
	        List<String> excludeFolders,boolean readSubfolders) {
    File directory = new File(directoryName);

    // Get all files from a directory.
    File[] fList = directory.listFiles();
    if(fList != null)
        for (File file : fList) {      
            if (file.isFile()) {

            	if(isHtmlExt(file) && !shouldExcluded(file,excludeFolders)){
            		files.add(file);
            	}
            } else if (file.isDirectory() && readSubfolders) {
                listf(file.getAbsolutePath(), files,excludeFolders,readSubfolders);
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

}
