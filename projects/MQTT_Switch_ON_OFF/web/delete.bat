SET ip=192.168.0.100

rem #cssFiles
curl -i -X DELETE -H "Expect:" -F "filename=/web/css/w3.css" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/css/digitalClock.css" http://%ip%/edit

rem #fonts
curl -i -X DELETE -H "Expect:" -F "filename=/web/font/digitalClock.woff2" http://%ip%/edit

rem #images
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/Altitude.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/connected.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/connecting.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/emptyWalk.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/Humidity.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/LightLevel.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/OffLamp_64.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/OnLamp_64.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/Pressure.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/question.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/redhat.png" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/img/Temperature.png" http://%ip%/edit

rem #js
curl -i -X DELETE -H "Expect:" -F "filename=/web/js/DeviceLib.js" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/js/index.js" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/js/lamp.js" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/js/menu.js" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/js/settings.js" http://%ip%/edit

rem #html
curl -i -X DELETE -H "Expect:" -F "filename=/web/edit.htm.gz" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/index.htm" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/settings.htm" http://%ip%/edit
