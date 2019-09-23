package com.balabas.smarthouse.server.model;

import java.util.Date;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.extern.log4j.Log4j2;

@Log4j2
@NoArgsConstructor
public class UpdateTimer {

    private String parentName;
    
    private String parentClass;
    
    private long updateInterval;
    
    @Getter
    private Date registrationTime;
    
    private long nextUpdateTime = -1;
    
    private boolean waitsForDataUpdate;
    
    public UpdateTimer(NameAble parent,  long updateInterval) {
        this.parentName = parent.getName();
        this.parentClass = parent.getClass().getSimpleName();
        this.updateInterval = updateInterval;
        this.registrationTime = new Date();
    }
    
    public void setWaitsForDataUpdate(boolean value){
        this.waitsForDataUpdate = value;
    }
    
    public void setDataReceived(){
        setWaitsForDataUpdate(false);
        setNextTimeToUpdate();
        
        log.info(this.parentClass+"("+this.parentName+") DataUpdate completed");
    }
    
    public void setNextTimeToUpdate(){
        if(updateInterval>-1){
            this.nextUpdateTime = (new Date()).getTime() + updateInterval;
        }else{
            log.error("Wrong updateInterval");
        }
    }
    
    public void setNextTimeToUpdate(long nextTime){
        if(nextTime>-1){
            this.nextUpdateTime = nextTime;
        }
    }
    
    public boolean isTimeToUpdate(){
        boolean result = this.waitsForDataUpdate; 
        
        if(this.nextUpdateTime > -1){
            long now = (new Date()).getTime();
            
            boolean timeResult = this.nextUpdateTime < now;
            result = this.waitsForDataUpdate || timeResult;
            
            if(timeResult){
                log.info(this.parentClass+"("+this.parentName+") Data is OLD nTime"+nextUpdateTime+" now="+now);
            }
        }
        
        if(result){
            log.info(this.parentClass+"("+this.parentName+") Waits for data update");
        }
        
        return result;
    }
   
}
