# MedMetaExtractor

A C++17 command-line tool for medical metadata extraction.

## Features

- **C++17** modern C++ standard
- **JSON parsing** with nlohmann/json library
- **String formatting** with fmt library
- **Extensible** architecture ready for DCMTK integration
- **Cross-platform** support (Windows, Linux, macOS)

## System Requirements

### Windows
- Visual Studio 2019 or later (with C++17 support)
- OR MinGW-w64 with GCC 8+
- OR Clang 7+
- CMake 3.16 or later

### Linux/macOS
- GCC 8+ or Clang 7+
- CMake 3.16 or later
- pkg-config (optional, for system libraries)

## Quick Build

### Windows
```cmd
mkdir build
cd build
cmake ..
cmake --build .
```

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Build Instructions

### 1. Clone and Setup
```bash
git clone <repository-url>
cd MedMetaExtractor
```

### 2. Configure Build
```bash
mkdir build
cd build
cmake ..
```

### 3. Build
```bash
cmake --build .
```

### 4. Run
```bash
# Windows
.\bin\medmeta.exe

# Linux/macOS
./bin/medmeta
```

## Generator-Specific Builds (Windows)

### Visual Studio
```cmd
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

### MinGW
```cmd
cmake .. -G "MinGW Makefiles"
mingw32-make
```

### Ninja (if installed)
```cmd
cmake .. -G "Ninja"
ninja
```

## Dependencies

The project automatically handles dependencies:

- **nlohmann/json**: Modern C++ JSON library for parsing and generating JSON data. Downloaded via CMake FetchContent. Provides intuitive syntax for handling medical metadata, configuration files, and API responses.
- **fmt**: Fast and safe C++ formatting library with Python-style format strings. Detected via find_package (install separately if needed). Used for generating clean, readable log messages and formatted reports.
- **DCMTK**: Optional, prepared for future integration

### Installing fmt (optional)

#### Windows (vcpkg)
```cmd
vcpkg install fmt
```

#### Ubuntu/Debian
```bash
sudo apt-get install libfmt-dev
```

#### macOS (Homebrew)
```bash
brew install fmt
```

## Project Structure

```
MedMetaExtractor/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── .gitignore             # Version control exclusions
├── src/
│   └── main.cpp           # Main application
└── build/                 # Build directory (created)
    └── bin/
        └── medmeta        # Executable
```

## Future Enhancements

- DCMTK integration for DICOM file processing
- Command-line argument parsing
- Medical metadata extraction algorithms
- Output format options (JSON, XML, CSV)
- Batch processing capabilities

## Troubleshooting

### CMake Configuration Issues
- Ensure CMake 3.16+ is installed
- Check C++17 compiler availability
- Clear build directory if switching generators

### Missing Dependencies
- fmt library: Install via package manager or vcpkg
- Compiler not found: Install Visual Studio Build Tools or GCC/Clang

### Build Errors
- Check compiler C++17 support
- Verify CMake generator compatibility
- Ensure internet connection for nlohmann/json download

## License

TBD - Add appropriate license for medical software