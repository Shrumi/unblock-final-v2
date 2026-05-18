@echo off
setlocal
set "TARGET_DIR=C:\unblock"
set "EXE_PATH=%TARGET_DIR%\bin\engine.exe"
set "SHORTCUT_NAME=Unblock Engine.lnk"
set "DESKTOP_PATH=%USERPROFILE%\Desktop"

echo Creating shortcut on Desktop...
powershell -Command "$s=(New-Object -COM WScript.Shell).CreateShortcut('%DESKTOP_PATH%\%SHORTCUT_NAME%');$s.TargetPath='%EXE_PATH%';$s.WorkingDirectory='%TARGET_DIR%\bin';$s.IconLocation='%EXE_PATH%,0';$s.Save();$s=(New-Object -COM WScript.Shell).CreateShortcut('%DESKTOP_PATH%\%SHORTCUT_NAME%');$s.TargetPath='%EXE_PATH%';$s.WorkingDirectory='%TARGET_DIR%\bin';$s.Save()"

echo.
echo Установка завершена!
echo Ярлык создан на рабочем столе.
echo Пожалуйста, запускайте программу от имени администратора.
pause
