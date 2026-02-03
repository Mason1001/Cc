#!/bin/bash
# Gaming Overlay Build Script for Linux/WSL

echo "Building Gaming Overlay..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    cd ..
    exit 1
fi

# Build the project
echo "Building project..."
cmake --build . --config Release -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Build failed!"
    cd ..
    exit 1
fi

echo "Build completed successfully!"
echo "Executable location: build/bin/GamingOverlay"

cd ..
