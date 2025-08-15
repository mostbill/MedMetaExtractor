@echo off
REM Build script for MedMetaExtractor
REM Requires: CMake, C++17 compiler (Visual Studio, MinGW, or Clang)

echo Building MedMetaExtractor...

REM Create build directory if it doesn't exist
if not exist build mkdir build

REM Change to build directory
cd build

REM Configure with CMake (adjust generator as needed)
REM For Visual Studio 2022:
REM cmake .. -G "Visual Studio 17 2022"
REM For MinGW:
REM cmake .. -G "MinGW Makefiles"
REM For default generator:
cmake ..

REM Build the project
cmake --build .

echo Build complete! Executable should be in bin/medmeta.exe
pause