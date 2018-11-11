SET ip=192.168.0.100

rem #js
curl -i -X POST -H "Expect:" -F "uploadfile=@js/DeviceLib.js;filename=web/js/DeviceLib.js;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@js/DeviceLib.js;filename=web/js/index.js;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@js/index.js;filename=web/js/index.js;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@js/lamp.js;filename=web/js/lamp.js;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@js/menu.js;filename=web/js/menu.js;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@js/settings.js;filename=web/js/settings.js;" -F submit=upload http://%ip%/edit

rem #html
curl -i -X POST -H "Expect:" -F "uploadfile=@fileManager/edit.htm.gz;filename=web/edit.htm.gz;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@index.html;filename=web/index.htm;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@settings.html;filename=web/settings.htm;" -F submit=upload http://%ip%/edit
