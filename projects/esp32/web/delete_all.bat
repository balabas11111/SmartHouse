REM SET ip=192.168.0.107

echo '---------------Delete files---------------------'
rem #cssFiles
curl -i -X DELETE -H "Expect:" -F "filename=/web/css/w3.css" http://%ip%/edit

curl -i -X DELETE -H "Expect:" -F "filename=/web/edit.htm.gz" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/settings.htm.gz" http://%ip%/edit
curl -i -X DELETE -H "Expect:" -F "filename=/web/index.htm.gz" http://%ip%/edit
