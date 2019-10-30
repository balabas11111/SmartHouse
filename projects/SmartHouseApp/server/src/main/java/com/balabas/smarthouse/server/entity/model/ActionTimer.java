package com.balabas.smarthouse.server.entity.model;

import java.util.Date;

import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Log4j2
public class ActionTimer {

	@Getter
    private long interval;
    
    @Getter
    private Date registrationTime;
    
    @Getter
    private long lastSuccessActionTime;
    
    @Getter
    private long nextActionTime = -1;
    
    @Getter @Setter
    private boolean actionForced;
    
    public ActionTimer(long interval) {
        this.interval = interval;
        this.registrationTime = new Date();
        
        log.debug("new UpdateTimer i="+interval);
    }
    
    public void setActionSuccess(){
        update(interval, false);

        this.lastSuccessActionTime = (new Date()).getTime();
    }
    
    public void setActionFailed(){
    	update(interval, false);
    }
    
    public void setNextActionTime(long incrementMs) {
        if(incrementMs>-1){
        	 this.nextActionTime =(new Date()).getTime() + incrementMs;
        }else{
            this.nextActionTime = -1;
        }
    }
    
    public void setNextActionTimeAsNow() {
        this.nextActionTime =(new Date()).getTime();
    }
    
    @JsonIgnore
    public boolean isActive(){
    	return this.nextActionTime > -1;
    }
    
    public void setInactive(){
    	this.nextActionTime = -1;
    }
    
    @JsonIgnore
    public boolean isTimeToExecuteAction() {
    	if(this.nextActionTime > -1){
            return this.nextActionTime <= (new Date()).getTime();
        }
    	return false;
    }
    
    @JsonIgnore
    public boolean isForcedAndTimeToExecute(){
    	return isActionForced() && isTimeToExecuteAction();
    }
    
    @JsonIgnore
    public boolean isForcedOrTimeToExecute(){
    	return isActionForced() || isTimeToExecuteAction();
    }

	public void update(long incrementMs, boolean force) {
		setActionForced(force);
		setNextActionTime(incrementMs);
	}

	public void setMessageInterval(long interval) {
		this.interval = interval;
	}
}
