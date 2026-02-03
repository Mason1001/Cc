@echo off
echo ============================================
echo FOV Overlay - Dependency Setup
echo ============================================
echo.

cd external

REM Download Dear ImGui
echo Downloading Dear ImGui...
if not exist imgui (
    git clone https://github.com/ocornut/imgui.git
    if errorlevel 1 (
        echo ERROR: Failed to download Dear ImGui
        pause
        exit /b 1
    )
    echo Dear ImGui downloaded successfully
) else (
    echo Dear ImGui already exists
)

REM Download nlohmann/json
echo.
echo Downloading nlohmann/json...
if not exist json\include\nlohmann (
    mkdir json\include\nlohmann
    cd json\include\nlohmann
    
    REM Download using PowerShell
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp' -OutFile 'json.hpp'"
    
    if errorlevel 1 (
        echo ERROR: Failed to download nlohmann/json
        echo Please download manually from: https://github.com/nlohmann/json/releases
        cd ..\..\..
        pause
        exit /b 1
    )
    
    cd ..\..\..
    echo nlohmann/json downloaded successfully
) else (
    echo nlohmann/json already exists
)

cd ..

echo.
echo ============================================
echo Dependencies setup complete!
echo ============================================
echo.
echo Next steps:
echo 1. Download YOLOv8 model and place in models/ directory
echo 2. Run build.bat to build the project
echo.
pause
