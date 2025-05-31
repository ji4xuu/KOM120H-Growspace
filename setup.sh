#!/bin/bash
echo "Creating build directory..."
mkdir -p build
cd build
echo "Running CMake..."
cmake ..
echo "Building project..."
make
echo "Build finished. Executable is in the build directory."