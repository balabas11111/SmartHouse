package com.balabas.html.packager;

public class EspPackager 
{
    
    public static void main( String[] args ) throws Exception
    {
        String baseFolder="D:\\projects\\!!!SmartHouse\\projects\\";
        String projectName="ESP8266NtpPir";
        String buildInfo="buildInfo";
        String ip="192.168.0.120";
        boolean deleteHtml=false;
        boolean uploadFiles=true;
        
        String targetFolder=baseFolder+projectName+"\\web\\target\\";
                
        ReplaceUtil.replacePlaceHolders(baseFolder, projectName, buildInfo, targetFolder, deleteHtml);
        
        if(uploadFiles){
            UploadUtility.deleteAndUpload(targetFolder, ip);
        }
    }

}