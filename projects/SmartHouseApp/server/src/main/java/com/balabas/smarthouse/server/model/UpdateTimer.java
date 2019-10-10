package com.balabas.smarthouse.server.model;

import java.util.Date;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.extern.log4j.Log4j2;

@Log4j2
@NoArgsConstructor
public class UpdateTimer {

    private String parentName;
    
    private long updateInterval;
    
    @Getter
    private Date registrationTime;
    
    @Getter
    private long lastUpdateTime;
    
    private long nextUpdateTime = -1;
    
    private boolean waitsForDataUpdate;
    
    public UpdateTimer(SmartHouseItem parent,  long updateInterval) {
        this.parentName = parent.getName();
        this.updateInterval = updateInterval;
        this.registrationTime = new Date();
        
        log.debug("new UpdateTimer("+parentName+") i="+updateInterval);
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
    	setNextTimeToUpdate(this.updateInterval);
    }
    
    public void setNextTimeToUpdate(long incrementMs) {
        if(incrementMs>-1){
        	 this.nextUpdateTime =(new Date()).getTime() + incrementMs;
        }else{
            log.error("Wrong updateInterval");
        }

    }
    
    public boolean isWaitsForDataUpdate() {
    	return this.waitsForDataUpdate;
    }
    
    public boolean isNextUpdateTimeReached() {
    	if(this.nextUpdateTime > -1){
            return this.nextUpdateTime < (new Date()).getTime();
        }
    	return false;
    }
}
