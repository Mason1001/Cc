@echo off
echo ============================================
echo FOV Overlay Application - Build Script
echo ============================================
echo.

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

echo.
echo Generating Visual Studio solution...
cmake .. -G "Visual Studio 17 2022" -A x64

if errorlevel 1 (
    echo.
    echo ERROR: CMake generation failed
    echo.
    echo Trying with Visual Studio 16 2019...
    cmake .. -G "Visual Studio 16 2019" -A x64
    
    if errorlevel 1 (
        echo.
        echo ERROR: Could not generate project files
        echo Please check your Visual Studio installation
        pause
        exit /b 1
    )
)

echo.
echo Building project in Release mode...
cmake --build . --config Release

if errorlevel 1 (
    echo.
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo ============================================
echo Build completed successfully!
echo ============================================
echo.
echo Executable location: build\bin\Release\FOVOverlay.exe
echo.
echo To run the application:
echo   cd build\bin\Release
echo   FOVOverlay.exe
echo.
pause
