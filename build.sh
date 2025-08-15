#!/bin/bash
# Build script for MedMetaExtractor
# Requires: CMake, C++17 compiler (GCC, Clang)

echo "Building MedMetaExtractor..."

# Create build directory if it doesn't exist
mkdir -p build

# Change to build directory
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

echo "Build complete! Executable should be in bin/medmeta"