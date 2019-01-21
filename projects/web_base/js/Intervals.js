/*-----------------------Sensors values processing--------------------------------*/
var innactiveIndex=-1;
var multidailyIndex=-1;
var periodicIndex=-1;
var intervalType=[];
var intervalState=[];
var dayOfWeekShort=[];
var dayOfWeek=[];
var intervalKind=[];

function processIntervalsJsonGet(data){
	fillParameters(data);
	processItemsJsonGet(data);
}

function fillParameters(data){
	fillParameterArray(intervalType,data.intervalType);
	fillParameterArray(intervalState,data.intervalState);
	fillParameterArray(dayOfWeekShort,data.dayOfWeekShort);
	fillParameterArray(dayOfWeek,data.dayOfWeek);
	fillParameterArray(intervalKind,data.intervalKind);
	
	multidailyIndex=data.multidailyIndex;
	periodicIndex=data.periodicIndex;
	innactiveIndex=data.innactiveIndex;
}

function fillParameterArray(targetArray,jsonArray){
	if(targetArray.length==0){
		for(var i=0;i<jsonArray.length;i++){targetArray[i]=jsonArray[i];}
	}
}

function openNewIntervalForm(){
	var dt=new Date();
	var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
	
	var interval={};
		interval.id=-1;
		interval.name='Новый';
		interval.type=0;
		interval.state=0;
		interval.startTime=tmp+60;
		interval.endTime=tmp+120;
		interval.time='0';
		interval.days='0,0,0,0,0,0,0';
		interval.kind=0;
		
		openItemPopup('','','Новый интервал',interval);
}

function openConfirmDeleteIntervalForm(tag,id,name){
	var message='Подтвердите удаление интервала '+name;
	
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

    var form = document.createElement("form");
    form.appendChild(target);
    form.appendChild(page);
    form.appendChild(val);
	
	postForm(form,submitValuesUrl,undefined,undefined,validateStatusMessageDefault,onIntervalDeletedHandler,getStatusMessageComp());
}

function onIntervalDeletedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		confirmDlgHide();
		reloadCurrentSettingsTab();
		showStatusMessage('Удалено. Обновляю данные','w3-yellow');
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}

function preprocessIntervalDisplay(items){
	var count=items.itemCount;
	var countDiv=document.getElementById('intervalsCountDiv');
	if(count==0){
		countDiv.innerHTML='Не задано ни одного интервала';
	}else{
		countDiv.innerHTML='Всего интервалов '+count;
	}
	
}

