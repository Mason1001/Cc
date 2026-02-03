# Gaming Overlay Build Script for Windows

Write-Host "Building Gaming Overlay..." -ForegroundColor Cyan

# Create build directory
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}

Set-Location "build"

# Configure with CMake
Write-Host "Configuring with CMake..." -ForegroundColor Yellow
cmake .. -G "Visual Studio 17 2022" -A x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Build the project
Write-Host "Building project..." -ForegroundColor Yellow
cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "Executable location: build/bin/Release/GamingOverlay.exe" -ForegroundColor Cyan

Set-Location ..
