SET ip=192.168.0.107

curl -i -X DELETE -H "Expect:" -F "filename=/web/settings.htm" http://%ip%/edit

pause
