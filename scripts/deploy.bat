@echo off
setlocal

:: Set variables
set APP_NAME=BitcoinSatoshiCalculator
set BUILD_DIR=build\bin\Debug
set DEPLOY_DIR=deploy
set QT_DIR=D:\Qt\6.8.1\msvc2022_64
set VS_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.39.33519\bin\Hostx64\x64

:: Create deployment directory
if not exist %DEPLOY_DIR% mkdir %DEPLOY_DIR%
if not exist %DEPLOY_DIR%\platforms mkdir %DEPLOY_DIR%\platforms
if not exist %DEPLOY_DIR%\tls mkdir %DEPLOY_DIR%\tls

:: Copy the executable
copy "%BUILD_DIR%\bitcoinsatoshicalculator.exe" "%DEPLOY_DIR%\%APP_NAME%.exe"

:: Copy required Qt DLLs
copy "%QT_DIR%\bin\Qt6Core.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Gui.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Network.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Widgets.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6OpenGL.dll" "%DEPLOY_DIR%\"

:: Copy SSL/TLS support from Visual Studio
copy "%VS_DIR%\libssl.dll" "%DEPLOY_DIR%\"
copy "%VS_DIR%\libcrypto.dll" "%DEPLOY_DIR%\"

:: Copy platform plugins
copy "%QT_DIR%\plugins\platforms\qwindows.dll" "%DEPLOY_DIR%\platforms\"

:: Copy TLS backend plugins
copy "%QT_DIR%\plugins\tls\qschannelbackend.dll" "%DEPLOY_DIR%\tls\"
copy "%QT_DIR%\plugins\tls\qopensslbackend.dll" "%DEPLOY_DIR%\tls\"

:: Create README
echo Bitcoin Satoshi Calculator > "%DEPLOY_DIR%\README.txt"
echo. >> "%DEPLOY_DIR%\README.txt"
echo Installation Instructions: >> "%DEPLOY_DIR%\README.txt"
echo 1. Extract all files to a directory of your choice >> "%DEPLOY_DIR%\README.txt"
echo 2. Run %APP_NAME%.exe >> "%DEPLOY_DIR%\README.txt"
echo. >> "%DEPLOY_DIR%\README.txt"
echo Requirements: >> "%DEPLOY_DIR%\README.txt"
echo - Windows 10 or later >> "%DEPLOY_DIR%\README.txt"
echo - Internet connection for real-time exchange rates >> "%DEPLOY_DIR%\README.txt"

echo Creating Windows deployment...
echo Deployment complete! Check the deploy folder for platform-specific zip files. 