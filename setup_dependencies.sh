#!/bin/bash

echo "============================================"
echo "FOV Overlay - Dependency Setup"
echo "============================================"
echo ""

cd external

# Download Dear ImGui
echo "Downloading Dear ImGui..."
if [ ! -d "imgui" ]; then
    git clone https://github.com/ocornut/imgui.git
    if [ $? -ne 0 ]; then
        echo "ERROR: Failed to download Dear ImGui"
        exit 1
    fi
    echo "Dear ImGui downloaded successfully"
else
    echo "Dear ImGui already exists"
fi

# Download nlohmann/json
echo ""
echo "Downloading nlohmann/json..."
if [ ! -d "json/include/nlohmann" ]; then
    mkdir -p json/include/nlohmann
    cd json/include/nlohmann
    
    wget -O json.hpp https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
    
    if [ $? -ne 0 ]; then
        echo "ERROR: Failed to download nlohmann/json"
        echo "Please download manually from: https://github.com/nlohmann/json/releases"
        cd ../../..
        exit 1
    fi
    
    cd ../../..
    echo "nlohmann/json downloaded successfully"
else
    echo "nlohmann/json already exists"
fi

cd ..

echo ""
echo "============================================"
echo "Dependencies setup complete!"
echo "============================================"
echo ""
echo "Next steps:"
echo "1. Download YOLOv8 model and place in models/ directory"
echo "2. Run ./build.sh to build the project"
echo ""
