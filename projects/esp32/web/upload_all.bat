REM SET ip=192.168.0.107

SET FOLDER=css
SET PREFFIX=web
SET DELIM=/

@ECHO OFF

echo '------------------------------------'
echo upload htm.gz
echo '------------------------------------'

cd tempDeploy

for %%f in (*.gz) do (
	echo Upload %PREFFIX%%DELIM%%%f
	curl -i -X POST -H "Expect:" -F "uploadfile=@%%f;filename=%PREFFIX%%DELIM%%%f;" -F submit=upload http://%ip%/edit
)

powershell -Command "Remove-Item %TEMP_FOLDER% -Force -Recurse -ErrorAction Ignore"

pause

