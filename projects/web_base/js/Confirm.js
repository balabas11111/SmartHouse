function openCommandRestartPopup(){
	var message='После сохранения данных рекомендуется перезагрузить устройство';
	
	var inputNames=['remote_target','remote_page','val_json','action'];
	var inputValues=['deviceHelper','manage','restart','restart'];
	
	var btnNames=['Закрыть и перезапустить','Перезагружу позже'];
	var btnToolTips=['',''];
	var btnClasses=['w3-border-red','w3-border-red'];
	var btnOnClicks=['displayRestartDialog();','confirmDlgHide();'];
	
	confirmDlgInit('Подтверждение',message,'',inputNames,inputValues,btnNames,btnToolTips,btnClasses,btnOnClicks,false);
	confirmDlgShow();
}

function displayRestartDialog(){
	openCommandConfirmPopup('Подтвердите рестарт устройства','deviceHelper','restart');
}

function openCommandConfirmPopup(message,target,action,displayCloseBtn){
	
	var inputNames=['remote_target','remote_page','val_json','action'];
	var inputValues=[target,'manage',action,action];
	
	var btnNames=['Подтвердить','Отменить'];
	var btnToolTips=['',''];
	var btnClasses=['w3-border-green','w3-border-red'];
	var btnOnClicks=['submitManageCommand();','confirmDlgHide();'];
	
	confirmDlgInit('Подтверждение',message,'',inputNames,inputValues,btnNames,btnToolTips,btnClasses,btnOnClicks,displayCloseBtn);
	confirmDlgShow();
}

function confirmDlgInit(header,content,msg,inputNames,inputValues,btnNames,btnToolTips,btnClasses,btnOnClicks,displayCloseBtn,show){

	var headerComp=getConfirmHeaderDiv();
	var inputsDiv=getConfirmInputsDiv();
	var contentDiv=getConfirmContentDiv();
	var msgDiv=getConfirmMsgDiv();
	var btnDiv=getConfirmBtnDiv();
	
	headerComp.innerHTML=header;
	inputsDiv.innerHTML='';
	contentDiv.innerHTML=content;
	msgDiv.innerHTML=msg;
	btnDiv.innerHTML='';
	
	for(var i=0;i<btnNames.length;i++){
		var btn=createConfirmButton(i,btnNames[i],btnToolTips[i],btnClasses[i],btnOnClicks[i]);
		btnDiv.appendChild(btn);
	}
	
	var maxWidth=0;
	
	var comps=getComponentChildrenByClass(btnDiv,'w3-btn');
	
	for(var i=0;i<comps.length;i++){
		var ow=comps[i].offsetWidth;
		
		if(ow!=undefined && ow>maxWidth){
			maxWidth=ow;
		}
	}
	
	if(maxWidth>0){
		for(var i=0;i<comps.length;i++){
			comps[i].style.width=maxWidth;
		}
	}
	

	for(var j=0;j<inputNames.length;j++){
		var name=inputNames[j];
		var hid=createHiddenInput('confirm_input_'+name,name,inputValues[j]);
		inputsDiv.appendChild(hid);
	}
	
	if(displayCloseBtn==undefined){
		displayCloseBtn=true;
	} 
	
	confirmDlgCloseButtonVisible(displayCloseBtn);
	
	if(show!=undefined && show==true){
		confirmDlgShow();
	}
}

function confirmDlgShow(){
	showComponent('confirm_modal');
	var msg=getConfirmMsgDiv();
	setStatusMessageComp(msg);
}

function confirmDlgHide(){
	hideComponent('confirm_modal');
	setPrevStatusMessageComp();
}

function confirmDlgCloseButtonVisible(visible){
	var comp=getComponentById('confirm_headerCloseBtn');
	setVisible(comp,visible);
}

function confirmDlgSetHeader(header){
	var headerComp=getConfirmHeaderDiv();
	headerComp.innerHTML=header;
}

function confirmDlgSetContent(content){
	var contentDiv=getConfirmContentDiv();
	contentDiv.innerHTML=content;
}

function confirmDlgSetMsg(msg,clazz){
	var msgDiv=getConfirmMsgDiv();
	msgDiv.innerHTML=msg;
	msgDiv.setAttribute('class','w3-card-4 '+clazz);
}

function confirmDlgGetGetInputValue(name){
	return getComponentById('confirm_input_'+name).value;
}

function confirmDlgRemoveButtons(){
	var btnDiv=getConfirmBtnDiv();
	btnDiv.innerHTML='';
}

function confirmDlgAddButton(id,btnText,tooltip,clazz,onclick){
	var btnDiv=getConfirmBtnDiv();
	var btn=createConfirmButton(id,btnText,tooltip,clazz,onclick);
	btnDiv.appendChild(btn);
}

function getConfirmButtonId(id){
	return 'confirmBtn_'+id;
}

function createConfirmButton(id,btnText,tooltip,clazz,onclick){
	id=getConfirmButtonId(id);
	clazz='w3-btn w3-white w3-border w3-round-large '+clazz;
	var style='margin-top: 2px; margin-bottom: 2px; margin-left:10 px;';
		
	var btnDiv=createDivComponent('','w3-cell w3-tooltip');
	
	if(tooltip!=undefined && tooltip!=''){
		btnDiv.appendChild(createTooltipSpan(tooltip));
	}
	
	var btn=createButtonComponent(id,clazz,style,btnText,onclick);
	
	btnDiv.appendChild(btn);
	
	return btnDiv;
}

var restartTimer=40;
var restartFunctionHandler=undefined;

function startRestart(){
	restartTimer=40;
	setTimeout(function(){tickRestart();}, 1000);
}

function tickRestart(){
	restartTimer--;
	
	if(restartTimer<=2){
		if(restartFunctionHandler==undefined){
			location.reload();
		}else{
			confirmDlgHide();
			restartFunctionHandler();
			return;
		}
	}
		
	var result='Устройство перезапустится через '+restartTimer+' сек';
	
	hideComponent('confirm_msg');
	getComponentById('confirm_content').innerHTML=result;
	
	setTimeout(function(){tickRestart();}, 1000);
}

function getConfirmHeaderDiv(){
	return getComponentById('confirm_header');
}

function getConfirmInputsDiv(){
	return getComponentById('confirm_inputs');
}

function getConfirmContentDiv(){
	return getComponentById('confirm_content');
}

function getConfirmMsgDiv(){
	return getComponentById('confirm_msg');
}

function getConfirmBtnDiv(){
	return getComponentById('confirm_buttons');
}
