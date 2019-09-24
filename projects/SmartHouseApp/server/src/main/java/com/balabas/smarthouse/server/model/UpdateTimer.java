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
    
    @Getter
    private long lastUpdateTime;
    
    private long nextUpdateTime = -1;
    
    private boolean waitsForDataUpdate;
    
    public UpdateTimer(NameAble parent,  long updateInterval) {
        this.parentName = parent.getName();
        this.parentClass = parent.getClass().getSimpleName();
        this.updateInterval = updateInterval;
        this.registrationTime = new Date();
        
        log.info("new UpdateTimer("+parentName+") i="+updateInterval);
    }
    
    public void setWaitsForDataUpdate(boolean value){
        this.waitsForDataUpdate = value;
    }
    
    public void setDataReceived(){
        setWaitsForDataUpdate(false);
        setNextTimeToUpdate();

        this.lastUpdateTime = (new Date()).getTime();
        //log.info(this.parentClass+"("+this.parentName+") DataUpdate completed");
    }
    
    public void setDataUpdateFailed(){
        setWaitsForDataUpdate(false);
        setNextTimeToUpdate();
    }
    
    private void setNextTimeToUpdate(){
        if(updateInterval>-1){
            Date now = new Date();
            this.nextUpdateTime = now.getTime() + updateInterval;
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
        boolean waits = this.waitsForDataUpdate;
        boolean timeResult = false;
        
        if(this.nextUpdateTime > -1){
            Date now = new Date();
            long time = now.getTime();
            
            timeResult = this.nextUpdateTime < time;
            /*
            if(timeResult){
                log.info(this.parentClass+"("+this.parentName+") Data is OLD lastUpdated="+lastUpdated+" now="+now);
            }*/
        }
        /*
        if(waits){
            log.info(this.parentClass+"("+this.parentName+") Waits for data update");
        }
        */
        
        return waits || timeResult;
    }
   
}
