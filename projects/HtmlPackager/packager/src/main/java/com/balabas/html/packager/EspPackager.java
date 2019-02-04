package com.balabas.html.packager;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.rmi.NotBoundException;

import javax.naming.ConfigurationException;

import org.apache.commons.cli.ParseException;

public class EspPackager 
{
    public static final String PROJECT_SOURCE="source";
    public static final String PROJECT_TARGET="target";
    public static final String COMMON_COMP="comp1";
    public static final String PROJECT_COMP="comp2";
    
    public static final String PROJECT_NAME="name";
    public static final String IP="ip";
    
    public static final String BUILD_INFO="build";
    public static final String DELETE_HTML="deleteHtml";
    public static final String UPLOAD_FILES="upload";
    /* Application will take all files from PROJECT_SOURCE
     * copy them to target location PROJECT_TARGET
     * 
     * all placeHolders ${fileName} in copied files 
     * will be replaced by file content from 
     * 
     * COMMON_COMP, PROJECT_COMP. Built gz will be uploaded as POST to http://%IP%/edit
     *
     * java -jar EspHtmlPackager.jar -name "%PROJECT_NAME%" -source "%PROJECT_SOURCE%" -target "%PROJECT_TARGET%" -comp1 "%COMMON_COMP%" -comp2 "%PROJECT_COMP%" -ip "%IP%" -upload "True" -deleteHtml "True"
     */
    
    
    public static void main( String[] args ) throws Exception
    {
/*
        baseFolder="D:\\projects\\!!!SmartHouse\\projects\\";
        projectName="ESP8266NtpPir";
        ip="192.168.0.120";
        
        buildInfo="buildInfo";
        deleteHtml=false;
        uploadFiles=true;
  */      
        EspPackager esp=new EspPackager();
        
        PackageConfiguration conf=esp.createConfiguration(args);
        esp.executeConfiguration(conf);
    }
    
    private PackageConfiguration createConfiguration(String[] args) throws ConfigurationException, ParseException{
        System.out.println("------------------------------------------------------------------------");
        System.out.println("EspHtmlPackager");
        System.out.println("------------------------------------------------------------------------");
        
        PackageConfiguration conf=ConfigurationUtility.parseConfiguration(args);
        
        return conf;
    }
    
    private void executeConfiguration(PackageConfiguration conf){
        boolean ok=false;
        
        try{
            conf.printDetails();
            saveBuildInfo(conf);
            scanFolders(conf);
            recreateTargetFolder(conf);
            resolvePlaceHolders(conf);
            uploadFiles(conf);
            
            ok=conf.isOk();
        
        }catch(Exception e){
            e.printStackTrace();
            System.out.println("Exception "+e.getMessage());
        }
        
        System.out.println("------------------------------------------------------------------------");
        System.out.println(" BUILD "+(ok?"COMPLETE":"FAILURE"));
        System.out.println("------------------------------------------------------------------------");
    }
    
    private void saveBuildInfo(PackageConfiguration conf) throws IOException{
        FileUtility.deleteFile(conf.getBuildInfoPath().toFile());
        FileUtility.saveStringToFile(conf.getBuildInfoPath(), conf.getBuildInfo());
        System.out.println("Build info saved");
    }
    
    private void scanFolders(PackageConfiguration conf) throws ConfigurationException{
        System.out.println("-----------------------------------------------------");
        System.out.println("Scanning...");
        System.out.println("-----------------------------------------------------");
        
        conf.scanSources();
        
        System.out.println("-----------------------------------------------------");
    }
    
    private void recreateTargetFolder(PackageConfiguration conf){
        Path target=conf.getTargetPath();
        FileUtility.deleteDirectory(target.toFile());
        
        File file = target.toFile();
        if (!file.exists()) {
            if (file.mkdir()) {
                System.out.println("Target folder reCreated");
                System.out.println("-----------------------------------------------------");
            }
        }
    }
    
    private void resolvePlaceHolders(PackageConfiguration conf) throws ConfigurationException, IOException, NotBoundException{
            System.out.println("---------------Replace placeholders------------------");

            int totalReplaced=0;
            
            for(File sourceFile:conf.getSourcesAsList()) {
                
                System.out.println("----------------------------------------------");
                System.out.print("process "+sourceFile.getName()+" ");
                
                int replaced=0;
                boolean doReplace=true;
            
                String sourceContent = FileUtility.getFileAsString(sourceFile);
                
                while (doReplace) {
                
                    Replacement repl = ReplaceUtility.findReplacement(sourceContent);
                    
                    if(repl==null){
                        //System.out.println("No replacements found");
                        doReplace=false;
                    }else{
                        sourceContent=ReplaceUtility.processPlaceHolder(repl, sourceContent, conf.getComponents());
                        replaced++;
                    }
                }
                
                totalReplaced+=replaced;
                System.out.println(" replace count="+replaced);
                
                Path targetPath=conf.getTargetPath(sourceFile);
                
                Path result=FileUtility.saveStringToFile(targetPath, sourceContent);
                
                //System.out.println(" target="+targetPath);
                //System.out.println(" content="+sourceContent);
                //
                
                String gzipFileName=GzipUtility.compressGzipFile(targetPath);
                System.out.println(" saved="+gzipFileName);
                
                if(conf.deleteHtml){
                    //FileUtility.deleteFile(targetPath.toFile()); 
                }
                System.out.println("----------------------------------------------");
            }
            
            System.out.println("----------------------------------------------");
            System.out.println("Placeholders done total ="+totalReplaced);
            System.out.println("==============================================");
       }
    
    private void uploadFiles(PackageConfiguration conf) throws Exception{
        if(conf.isUploadFiles()){
            UploadUtility.uploadBuiltFiles(conf.getTargetPath().toFile(), conf.getIp());
        }
    }

}