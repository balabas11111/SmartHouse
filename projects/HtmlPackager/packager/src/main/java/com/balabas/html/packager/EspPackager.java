package com.balabas.html.packager;

import java.util.Date;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

public class EspPackager 
{
    public static final String BASE_FOLDER="baseFolder";
    public static final String PROJECT_NAME="projectName";
    public static final String BUILD_INFO="buildInfo";
    public static final String IP="ip";
    public static final String DELETE_HTML="deleteHtml";
    public static final String UPLOAD_FILES="upload";
    
    public static void main( String[] args ) throws Exception
    {
        Options options = new Options();
        options.addOption(BASE_FOLDER, true, "base folder of smart house projects");
        options.addOption(PROJECT_NAME, true, "Project name");
        options.addOption(BUILD_INFO, false, "Build info");
        options.addOption(IP, true, "Device Ip");
        options.addOption(DELETE_HTML, true, "Delete html files after created gz");
        options.addOption(UPLOAD_FILES, true, "Upload gz files to device");
        
        CommandLineParser parser = new DefaultParser();
        CommandLine cmd = parser.parse( options, args);
        
        String baseFolder=cmd.getOptionValue(BASE_FOLDER);
        String projectName=cmd.getOptionValue(PROJECT_NAME);
        String ip=cmd.getOptionValue(IP);
        
        String buildInfo=(cmd.hasOption(BUILD_INFO))?
                cmd.getOptionValue(BUILD_INFO)
                :projectName+" (ver "+new Date()+" )";
        boolean deleteHtml=(cmd.hasOption(DELETE_HTML))?
                Boolean.parseBoolean(cmd.getOptionValue(DELETE_HTML))
                :true;
        boolean uploadFiles=(cmd.hasOption(UPLOAD_FILES))?
                Boolean.parseBoolean(cmd.getOptionValue(UPLOAD_FILES))
                :false;

        baseFolder="D:\\projects\\!!!SmartHouse\\projects\\";
        projectName="ESP8266NtpPir";
        ip="192.168.0.120";
        
        buildInfo="buildInfo";
        deleteHtml=false;
        uploadFiles=true;
                
        System.out.println("------------------------------------------------------------------------");
        System.out.println("PROJECT "+projectName);
        System.out.println("BUILD "+buildInfo);
        System.out.println("------------------------------------------------------------------------");
        System.out.println("Parameters");
        System.out.println("------------------------------------------------------------------------");
        
        for(Option opt:cmd.getOptions()){
            System.out.println(opt.getOpt()+"="+opt.getValue());
        }
        
        System.out.println(UPLOAD_FILES+"="+uploadFiles);
        System.out.println(DELETE_HTML+"="+deleteHtml);
                
        String targetFolder=baseFolder+projectName+"\\web\\target\\";
                
        ReplaceUtil.replacePlaceHolders(baseFolder, projectName, buildInfo, targetFolder, deleteHtml);
        
        if(uploadFiles){
            System.out.println("------------------------------------------------------------------------");
            System.out.println("Upload files");
            System.out.println("------------------------------------------------------------------------");
            UploadUtility.deleteAndUpload(targetFolder, ip);
            System.out.println("------------------------------------------------------------------------");
        }
        
        System.out.println("------------------------------------------------------------------------");
        System.out.println(" BUILD COMPLETE");
        System.out.println("------------------------------------------------------------------------");
    }

}