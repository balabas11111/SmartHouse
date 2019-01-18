function openCommandConfirmPopup(message,target,action,displayCloseBtn){
	
	var inputNames=['remote_target','remote_page','val_json','action'];
	var inputValues=[target,'command','{"command": "'+action+'"}',action];
	
	var btnNames=['Подтвердить','Отменить'];
	var btnToolTips=['Подтвердить','Отменить'];
	var btnClasses=['w3-border-green','w3-border-red'];
	var btnOnClicks=['submitManageCommand();','confirmDlgHide();'];
	
	confirmDlgInit('Подтверждение',message,'',inputNames,inputValues,btnNames,btnToolTips,btnClasses,btnOnClicks,displayCloseBtn);
	confirmDlgShow();
}

function submitManageCommand(){
	var action=confirmDlgGetGetInputValue('action');
	
	var form=getComponentById('confirm_form');
	var msg=getComponentById('confirm_msg');
	
	showMessage(msg,'Отправляю команду '+command+' на устройство...','w3-yellow');
	
	postForm(form,'/submitForm_commands',undefined,constructFormDataDefault,undefined,getCurrentCommandHandler,msg);
}

function getCurrentCommandHandler(data){
	var command=data.status;
	
	var message=data.message;
	var messageColorClass='w3-yellow';
	
	if(status=='Ok'){
		messageColorClass='w3-green';
	}
	if(status=='Error' || status=='NotFound'){
		messageColorClass='w3-red';
	}
	
	var msg=getComponentById('confirm_msg');
	showMessage(msg,' Сообщение '+message,messageColorClass);
	
	var command=getComponentById('confirm_command').value;
	
	if(command=='restart'){
		confirmDlgSetHeader('Перезагрузка устройства');
		confirmDlgSetContent('Перезагрузка устройства');
		
		setTimeout(function(){tickRestart();}, 1000);
	}
	if(command=='recreateThingSpeak'){
		confirmDlgSetHeader('Сообщение системы');
		
		confirmDlgSetContent('Каналы и очереди ThingSpeak были пересозданы.<br> Перезапустите устройство для применения новых настроек.<br> Все предыдущие данные каналов находятся в старых каналах и могут быть просмотрены<br>'+
					'Создано : '+message);
		
		restartDialogToConfirm();
	}
	
	if(command=='deleteSettings'){
		confirmDlgSetHeader('Сообщение системы');
		
		confirmDlgSetContent('Все настройки системы были удалены<br> Перезапустите устройство для применения настроек по умолчанию. <br>'+message+'<br>');
		
		restartDialogToConfirm();
	}
}

function restartDialogToConfirm(){
	confirmDlgRemoveButtons();
	confirmDlgAddButton(0,'Закрыть и перезапустить','Закрыть и перезапустить','w3-border-red','displayRestartDialog();');
	
	confirmDlgCloseButtonVisible(false);
					
	hideComponent('confirm_msg');
}

function displayRestartDialog(){
	openConfirmPopup('Подтвердите рестарт устройства','restart');
}

var restartTimer=30;

function tickRestart(){
	restartTimer--;
	
	if(restartTimer<=0){
		location.reload();
	}
		
	var result='Устройство перезапустится через '+restartTimer+' сек';
	
	hideComponent('confirm_msg');
	getComponentById('confirm_content').innerHTML=result;
	
	setTimeout(function(){tickRestart();}, 1000);
}

function closePopup(modalName,messageComponentName){
	hideComponent(modalName);
	currentMessageComp=getComponentById(messageComponentName);
}