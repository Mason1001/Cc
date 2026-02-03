# Gaming Overlay - Dependency Setup Script
# This script downloads all required dependencies

Write-Host "Gaming Overlay - Dependency Setup" -ForegroundColor Cyan
Write-Host "=================================" -ForegroundColor Cyan
Write-Host ""

$ErrorActionPreference = "Stop"

# Create libs directory
Write-Host "Creating libs directory..." -ForegroundColor Yellow
if (!(Test-Path "libs")) {
    New-Item -ItemType Directory -Path "libs"
}

# Download Dear ImGui
Write-Host ""
Write-Host "Downloading Dear ImGui..." -ForegroundColor Yellow
Set-Location "libs"

if (Test-Path "imgui") {
    Write-Host "Dear ImGui already exists. Skipping..." -ForegroundColor Gray
} else {
    git clone https://github.com/ocornut/imgui.git
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Dear ImGui downloaded successfully!" -ForegroundColor Green
    } else {
        Write-Host "Failed to download Dear ImGui!" -ForegroundColor Red
        Set-Location ..
        exit 1
    }
}

Set-Location ..

# Optional: Download ONNX Runtime
Write-Host ""
Write-Host "Optional Dependencies:" -ForegroundColor Cyan
Write-Host ""
$downloadOnnx = Read-Host "Download ONNX Runtime for detection features? (y/n)"

if ($downloadOnnx -eq "y" -or $downloadOnnx -eq "Y") {
    Write-Host ""
    Write-Host "Downloading ONNX Runtime..." -ForegroundColor Yellow
    
    $onnxVersion = "1.16.3"
    $onnxUrl = "https://github.com/microsoft/onnxruntime/releases/download/v$onnxVersion/onnxruntime-win-x64-$onnxVersion.zip"
    $onnxZip = "libs/onnxruntime.zip"
    
    try {
        Invoke-WebRequest -Uri $onnxUrl -OutFile $onnxZip
        Write-Host "Extracting ONNX Runtime..." -ForegroundColor Yellow
        Expand-Archive -Path $onnxZip -DestinationPath "libs/onnxruntime" -Force
        Remove-Item $onnxZip
        Write-Host "ONNX Runtime downloaded and extracted!" -ForegroundColor Green
    } catch {
        Write-Host "Failed to download ONNX Runtime. You can download it manually later." -ForegroundColor Yellow
    }
} else {
    Write-Host "Skipping ONNX Runtime. Detection features will run in demo mode." -ForegroundColor Gray
}

# YOLOv8 Model
Write-Host ""
$downloadModel = Read-Host "Download YOLOv8 model? (requires Python/pip) (y/n)"

if ($downloadModel -eq "y" -or $downloadModel -eq "Y") {
    Write-Host ""
    Write-Host "Downloading YOLOv8 model..." -ForegroundColor Yellow
    Write-Host "This requires Python and ultralytics package." -ForegroundColor Gray
    Write-Host ""
    
    # Check if Python is available
    $pythonAvailable = Get-Command python -ErrorAction SilentlyContinue
    
    if ($pythonAvailable) {
        Write-Host "Installing ultralytics..." -ForegroundColor Yellow
        python -m pip install ultralytics --quiet
        
        Write-Host "Exporting YOLOv8 model to ONNX..." -ForegroundColor Yellow
        python -c "from ultralytics import YOLO; model = YOLO('yolov8n.pt'); model.export(format='onnx')"
        
        # Move to assets/models
        if (Test-Path "yolov8n.onnx") {
            Move-Item "yolov8n.onnx" "assets/models/yolov8n.onnx" -Force
            Write-Host "YOLOv8 model downloaded and placed in assets/models!" -ForegroundColor Green
        }
    } else {
        Write-Host "Python not found. Please install Python and try again, or download the model manually." -ForegroundColor Yellow
        Write-Host "Visit: https://github.com/ultralytics/ultralytics" -ForegroundColor Gray
    }
} else {
    Write-Host "Skipping YOLOv8 model. Detection will run in demo mode." -ForegroundColor Gray
}

# Summary
Write-Host ""
Write-Host "=================================" -ForegroundColor Cyan
Write-Host "Dependency Setup Complete!" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. Run build.ps1 to build the project" -ForegroundColor White
Write-Host "2. Check docs/SETUP.md for detailed instructions" -ForegroundColor White
Write-Host "3. Run the executable from build/bin/Release/" -ForegroundColor White
Write-Host ""
