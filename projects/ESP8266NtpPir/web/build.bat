SET IP=192.168.0.120
SET DELETE_HTML=False
SET UPLOAD_GZ=True

SET PROJECT_PATH=%cd%

call %PROJECT_PATH%\..\..\HtmlPackager\packager\target\build.bat
