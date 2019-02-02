package com.balabas.html.packager;

import java.io.File;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class UploadUtility {

    public static final String CRLF = "\r\n";
    public static final String charset = "UTF-8";
    
    public static void deleteAndUpload(String folder,String ip) throws Exception{
        List<File> files=new ArrayList<File>();

        FileUtility.listf(folder, files,null,true);
        
        System.out.println("total "+files.size()+" files in "+folder);
        
        for(File file:files){
            deleteFile(Paths.get(file.getPath()),ip);
            uploadFile(Paths.get(file.getPath()),ip);
        }
    }
    
    public static boolean deleteFile(Path path,String ip)throws Exception{
        int code =0;
        try{
            System.out.print("delete "+path.getFileName());
            
            URL url = constructUrl(ip);
            HttpURLConnection httpCon = (HttpURLConnection) url.openConnection();
            httpCon.setDoOutput(true);
            httpCon.setRequestProperty("filename", path.toString());
            //httpCon.setRequestProperty("Content-Type", "application/json");
            httpCon.setRequestMethod("DELETE");
            OutputStreamWriter out = new OutputStreamWriter(httpCon.getOutputStream());
            
            httpCon.connect();
            
            code = httpCon.getResponseCode();
            
            System.out.print("... OK");
        }catch(Exception e){
            System.out.print("...FAILED "+code);
            return false;
        }
        
        return true;
    }
    
    public static boolean uploadFile(Path path,String ip)throws Exception{
        boolean ok=true;
        int code=0;
        
        try{
        System.out.print("upload "+path.getFileName());
        String boundary = Long.toHexString(System.currentTimeMillis());
        
        URLConnection connection = constructUrl(ip).openConnection();
        connection.setDoOutput(true);
        connection.setRequestProperty("Content-Type", "multipart/form-data; boundary=" + boundary);

        try (
            OutputStream output = connection.getOutputStream();
            PrintWriter writer = new PrintWriter(new OutputStreamWriter(output, charset), true);
        ) {
            // Send normal param.
            writer.append("--" + boundary).append(CRLF);
            writer.append("Content-Disposition: form-data; name=\"param\"").append(CRLF);
            writer.append("Content-Type: text/plain; charset=" + charset).append(CRLF);
            //writer.append(CRLF).append(param).append(CRLF).flush();

            // Send text file.
            writer.append("--" + boundary).append(CRLF);
            writer.append("Content-Disposition: form-data; name=\"textFile\"; filename=\"" + path.getFileName() + "\"").append(CRLF);
            writer.append("Content-Type: text/plain; charset=" + charset).append(CRLF); // Text file itself must be saved in this charset!
            writer.append(CRLF).flush();
            Files.copy(path, output);
            output.flush(); // Important before continuing with writer!
            writer.append(CRLF).flush(); // CRLF is important! It indicates end of boundary.

            // End of multipart/form-data.
            writer.append("--" + boundary + "--").append(CRLF).flush();
            code = ((HttpURLConnection) connection).getResponseCode();
        }catch(Exception e){
            ok=false;
        }
        }catch(Exception e){
            ok=false;
        }
        
        System.out.println("... "+(ok?"OK ":"FAILED ")+code);
        return ok;
    }
    
    public static URL constructUrl(String ip) throws Exception{
        URL url=new URL("http://"+ip+"/edit");
        return url;
    }
}
