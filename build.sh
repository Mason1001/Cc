#!/bin/bash

echo "============================================"
echo "FOV Overlay Application - Build Script"
echo "============================================"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake is not installed or not in PATH"
    echo "Please install CMake from https://cmake.org/download/"
    exit 1
fi

# Create build directory
mkdir -p build
cd build

echo ""
echo "Generating build files..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: CMake generation failed"
    exit 1
fi

echo ""
echo "Building project..."
cmake --build . --config Release

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed"
    exit 1
fi

echo ""
echo "============================================"
echo "Build completed successfully!"
echo "============================================"
echo ""
echo "Executable location: build/bin/FOVOverlay"
echo ""
echo "To run the application:"
echo "  cd build/bin"
echo "  ./FOVOverlay"
echo ""
