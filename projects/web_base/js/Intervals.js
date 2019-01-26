/*-----------------------Intervals values processing--------------------------------*/
var intSettings={
		rescheduleIndex: 5,
		multidailyIndex: 3,
		periodicIndex: 1
};

var customIntervalsMessage=undefined;

function openIntervalsTab(){
	preventFormSubmit('intervals_form');
	preventFormSubmit('intervals_cur_form');
	
	document.title='Установки интервалов времени';

	hideComponentsByClassName('settingsTab');
	document.getElementById('intervals').style.display = "block";
	
	setStatusMessageComp(document.getElementById('intervals_msg'));
	showStatusMessage('Загружаю интервалы...','w3-yellow');
	
	var url='/getJson?name=timeIntervalService';
	updateComponentsByAjaxCall('GET', url, processIntervalsJsonGet,"",undefined, 0,2000);
}
function processIntervalsJsonGet(data){
	var fields=["rescheduleIndex","multidailyIndex","periodicIndex"];
	for(var i in fields){
		if(data[fields[i]]!=undefined){
			intSettings[fields[i]]=data[fields[i]];
		}
	}
	
	var container=document.getElementById('intervals_form');
	var countDiv=document.getElementById('intervals_count');
	var templateName='intervals-template';
	
	var intervals=data.intervals;
	var count=data.itemCount;
	
	countDiv.innerHTML=(count>0)?('Всего интервалов '+count):'Не задано ни одного интервала';
	container.innerHTML='';
			
	for(var i in intervals){
		var interval=intervals[i];
		var suffix='_'+interval.id;
		
		interval.daysArr=interval.days.split(',');
		putIntervalToForm(templateName,container,interval,true,suffix);
	}
	
	var msg=(customIntervalsMessage!=undefined)?customIntervalsMessage:('Интервалы загружены');
	
	showStatusMessage(msg,'w3-green');
	customIntervalsMessage=undefined;
}
function showNewIntervalForm(){
	var dt=new Date();
	var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
	
	var interval={};
	
	interval.id=-1;
	interval.name='Будильник';
	interval.type=0;
	interval.state=0;
	interval.startTime=tmp+60;
	interval.endTime=tmp+120;
	interval.time='0';
	interval.days='0,0,0,0,0,0,0';
	interval.daysArr=interval.days.split(',');
	interval.kind=0;
	
	var header='➲ Создание нового интервала';
	openIntervalPopup(interval,header);
}
function showEditIntervalForm(id){

	var ids=['id','name','type','state','startTime','endTime','time','kind'];
	
	var interval={};
	interval.daysArr=[];
	
	for(var i=0;i<ids.length;i++){
		interval[ids[i]]=getComponentValue(document.getElementById('intervals_'+ids[i]+'_'+id));
	}
	for(var i=0;i<7;i++){
		interval.daysArr[i]=getComponentValue(document.getElementById('intervals_days_'+i+'_'+id));
	}
		
	var header='➲ Редактирование : '+interval.name;
	openIntervalPopup(interval,header);
}
function openIntervalPopup(interval,header){
	var suffix='';
	var templateName='intervals-template';
	
	var headComp=getComponentById('intervals_cur_header');
	var container=getComponentById('intervals_cur_content');
	var msgComp=getComponentById('intervals_cur_msg');
	
	headComp.innerHTML=header;
	container.innerHTML='';
	
	msgComp.innerHTML='';
	setStatusMessageComp(msgComp);
	
	putIntervalToForm(templateName,container,interval,false,suffix);
	
	showComponent('intervals_cur_modal');
}
function putIntervalToForm(templateName,container,interval,disabled,suffix){
	var template=document.getElementById(templateName).content.cloneNode(true);
	
	template.querySelector('div.intervals').id = 'intervals'+suffix;
	
	if(!disabled){
		var editBtn=template.querySelector('div.editBtn');
		var delBtn=template.querySelector('div.deleteBtn');
		
		editBtn.parentNode.removeChild(editBtn);
		delBtn.parentNode.removeChild(delBtn);
		
	}else{	
		template.querySelector('span.editBtn').innerText = '➲ Редактирование '+interval.name;
		template.querySelector('span.deleteBtn').innerText = '➲ Удаление '+interval.name;
		
		template.querySelector('button.editBtn').setAttribute('onclick',"showEditIntervalForm("+interval.id+");");
		template.querySelector('button.deleteBtn').setAttribute('onclick',"showDeleteIntervalForm("+interval.id+");");
	}
	
	var fields=['id','name','type','state','startTime','endTime','time','kind'];
	
	for(var i=0;i<fields.length;i++){
		updateTemplateValue(template,'intervals',fields[i],interval[fields[i]],suffix,disabled);
	}
	
	for(var i=0;i<7;i++){
		updateTemplateValue(template,'intervals','days_'+i,interval.daysArr[i],suffix,disabled);
	}
	
	var daysDiv=template.querySelector('div.days');
	var timeDiv=template.querySelector('div.time');
	var kindDiv=template.querySelector('div.kind');
	
	daysDiv.id='intervals_daysDiv'+suffix;
	timeDiv.id='intervals_timeDiv'+suffix;
	kindDiv.id='intervals_kindDiv'+suffix;
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,interval.type);

	container.appendChild(template);
}
function handlePeriodTypeChange(selectedIndex){
	var daysDiv=getComponentById('intervals_daysDiv');
	var timeDiv=getComponentById('intervals_timeDiv');
	var kindDiv=getComponentById('intervals_kindDiv');
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,selectedIndex);
}
function applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,type){
	var isMultidaily=(type==intSettings.multidailyIndex);
	var isPeriodic=(type==intSettings.periodicIndex);
	var isKindVis=true;
	
	setVisible(daysDiv,isMultidaily);
	setVisible(timeDiv,isPeriodic);
	setVisible(kindDiv,isKindVis);
}
function submitIntervalsFormAsJsonReloadCurrTab(){
	showStatusMessage('Сохраняю интервалы ...','w3-yellow');
	
	var form=document.getElementById('intervals_cur_form');
	var submitIntervalsUrl='/processJson?name=timeIntervalService';
	
	postForm(form,submitIntervalsUrl,validateCurrentIntervalForm,constructIntervalsFormDataAsJson,validateStatusMessageDefault,onIntervalSavedHandler,getStatusMessageComp());
}
function validateCurrentIntervalForm(){
	var form=document.getElementById('intervals_cur_form');
	
	markFormAsValid(form,getStatusMessageComp(),true,true);
	markComponentValidityById('days',true);
	
	var errorMessage='';
	
	var idInput=document.getElementById('intervals_id');
	var nameInput=document.getElementById('intervals_name');
	var typeSelect=document.getElementById('intervals_type');
	var stateSelect=document.getElementById('intervals_state');
	var startTimeInput=document.getElementById('intervals_startTime');
	var endTimeInput=document.getElementById('intervals_endTime');
	
	var daysDiv=document.getElementById('intervals_days');
	var timeInput=document.getElementById('intervals_time');
	var kindSelect=document.getElementById('intervals_kind');
	
	if(nameInput==undefined 
			|| typeSelect==undefined || typeSelect.selectedIndex==undefined  
			|| stateSelect==undefined || stateSelect.selectedIndex==undefined 
			|| startTimeInput==undefined || endTimeInput==undefined 
			|| timeInput==undefined 
			|| kindSelect==undefined || kindSelect.selectedIndex==undefined){
		return 'Общая ошибка приложения <br>';
	}
	
	if(nameInput.value==undefined || nameInput.value==''){
		errorMessage+=markComponentValidityWithMessage(nameInput,false,'Пустое поле имя <br>');
	}else{
		var nameComponents=document.getElementById('intervals_form').getElementsByClassName('intervals');
		
		if(nameComponents!=undefined){
			for(var i=0;i<nameComponents.length;i++){
				if(nameComponents[i]!=undefined && nameComponents[i].name=='name'
					&& nameComponents[i].value!=undefined && nameComponents[i].id!=nameInput.id
					&& nameComponents[i].id!='intervals_name_'+idInput.value
					&& nameInput.value==nameComponents[i].value){
					
					errorMessage+=markComponentValidityWithMessage(nameInput,false,'Название не уникально <br>');
				}
			}
		}
	}
	var typeInt=typeSelect.selectedIndex;
	if(typeInt<0){
		errorMessage+=markComponentValidityWithMessage(typeSelect,false,'Пустое поле тип <br>');
	}
	if(stateSelect.selectedIndex<0){
		errorMessage+=markComponentValidityWithMessage(stateSelect,false,'Пустое поле состояние <br>');
	}
	var startTime=getComponentValue(startTimeInput);
	var endTime=getComponentValue(endTimeInput);
	if(startTime==undefined){
		errorMessage+=markComponentValidityWithMessage(startTimeInput,false,'Пустое поле Старт <br>');
	}
	if(endTime==undefined){
		errorMessage+=markComponentValidityWithMessage(endTimeInput,false,'Пустое поле Старт <br>');
	}
	if(startTime>=endTime){
		errorMessage+=markComponentValidityWithMessage(startTimeInput,false,'Время начала больше или равно времени завершения <br>');
		errorMessage+=markComponentValidityWithMessage(endTimeInput,false,'Время Завершения меньше времени начала <br>');
	}
	if(typeInt==intSettings.multidailyIndex){
		var days=checkBoxListToString('days');
		
		if(days.length!=13 || !days.includes('1')){
			errorMessage+=markComponentValidityWithMessage(daysDiv,false,'Не указан ни один день <br>');
		}
	}
	if(typeInt==intSettings.periodicIndex){
		var time=getComponentValue(timeInput);
		
		if(time==undefined || time=='' || time==0 || time=='0' || time<60){
			errorMessage+=markComponentValidityWithMessage(timeInput,false,'Минимально возможное значение периода 60 сек <br>');
		}
	}
	if((typeInt==intSettings.rescheduleIndex)&&(errorMessage=='')){
		alert('Поточный будильник будет перепланирован. Время старта и окончания будет изменено автоматически.');
	}
	return errorMessage;
}
function constructIntervalsFormDataAsJson(form){
	var formInputs = getComponentChildrenByClass(form,'intervals');
	
	var json='{';
	var pageName='';
	
	for(var i=0;i<formInputs.length;i++){
		var child=formInputs[i];
		if(child!=undefined && child.tagName!=undefined && child.type!=undefined
				&& child.name!=undefined && child.id!=undefined){

				if(child.name.substring(0,8)!='cb_days_'){
				
					inputName=child.name;
					inputValue=getComponentValue(child);
				
					json=json+'"'+inputName+'": "'+inputValue+'",';
				}
		}
	}
	
	var days='';
	for(var i=0;i<7;i++){
		days+=getComponentValue(document.getElementById('intervals_days_'+i))+',';
	}
	
	if(days.substring(days.length - 1)==','){
		days=days.substring(0,days.length-1);
	}
	
	json=json+'"days": "'+days+'"';
	
	if(json.substring(json.length - 1)==','){
		json=json.substring(0,json.length-1);
	}
	
	json+='}';
	
	var target=document.getElementById('intervals_target').value;
	var page='intervals';
	
	return constructFormData_JSONprocessor(target,page,json);
}
function onIntervalSavedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		hideComponent('intervals_cur_modal');
		openIntervalsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
		customIntervalsMessage='Интервал был успешно сохранен';
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}
/*----------delete intervals---------------*/
function showDeleteIntervalForm(id){
	var message='Подтвердите удаление интервала '+document.getElementById('intervals_name_'+id).value;
	
	var btnNames=['Удалить','Отменить'];
	var btnToolTips=['',''];
	var btnClasses=['w3-border-red','w3-border-red'];
	var btnOnClicks=['confirmDeleteInterval('+id+');','confirmDlgHide();'];
	
	confirmDlgInit('Подтверждение',message,'',undefined,undefined,btnNames,btnToolTips,btnClasses,btnOnClicks,false);
	confirmDlgShow();
}
function confirmDeleteInterval(id){
	showStatusMessage('Удаляю интервал ...','w3-yellow');
	
    var target = createHiddenInput(undefined,'remote_target','timeIntervalService');
    var page = createHiddenInput(undefined,'remote_page','delete');
    var val = createHiddenInput(undefined,'val_json',id);

    var deleteUrl='/processJson';
    
    var form = document.createElement("form");
    form.appendChild(target);
    form.appendChild(page);
    form.appendChild(val);
	
	postForm(form,deleteUrl,undefined,undefined,validateStatusMessageDefault,onIntervalDeletedHandler,getStatusMessageComp());
}
function onIntervalDeletedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		confirmDlgHide();
		openIntervalsTab();
		showStatusMessage('Удалено. Обновляю данные','w3-yellow');
		customIntervalsMessage='Интервал был успешно удален';
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}