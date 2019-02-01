package com.balabas.html.packager;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args ) throws Exception
    {
        System.out.println( "Hello World!" );
        
        String pathProjectFolder="D:\\projects\\!!SmartHouse\\projects\\ESP8266NtpPir\\web";
        String pathBaseProjectFolder="D:\\projects\\!!SmartHouse\\projects\\ESP8266NtpPir\\web\\base";
        String targetProjectFolder="D:\\projects\\!!SmartHouse\\projects\\ESP8266NtpPir\\web\\target";
               
        String pathBaseCompFolder="D:\\projects\\!!SmartHouse\\projects\\web_base";
        
        List<String> excludeFolders=new ArrayList<String>();
        
        excludeFolders.add(pathBaseProjectFolder);
        excludeFolders.add(targetProjectFolder);
        
        List<File> baseProjectFiles=new ArrayList<File>();
        List<File> compProjectFiles=new ArrayList<File>();
        List<File> compbaseFiles=new ArrayList<File>();
        List<File> targetFiles=new ArrayList<File>();
        
        FileUtility.listf(pathBaseProjectFolder, baseProjectFiles,null);
        FileUtility.listf(pathProjectFolder, compProjectFiles,excludeFolders);
        FileUtility.listf(pathBaseCompFolder, compbaseFiles,null);
        
        Path targetPath = Paths.get(targetProjectFolder+"\\");
        
        FileUtility.deleteDirectory(new File(targetProjectFolder));
        
        Files.createDirectories(targetPath);
        
        for(File f:baseProjectFiles){

            Path result = Files.copy(f.toPath(), targetPath,StandardCopyOption.REPLACE_EXISTING);
            targetFiles.add(new File(result.toUri()));
        }
        
        FileUtility.printFiles(compbaseFiles,"Common compon files");
        FileUtility.printFiles(compProjectFiles,"Project compon files");
        FileUtility.printFiles(baseProjectFiles,"Base project files");
        FileUtility.printFiles(targetFiles,"Project target files");
    }
}