function putIntervalContentToContainer(container,interval,noId,editable){
	var id=interval.id;
	var name=interval.name;
	var typeInt=interval.type;
	var stateInt=interval.state;
	var startTime=interval.startTime;
	var endTime=interval.endTime;
	var time=interval.time;
	var days=interval.days.split(',');
	var kindInt=interval.kind;
	
	var hr = document.createElement('hr');
	container.appendChild(hr);
	container.appendChild(hr);
	var divh=document.createElement('div');
	divh.setAttribute('class','w3-row w3-teal');
	divh.setAttribute('style','margin-top: 15px; height: 50px;');
	
	var div1h=createDivComponent('','w3-quarter','','Интервал');
	var div2h=createDivComponent('','w3-quarter','','Тип/Состояние.');
	var div3h=createDivComponent('','w3-quarter','','Старт/Стоп');
	var div4h=createDivComponent('','w3-quarter','','Параметры');
	
	divh.appendChild(div1h);
	divh.appendChild(div2h);
	divh.appendChild(div3h);
	divh.appendChild(div4h);
	
	container.appendChild(divh);
	var divhv=document.createElement('div');
	divhv.setAttribute('class','w3-row');
	divhv.setAttribute('style','margin-top: 1px; height: 130px;');
	
	var col1h=createDivComponent('','w3-quarter');
	var col2h=createDivComponent('','w3-quarter');
	var col3h=createDivComponent('','w3-quarter');
	var col4h=createDivComponent('','w3-quarter');
	/* column 1*/
	
	var suf='';
	
	if(editable==undefined || !editable){
		suf='_'+id;
	}
	
	var cid='id'+suf;
	var cname='name'+suf;
	var ctypeInt='typeInt'+suf;
	var cstateInt='stateInt'+suf;
	var cstartTime='startTime'+suf;
	var cendTime='endTime'+suf;
	var ctime='time'+suf;
	var cdays='days'+suf;
	var ckindInt='kind'+suf;
	
	if(editable==undefined || !editable){
		var cellEditButton=createDivComponent('','w3-cell');
		var cellDeleteButton=createDivComponent('','w3-cell');
		var editButton=createItemEditButton('id',id,name,undefined);
		var deleteButton=createItemDeleteButton('id',id,name,'openConfirmDeleteIntervalForm');
		
		cellEditButton.appendChild(editButton);
		cellDeleteButton.appendChild(deleteButton);
		
		col1h.appendChild(cellEditButton);
		col1h.appendChild(cellDeleteButton);
	}
	
	var inputId=createHiddenInput(cid,'id',id,'intervals');
	var cellDivId=createDivComponent('','w3-cell w3-container');
	cellDivId.appendChild(inputId);
	setVisible(cellDivId,false);

	var lblName=createLabelSimple('lbl'+cname,cname,undefined,undefined,'Название');
	var inputName=createInputSimple(cname,'name','intervals','width: 80%;',name,editable);
	inputName.setAttribute('type','text');
	inputName.setAttribute('required','1');
	
	var cellDivName=createDivComponent();
	cellDivName.appendChild(lblName);
	cellDivName.appendChild(inputName);
		
	col1h.appendChild(cellDivId);
	col1h.appendChild(cellDivName);
	/* column 2*/
	
	var lblType=createLabelSimple('lbl'+ctypeInt,ctypeInt,undefined,undefined,'Периодичность');
	var typeSelect=document.createElement("select");
	var cellDivType=createDivComponent();
	
	typeSelect.id=ctypeInt;
	typeSelect.name='type';
	typeSelect.setAttribute('onChange',"handlePeriodTypeChange(this.selectedIndex);");
	typeSelect.setAttribute('class','w3-select intervals');
	typeSelect.setAttribute('style','width: 80%;');
	
	if(editable==undefined || !editable){typeSelect.setAttribute("disabled","disabled");}
	putItemsToComboCox(typeSelect,intervalType,typeInt);
	
	cellDivType.appendChild(lblType);
	cellDivType.appendChild(typeSelect);
	/*--*/
	var lblState=createLabelSimple('lbl'+cstateInt,cstateInt,undefined,undefined,'Состояние');
	var stateSelect=document.createElement("select");
	var cellDivState=createDivComponent();
	
	stateSelect.id=cstateInt;
	stateSelect.name='state';
	stateSelect.setAttribute('class','w3-select intervals');
	stateSelect.setAttribute('style','width: 80%;');
	
	if(editable==undefined || !editable){stateSelect.setAttribute("disabled","disabled");}
	putItemsToComboCox(stateSelect,intervalState,stateInt);
	
	cellDivState.appendChild(lblState);
	cellDivState.appendChild(stateSelect);
	
	col2h.appendChild(cellDivType);
	col2h.appendChild(cellDivState);
	
	/* column 3*/
	var lblStartTime=createLabelSimple('lbl'+cstartTime,cstartTime,undefined,undefined,'Старт');
	var inputStartTime=createInputSimple(cstartTime,'startTime','intervals','width: 90%;','',editable);
	var cellDivStartTime=createDivComponent();
	
	inputStartTime.setAttribute('type','datetime-local');
	inputStartTime.setAttribute('required','1');
	setComponentValue(inputStartTime,startTime);
	
	
	cellDivStartTime.appendChild(lblStartTime);
	cellDivStartTime.appendChild(inputStartTime);
	/*--*/
	var lblEndTime=createLabelSimple('lbl'+cendTime,cendTime,undefined,undefined,'Стоп ');
	var inputEndTime=createInputSimple(cendTime,'endTime','intervals','width: 90%;','',editable);
	var cellDivendTime=createDivComponent();
	
	inputEndTime.setAttribute('type','datetime-local');
	inputStartTime.setAttribute('required','1');
	setComponentValue(inputEndTime,endTime);
	
	cellDivendTime.appendChild(lblEndTime);
	cellDivendTime.appendChild(inputEndTime);
	
	col3h.appendChild(cellDivStartTime);
	col3h.appendChild(cellDivendTime);
	
	/* column 4*/
		
		var lblDays=createLabelSimple('lbl'+cdays,undefined,undefined,undefined,'Дни выполнения');
		var daysDiv=createDivComponent();
		var daysDivChkBox=createDivComponent();
		if(editable==undefined || !editable){
			daysDivChkBox.id=cdays;
		}
		else{
			daysDivChkBox.id='days';
		}
		
		daysDiv.id=cdays+'Div';
		daysDiv.name=cdays+'Div';
		
		arrayToCheckBoxList(daysDivChkBox,cdays,days,dayOfWeekShort,editable);
		daysDiv.appendChild(lblDays);
		daysDiv.appendChild(daysDivChkBox);
		/*--*/
		var lblTime=createLabelSimple('lbl'+ctime,ctime,undefined,undefined,'Период (сек) ');
		var inputTime=createInputSimple(ctime,'time','intervals','width: 50%;',time,editable);
		var timeDiv=createDivComponent();
		timeDiv.id=ctime+'Div';
		
		inputTime.setAttribute('type','number');
		
		timeDiv.appendChild(lblTime);
		timeDiv.appendChild(inputTime);
		/*--*/
		var lblKind=createLabelSimple('lbl'+ckindInt,ckindInt,undefined,undefined,'Тип');
		var kindSelect=document.createElement("select");
		var kindDiv=createDivComponent();
		
		kindSelect.id=ckindInt;
		kindSelect.name='kind';
		kindSelect.setAttribute('class','w3-select intervals');
		kindSelect.setAttribute('style','width: 80%;');
		
		if(editable==undefined || !editable){kindSelect.setAttribute("disabled","disabled");}
		putItemsToComboCox(kindSelect,intervalKind,kindInt);
		
		kindDiv.appendChild(lblKind);
		kindDiv.appendChild(kindSelect);
		/*--*/
		applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,typeInt);
		
	col4h.appendChild(daysDiv);
	col4h.appendChild(timeDiv);
	col4h.appendChild(kindDiv);
					
	divhv.appendChild(col1h);
	divhv.appendChild(col2h);
	divhv.appendChild(col3h);
	divhv.appendChild(col4h);
	
	container.appendChild(divhv);
	
	var divb=document.createElement('div');
	divb.setAttribute('class','w3-row w3-black');
	divb.setAttribute('style','margin-top: 1px; margin-bottom: 5px; height: 5px;');
			
	container.appendChild(divb);
	container.appendChild(hr);
}

