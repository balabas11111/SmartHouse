/*----------------------------Status messages Logic-----------------------------------------*/
var currentMessageComp=undefined;
var previousMessageComp=undefined;

function setStatusMessageComp(msgComp){
	if(currentMessageComp!=undefined){
		previousMessageComp=currentMessageComp;
	}
	currentMessageComp=msgComp;
	return currentMessageComp;
}

function setPrevStatusMessageComp(){
	if(getPrevMessageComp()!=undefined){
		var msgComp=getPrevMessageComp();
		return setStatusMessageComp(msgComp);
	}
	
	return getStatusMessageComp();
}

function getStatusMessageComp(){
	return currentMessageComp;
}

function getPrevMessageComp(){
	return previousMessageComp;
}

function showStatusMessage(message,className){
	showMessage(currentMessageComp,message,className);
}

function showMessage(msgComp,message,className){
	if(msgComp!=undefined){
		if(className!=undefined && className!=''){
			msgComp.setAttribute('class',className);
		}
		
		if(message!=undefined && message!=''){
			msgComp.style.display = "block";
			msgComp.innerHTML = message;
		}else{
			msgComp.style.display = "none"; 
		}
	}
}