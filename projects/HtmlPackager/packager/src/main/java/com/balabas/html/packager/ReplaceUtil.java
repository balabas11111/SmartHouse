package com.balabas.html.packager;

import java.io.File;
import java.io.FileInputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.io.IOUtils;

public class ReplaceUtil {

    public static final String REPL_START="${";
    public static final String REPL_END="}";
    
    public static final String BUILD_INFO_PLACEHOLDER="${build info}";
    
    public static void replacePlaceHolders(String baseFolder,String projectName,String buildInfo,
            String targetFolder,
            boolean deleteHtml)throws Exception{
        String htmlFolder=baseFolder+projectName+"\\web";
        String projectComponentsFolder=baseFolder+projectName+"\\web\\!projectPages\\";
               
        String commonComponentsFolder=baseFolder+"web_base\\";
        
        System.out.println("-----------------------------------------------------");
        System.out.println("html folder="+Paths.get(htmlFolder).toAbsolutePath());
        System.out.println("project comp folder="+projectComponentsFolder);
        System.out.println("common  comp folder="+commonComponentsFolder);
        System.out.println("-----------------------------------------------------");
        
        List<String> excludeFolders=new ArrayList<String>();
        
        excludeFolders.add(projectComponentsFolder);
        excludeFolders.add(targetFolder);
        
        List<File> components=new ArrayList<File>();
        List<File> projectFiles=new ArrayList<File>();

        FileUtility.listf(commonComponentsFolder, components,excludeFolders,true);
        FileUtility.listf(htmlFolder, components,excludeFolders,true);
        
        FileUtility.listf(projectComponentsFolder, projectFiles,null,false);
        
        FileUtility.deleteDirectory(new File(targetFolder));
        
        File file = new File(targetFolder+"\\");
        if (!file.exists()) {
            if (file.mkdir()) {
                System.out.println("Target folder created");
            }
        }
        
        FileUtility.printFiles(components,"Component files");
        FileUtility.printFiles(projectFiles,"Project files");
        
        System.out.println("-----Replace placeholders-----");
        for(File f:projectFiles) {
            
            System.out.println("-------------------------");
            System.out.println("process "+f.toPath());
            System.out.println("-------------------------");
            
            String content = IOUtils.toString(new FileInputStream(f),"UTF-8");
            boolean replace=true;
            
            while (replace) {
                
                if(content.indexOf(REPL_START)==-1){
                    replace=false;
                }else{
                    replace=true;
                }
                
                replace=false;
                for(int i=0;i<components.size();i++){
                    
                    File cf=components.get(i);
                    String lookupStr=REPL_START+cf.getName()+REPL_END;
                        
                    int ind=content.indexOf(lookupStr);
                    
                    if(ind>-1){
                        int ind2=content.indexOf(REPL_END,ind);
                        String found=content.substring(ind, ind2+1);
                        String repl = IOUtils.toString(new FileInputStream(cf),"UTF-8");
                        
                        String before=content.substring(0, ind);
                        String after=content.substring(ind2+1);
                        
                        System.out.println(" replace "+found);
                        content = before+repl+after;
                        
                        replace=true;
                    }
                }
            }
            
            content=content.replace(BUILD_INFO_PLACEHOLDER, buildInfo);
            
            String targetPathStr=targetFolder+f.getName();
            Path filePath=Paths.get(targetPathStr);
            System.out.println("Save "+filePath);
            Files.write(filePath, content.getBytes("UTF-8"));
            
            GzipUtility.compressGzipFile(targetPathStr, targetPathStr+".gz");
            
            if(deleteHtml){
                Files.delete(filePath);
            }
            
            System.out.println("======================================");
        }
        
            
        }
}
