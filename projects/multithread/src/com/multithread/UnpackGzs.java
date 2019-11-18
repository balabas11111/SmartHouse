package com.multithread;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.zip.GZIPInputStream;

public class UnpackGzs {

	public static final String PATH = "c:\\\\projects\\\\case-manager-rest-with-metrics-config\\\\cloudformation\\\\cloud3_fspController_alarms\\\\logs\\\\apicontrolrest\\\\";
	public static final String TARGET = "c:\\\\projects\\\\case-manager-rest-with-metrics-config\\\\cloudformation\\\\cloud3_fspController_alarms\\\\logs\\\\target\\\\";
	public static final String TEMP = "c:\\\\projects\\\\case-manager-rest-with-metrics-config\\\\cloudformation\\\\cloud3_fspController_alarms\\\\logs\\\\temp\\\\";
	public static final String FILE = "000000.gz";
	
	public static void main(String[] args) {
		UnpackGzs gzs = new UnpackGzs();
		
		String[] directories = gzs.getSubfolders(PATH);
		
		for(String dir : directories) {
			String[] dirs2 = gzs.getSubfolders(PATH + dir);
			
			for(String dir2 : dirs2) {
				String arch = PATH + dir + "\\\\" + dir2 + "\\\\" + FILE;
				System.out.println(arch);
				
				String target = TARGET + "\\\\" + dir2 + ".txt";
				
				gzs.gunzipIt(arch, target);
			}
		}

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
	
	 public void gunzipIt(String fileIn, String fileOut){
		 
	     byte[] buffer = new byte[1024];
	 
	     try{
	 
	    	 GZIPInputStream gzis = 
	    		new GZIPInputStream(new FileInputStream(fileIn));
	 
	    	 FileOutputStream out = 
	            new FileOutputStream(fileOut);
	 
	        int len;
	        while ((len = gzis.read(buffer)) > 0) {
	        	out.write(buffer, 0, len);
	        }
	 
	        gzis.close();
	    	out.close();
	 
	    	System.out.println("Done");
	    	
	    }catch(IOException ex){
	       ex.printStackTrace();   
	    }
	   } 

}
