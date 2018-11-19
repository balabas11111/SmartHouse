SET ip=192.168.0.102

rem #cssFiles
curl -i -X POST -H "Expect:" -F "uploadfile=@css/w3.css;filename=web/css/w3.css;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@css/digitalClock.css;filename=web/css/digitalClock.css;" -F submit=upload http://%ip%/edit

rem #fonts
curl -i -X POST -H "Expect:" -F "uploadfile=@font/digitalClock.woff2;filename=web/font/digitalClock.woff2;" -F submit=upload http://%ip%/edit

rem #images
curl -i -X POST -H "Expect:" -F "uploadfile=@img/Altitude.png;filename=web/img/Altitude.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/connected.png;filename=web/img/connected.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/connecting.png;filename=web/img/connecting.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/emptyWalk.png;filename=web/img/emptyWalk.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/Humidity.png;filename=web/img/Humidity.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/hv_on.png;filename=web/img/hv_on.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/hv_off.png;filename=web/img/hv_off.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/LightLevel.png;filename=web/img/LightLevel.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/OffLamp_64.png;filename=web/img/OffLamp_64.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/OnLamp_64.png;filename=web/img/OnLamp_64.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/Pressure.png;filename=web/img/Pressure.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/question.png;filename=web/img/question.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/redhat.png;filename=web/img/redhat.png;" -F submit=upload http://%ip%/edit
curl -i -X POST -H "Expect:" -F "uploadfile=@img/Temperature.png;filename=web/img/Temperature.png;" -F submit=upload http://%ip%/edit

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
