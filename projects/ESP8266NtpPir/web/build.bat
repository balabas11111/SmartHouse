SET IP=192.168.0.50
SET PROJECT_PATH=%cd%\
SET BASE_WEB_PATH=..\..\web_base\

call %BASE_WEB_PATH%pack_all.bat
call %BASE_WEB_PATH%upload_all.bat