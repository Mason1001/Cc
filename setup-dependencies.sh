#!/bin/bash
# Gaming Overlay - Dependency Setup Script
# This script downloads all required dependencies

echo "Gaming Overlay - Dependency Setup"
echo "================================="
echo ""

set -e

# Create libs directory
echo "Creating libs directory..."
mkdir -p libs

# Download Dear ImGui
echo ""
echo "Downloading Dear ImGui..."
cd libs

if [ -d "imgui" ]; then
    echo "Dear ImGui already exists. Skipping..."
else
    git clone https://github.com/ocornut/imgui.git
    if [ $? -eq 0 ]; then
        echo "Dear ImGui downloaded successfully!"
    else
        echo "Failed to download Dear ImGui!"
        cd ..
        exit 1
    fi
fi

cd ..

# Optional: Download ONNX Runtime
echo ""
echo "Optional Dependencies:"
echo ""
read -p "Download ONNX Runtime for detection features? (y/n): " download_onnx

if [ "$download_onnx" = "y" ] || [ "$download_onnx" = "Y" ]; then
    echo ""
    echo "Downloading ONNX Runtime..."
    
    ONNX_VERSION="1.16.3"
    ONNX_URL="https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_VERSION}/onnxruntime-linux-x64-${ONNX_VERSION}.tgz"
    ONNX_FILE="libs/onnxruntime.tgz"
    
    wget -O "$ONNX_FILE" "$ONNX_URL" 2>/dev/null || curl -L -o "$ONNX_FILE" "$ONNX_URL"
    
    if [ -f "$ONNX_FILE" ]; then
        echo "Extracting ONNX Runtime..."
        tar -xzf "$ONNX_FILE" -C libs/
        rm "$ONNX_FILE"
        mv libs/onnxruntime-* libs/onnxruntime 2>/dev/null || true
        echo "ONNX Runtime downloaded and extracted!"
    else
        echo "Failed to download ONNX Runtime. You can download it manually later."
    fi
else
    echo "Skipping ONNX Runtime. Detection features will run in demo mode."
fi

# YOLOv8 Model
echo ""
read -p "Download YOLOv8 model? (requires Python/pip) (y/n): " download_model

if [ "$download_model" = "y" ] || [ "$download_model" = "Y" ]; then
    echo ""
    echo "Downloading YOLOv8 model..."
    echo "This requires Python and ultralytics package."
    echo ""
    
    # Check if Python is available
    if command -v python3 &> /dev/null; then
        echo "Installing ultralytics..."
        python3 -m pip install ultralytics --quiet
        
        echo "Exporting YOLOv8 model to ONNX..."
        python3 -c "from ultralytics import YOLO; model = YOLO('yolov8n.pt'); model.export(format='onnx')"
        
        # Move to assets/models
        if [ -f "yolov8n.onnx" ]; then
            mv yolov8n.onnx assets/models/yolov8n.onnx
            echo "YOLOv8 model downloaded and placed in assets/models!"
        fi
    else
        echo "Python not found. Please install Python and try again, or download the model manually."
        echo "Visit: https://github.com/ultralytics/ultralytics"
    fi
else
    echo "Skipping YOLOv8 model. Detection will run in demo mode."
fi

# Summary
echo ""
echo "================================="
echo "Dependency Setup Complete!"
echo ""
echo "Next steps:"
echo "1. Run ./build.sh to build the project"
echo "2. Check docs/SETUP.md for detailed instructions"
echo "3. Run the executable from build/bin/"
echo ""
