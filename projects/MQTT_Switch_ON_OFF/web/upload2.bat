SET IP=192.168.0.102
SET FOLDER=css
SET PREFFIX=web
SET DELIM=/

@ECHO OFF

set sources[0]=css
set sources[1]=font
set sources[2]=img
set sources[3]=js
set sources[4]=fileManager


for /F "tokens=2 delims==" %%p in ('set sources[') do (
	echo '------------------------------------'
	echo process %%p
	echo '------------------------------------'
	for /f "delims=" %%f in ('dir %%p /b /a-d-h-s') do (
	   curl -i -X POST -H "Expect:" -F "uploadfile=@%%p%DELIM%%%f;filename=%PREFFIX%%DELIM%%%p%DELIM%%%f;" -F submit=upload http://%ip%/edit
	)
)

echo '------------------------------------'
echo process html
echo '------------------------------------'

for %%f in (*.htm) do (
    curl -i -X POST -H "Expect:" -F "uploadfile=@%%f;filename=%PREFFIX%%DELIM%%%f;" -F submit=upload http://%ip%/edit
)

pause

