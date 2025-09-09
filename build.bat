@echo off
setlocal

REM Set target directory
set TARGET_DIR=resources\objects\backpack
set ZIP_URL=https://learnopengl-cn.github.io/data/backpack.zip
set ZIP_FILE=backpack.zip

REM Check if directory exists
if exist "%TARGET_DIR%" (
    echo Directory "%TARGET_DIR%" already exists. No need to download.
) else (
    echo Directory "%TARGET_DIR%" does not exist. Creating directory and downloading files...
    REM Create directory
    mkdir "%TARGET_DIR%"

    REM Download zip file
    powershell -Command "Invoke-WebRequest -Uri '%ZIP_URL%' -OutFile '%ZIP_FILE%'"

    REM Extract zip file to target directory
    powershell -Command "Expand-Archive -Path '%ZIP_FILE%' -DestinationPath '%TARGET_DIR%'"

    REM Delete zip file
    del "%ZIP_FILE%"

    echo Download and extraction completed.
)

endlocal
pause
