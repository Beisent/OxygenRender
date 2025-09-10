@echo off
setlocal

REM ============ Part 1: Download backpack (if not exists) ============
set TARGET_DIR=resources\objects\backpack
set ZIP_URL=https://learnopengl-cn.github.io/data/backpack.zip
set ZIP_FILE=backpack.zip

if exist "%TARGET_DIR%" (
    echo Directory "%TARGET_DIR%" already exists. Skipping backpack download.
) else (
    echo Directory "%TARGET_DIR%" does not exist. Creating and downloading backpack...
    mkdir "%TARGET_DIR%" 2>nul
    powershell -Command "Invoke-WebRequest -Uri '%ZIP_URL%' -OutFile '%ZIP_FILE%'"
    powershell -Command "Expand-Archive -Path '%ZIP_FILE%' -DestinationPath '%TARGET_DIR%'"
    del "%ZIP_FILE%"
    echo Backpack download and extraction completed.
)

REM ============ Part 2: Download images to resources\ ============
set IMAGE_DIR=resources
mkdir "%IMAGE_DIR%" 2>nul

REM Download wall.jpg as container.jpg
set WALL_URL=https://learnopengl-cn.github.io/img/01/06/wall.jpg
set CONTAINER_FILE=%IMAGE_DIR%\container.jpg

if exist "%CONTAINER_FILE%" (
    echo "%CONTAINER_FILE%" already exists. Skipping download.
) else (
    echo Downloading container.jpg...
    powershell -Command "Invoke-WebRequest -Uri '%WALL_URL%' -OutFile '%CONTAINER_FILE%'"
)

REM Download awesomeface.png
set AWESOME_URL=https://learnopengl-cn.github.io/img/01/06/awesomeface.png
set AWESOME_FILE=%IMAGE_DIR%\awesomeface.png

if exist "%AWESOME_FILE%" (
    echo "%AWESOME_FILE%" already exists. Skipping download.
) else (
    echo Downloading awesomeface.png...
    powershell -Command "Invoke-WebRequest -Uri '%AWESOME_URL%' -OutFile '%AWESOME_FILE%'"
)

echo All downloads completed.
endlocal
pause