function handlePeriodTypeChange(selectedIndex){
	var daysDiv=getComponentById('daysDiv');
	var timeDiv=getComponentById('timeDiv');
	var kindDiv=getComponentById('kindDiv');
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,selectedIndex)
}

/*show or hide days interval component based on type*/
function applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,typeInt){
	var isMultidaily=(typeInt==multidailyIndex);
	var isPeriodic=(typeInt==periodicIndex);
	var isKindVis=true;
	
	setVisible(daysDiv,isMultidaily);
	setVisible(timeDiv,isPeriodic);
	setVisible(kindDiv,isKindVis);
	
	markFormAsValid(currentForm,getStatusMessageComp(),true,true);
}

function submitIntervalsFormAsJsonReloadCurrTab(){
	showStatusMessage('Сохраняю интервалы ...','w3-yellow');
	postForm(currentForm,submitValuesUrl,validateValuesHandler,constructIntervalsFormDataAsJson,validateStatusMessageDefault,onIntervalSavedHandler,getStatusMessageComp());
}

function onIntervalSavedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		hideComponent(currentItemPreffix+'_modal');
		reloadCurrentSettingsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}

function constructIntervalsFormDataAsJson(form){
	
	var formInputs = getComponentChildrenByClass(form,'intervals');
	
	var json='{';
	var pageName='';
	
	var days={};
	
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
	
	json=json+'"days": "'+checkBoxListToString('days')+'"';
	
	if(json.substring(json.length - 1)==','){
		json=json.substring(0,json.length-1);
	}
	
	json+='}';
	
	var targetId=form.getAttribute('id')+'_target';
	var target=getComponentValueById(targetId);
	
	return constructFormData_JSONprocessor(target,currentTab,json);
}

function validateCurrentIntervalForm(){
	markFormAsValid(currentForm,getStatusMessageComp(),true,true);
	markComponentValidityById('days',true);
	
	var errorMessage='';
	
	var idInput=getComponentById('id');
	var nameInput=getComponentById('name');
	var typeSelect=getComponentById('typeInt');
	var stateSelect=getComponentById('stateInt');
	var startTimeInput=getComponentById('startTime');
	var endTimeInput=getComponentById('endTime');
	
	var daysDiv=getComponentById('days');
	var timeInput=getComponentById('time');
	var kindSelect=getComponentById('kind');
	
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
					&& nameComponents[i].id!='name_'+idInput.value
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
	
	if(typeInt==multidailyIndex){
		var days=checkBoxListToString('days');
		
		if(days.length!=13 || !days.includes('1')){
			errorMessage+=markComponentValidityWithMessage(daysDiv,false,'Не указан ни один день <br>');
		}
	}
	
	if(typeInt==periodicIndex){
		var time=getComponentValue(timeInput);
		
		if(time==undefined || time=='' || time==0 || time=='0' || time<60){
			errorMessage+=markComponentValidityWithMessage(timeInput,false,'Минимально возможное значение периода 60 сек <br>');
		}
	}
	
	return errorMessage;
}

function closeIntervalsPopup(){
	
}
