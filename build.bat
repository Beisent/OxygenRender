@echo off
setlocal enabledelayedexpansion

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

REM ============ Part 3: Download skybox images ============
set SKYBOX_DIR=resources\skybox
mkdir "%SKYBOX_DIR%" 2>nul

set SKYBOX_UP=https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_up.png
set SKYBOX_DN=https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_dn.png
set SKYBOX_LF=https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_lf.png
set SKYBOX_RT=https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_rt.png
set SKYBOX_FT=https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_ft.png
set SKYBOX_BK=https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_bk.png

for %%i in (up dn lf rt ft bk) do (
    set "URL=!SKYBOX_%%i!"
    set "FILE=%SKYBOX_DIR%\arch3_%%i.png"
    if exist "!FILE!" (
        echo "!FILE!" already exists. Skipping download.
    ) else (
        echo Downloading "!FILE!"...
        powershell -Command "Invoke-WebRequest -Uri '!URL!' -OutFile '!FILE!'"
    )
)

echo All downloads completed.
endlocal
pause
