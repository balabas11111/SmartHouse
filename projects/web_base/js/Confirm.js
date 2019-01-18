function confirmDlgInit(header,content,msg,inputNames,inputValues,btnNames,btnToolTips,btnClasses,btnOnClicks,displayCloseBtn){

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

	for(var j=0;j<inputNames.length;j++){
		var name=inputNames[i];
		createHiddenInput('confirm_input_'+name,name,inputValues[i]);
	}
	
	if(displayCloseBtn==undefined){
		displayCloseBtn=true;
	} 
	
	confirmDlgCloseButtonVisible(displayCloseBtn);
	
}

function confirmDlgShow(){
	showComponent('confirm_modal');
}

function confirmDlgHide(){
	hideComponent('confirm_modal');
}

function confirmDlgCloseButtonVisible(visible){
	var comp=getComponentById('confirm_headerCloseBtn');
	setVisible(comp,visible)
}

function confirmDlgSetHeadert(header){
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

function createConfirmButton(id,btnText,tooltip,clazz,onclick){
	id='confirmBtn_'+id;
	clazz='w3-btn w3-white w3-border w3-round-large '+clazz;
	var style='margin-top: 2px; margin-bottom: 2px;'
		
	var btnDiv=createDivComponent('','w3-cell w3-tooltip');
	var tooltipSpan=createHtmlComponent('span','','','w3-text w3-tag','position:absolute;left:0;bottom:20px',tooltip);
	var btn=createButtonComponent(tag,id,clazz,style,btnText,onclick)

	btnDiv.appendChild(tooltipSpan);
	btnDiv.appendChild(btn);
	
	return btnDiv;
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
