package com.balabas.html.packager;

import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.naming.ConfigurationException;

import org.apache.commons.cli.CommandLine;

import com.balabas.html.packager.EspPackager;

public class PackageConfiguration {

    public static final String REPL_START="${";
    public static final String REPL_END="}";
    
    public static final String BUILD_INFO_FILENAME="build_info.txt";
    public static final String BUILD_INFO_PLACEHOLDER=REPL_START+BUILD_INFO_FILENAME+REPL_END;
    
    public static final int MAX_PLACEHOLDER_LENGTH=50;
    public static final int FAILED_PLACEHOLDER_LENGTH=15;
    
    String sourceFolder;
    String targetFolder;
    String projectName;
    String ip;
    
    String buildInfo;
    boolean deleteHtml=true;
    boolean uploadFiles=false;
    
    boolean ok=true;
    
    List<String> componentFolders=new ArrayList<String>();
    List<String> excludeFolders=new ArrayList<String>();
    
    Map<String,File> sources=new HashMap<String,File>();
    Map<String,File> components=new HashMap<String,File>();
    
    public PackageConfiguration(CommandLine cmd) throws ConfigurationException{
        String baseCompFolder=cmd.getOptionValue(EspPackager.COMMON_COMP);
        String projCompFolder=cmd.getOptionValue(EspPackager.PROJECT_COMP);
        sourceFolder=cmd.getOptionValue(EspPackager.PROJECT_SOURCE);
        targetFolder=cmd.getOptionValue(EspPackager.PROJECT_TARGET);
        
        projectName=cmd.getOptionValue(EspPackager.PROJECT_NAME);
        ip=cmd.getOptionValue(EspPackager.IP);
        
        buildInfo=projectName+" (ver "+new Date()+" ) "+
                    (cmd.hasOption(EspPackager.BUILD_INFO)?cmd.getOptionValue(EspPackager.BUILD_INFO):"");
        deleteHtml=false;
        uploadFiles=false;
                
        if(projectName==null || projectName.isEmpty()){
            throw new ConfigurationException("Project name not specified");
        }
        
        if(baseCompFolder==null || baseCompFolder.isEmpty()){
            throw new ConfigurationException("baseCompFolder not specified");
        }
        
        if(projCompFolder==null || projCompFolder.isEmpty()){
            throw new ConfigurationException("projCompFolder not specified");
        }
        
        if(sourceFolder==null || sourceFolder.isEmpty()){
            throw new ConfigurationException("sourceFolder not specified");
        }
        
        if(targetFolder==null || targetFolder.isEmpty()){
            throw new ConfigurationException("targetFolder not specified");
        }
        
        sourceFolder=FileUtility.checkFolderName(sourceFolder);
        targetFolder=FileUtility.checkFolderName(targetFolder);

        componentFolders.add(FileUtility.checkFolderName(projCompFolder));
        componentFolders.add(FileUtility.checkFolderName(baseCompFolder));
        
        if(componentFolders.size()==0){
            throw new ConfigurationException("Component folder not specified"); 
        }
        excludeFolders.add(sourceFolder);
        excludeFolders.add(targetFolder);
    }
    
    public void scanSources() throws ConfigurationException{
        try{
            scanFolderToMap("Sources", Collections.singletonList(sourceFolder), sources,null,true);
            scanFolderToMap("Components", componentFolders, components,excludeFolders,true);
        }catch(ConfigurationException e){
            System.out.println(e.getMessage());
            ok=false;
            throw e;
        }
        
        ok=true && ok;
    }
    
    private void scanFolderToMap(String head,List<String> scanFolders,Map<String,File> targetMap,List<String> exclude,boolean subfolders) throws ConfigurationException{

        if(scanFolders.size()==0){
            throw new ConfigurationException("No scan Folders specified"); 
        }
        List<String> duplicates=new ArrayList<String>();
        
        System.out.print(head+"...");
        
        for(String folder:scanFolders){
            //System.out.print("Scan "+folder+" ");
            FileUtility.listMap(folder, targetMap ,exclude,duplicates,subfolders);
            
            int mapSize=targetMap.keySet().size();
            
            if(mapSize<1){
                throw new ConfigurationException("No files found");
            }
        }
        
        if(duplicates.size()>0){
            System.out.println();
            System.out.println("-----------Duplicates found-------------");
            for(String str:duplicates){
                System.out.println(str);
            }
            System.out.println("----------------------------------------------");
            throw new ConfigurationException("Duplicated files");
        }else{
            //
        }
        System.out.println(" found ="+targetMap.keySet().size());
        System.out.println("----------------------------------------------");
    }
    
    public Path getProjectComponentsPath(){
        return Paths.get(componentFolders.get(0));
    }
    
    public Path getTargetPath(){
        return Paths.get(targetFolder);
    }
    
    public Path getBuildInfoPath(){
        return Paths.get(componentFolders.get(0)+BUILD_INFO_FILENAME);
    }
    
    public Path getTargetPath(File sourceFile){
        String subPath=sourceFile.getAbsolutePath().replace(sourceFolder, "");
        return Paths.get(targetFolder+subPath);
    }
    
    public void printDetails(){
        System.out.println("------------------------------------------------------------------------");
        System.out.println("PROJECT "+projectName);
        System.out.println("BUILD "+buildInfo);
        System.out.println("------------------------------------------------------------------------");
        System.out.println("Source "+sourceFolder);
        System.out.println("Target "+targetFolder);
        System.out.println("Components folders");
        for(String str:componentFolders){
            System.out.println(str);
        }
        System.out.println("------------------------------------------------------------------------");
    }

    public String getProjectName() {
        return projectName;
    }
    
    public String getIp() {
        return ip;
    }

    public String getBuildInfo() {
        return buildInfo;
    }

    public boolean isDeleteHtml() {
        return deleteHtml;
    }

    public boolean isUploadFiles() {
        return uploadFiles;
    }

    public List<String> getComponentFolders() {
        return componentFolders;
    }

    public List<String> getExcludeFolders() {
        return excludeFolders;
    }

    public boolean isOk() {
        return ok;
    }

    public void setOk(boolean ok) {
        this.ok = ok;
    }

    public Map<String, File> getSources() {
        return sources;
    }
    
    public Collection<File> getSourcesAsList() {
        return sources.values();
    }

    public Map<String, File> getComponents() {
        return components;
    }
    
    
}
