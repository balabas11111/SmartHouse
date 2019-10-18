package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Date;

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
    
    public boolean isActive(){
    	return this.nextActionTime > -1;
    }
    
    public void setInactive(){
    	this.nextActionTime = -1;
    }
    
    public boolean isTimeToExecuteAction() {
    	if(this.nextActionTime > -1){
            return this.nextActionTime < (new Date()).getTime();
        }
    	return false;
    }
    
    public boolean isForcedAndTimeToExecute(){
    	return isActionForced() && isTimeToExecuteAction();
    }
    
    public boolean isForcedOrTimeToExecute(){
    	return isActionForced() || isTimeToExecuteAction();
    }

	public void update(long incrementMs, boolean force) {
		setActionForced(force);
		setNextActionTime(incrementMs);
	}
}
