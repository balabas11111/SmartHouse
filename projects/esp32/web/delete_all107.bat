SET ip=192.168.0.107

rem #cssFiles
curl -i -X DELETE -H "Expect:" -F "filename=/web/css/w3.css" http://%ip%/edit

rem #images

rem #js

curl -i -X DELETE -H "Expect:" -F "filename=/web/fileManager/edit.htm.gz" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/fileManager/edit2.html" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/settings.htm" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/index.htm" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/settings.htm.gz" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/index.htm.gz" http://%ip%/edit


pause
