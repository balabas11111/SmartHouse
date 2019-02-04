package com.balabas.html.packager;

import java.io.File;
import java.io.IOException;
import java.rmi.NotBoundException;
import java.util.Map;

import javax.naming.ConfigurationException;

public class ReplaceUtility {

    public static String getPlaceHolderKey(String fileName){
        return PackageConfiguration.REPL_START+fileName+PackageConfiguration.REPL_END;
    }
    
    public static Replacement findReplacement(String content) throws ConfigurationException{
        int ind=content.indexOf(PackageConfiguration.REPL_START);
        if(ind>-1){
            int ind2=content.indexOf(PackageConfiguration.REPL_END,ind);
            
            if(ind2==-1){
                ind2=(ind+PackageConfiguration.FAILED_PLACEHOLDER_LENGTH>content.length())?content.length()-1:ind+PackageConfiguration.FAILED_PLACEHOLDER_LENGTH;
                throw new ConfigurationException("Placeholder start found but no finish tag "+content.substring(ind, ind2)); 
            }
            if(ind2-ind>PackageConfiguration.MAX_PLACEHOLDER_LENGTH){
                throw new ConfigurationException("Placeholder length > "+PackageConfiguration.MAX_PLACEHOLDER_LENGTH+" "+content.substring(ind, ind2));
            }
            
            String key=content.substring(ind, ind2+1);
            
            return new Replacement(ind, ind2+1, key);
        }
        
        return null;
    }
    
    public static String processPlaceHolder(Replacement repl,String content,Map<String,File> replacements) throws ConfigurationException, IOException, NotBoundException{
        if(repl==null || repl.getKey()==null){
            throw new ConfigurationException("replacement not specified");
        }
        
        if(replacements.keySet().size()==0){
            throw new ConfigurationException("Replacement files not found");
        }
        if(!replacements.containsKey(repl.getKey())){
            throw new NotBoundException("key "+repl+" not found in filesList");
        }
        
        String before=content.substring(0, repl.getStart());
        String after=content.substring(repl.getEnd());
        
        File replacementFile=replacements.get(repl.getKey());
        
        String replacementText=FileUtility.getFileAsString(replacementFile);
        
        content = before+replacementText+after;
        
        return content;
    }
    
   
}
