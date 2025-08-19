# MedMetaExtractor

MedMetaExtractor is a robust C++ application designed for extracting metadata from DICOM medical imaging files with flexible configuration options. The tool supports both single file and batch directory processing, offering configurable field extraction, multiple output formats (CSV/JSON), and optional data anonymization features. Built with modern C++ practices and cross-platform compatibility, it provides a reliable solution for medical data processing workflows.

## Features

- **DICOM Metadata Extraction** - Extract metadata from DICOM files (.dcm)
- **Batch Processing** - Process single files or entire directories recursively
- **Configurable Fields** - JSON-based configuration for field selection
- **Multiple Output Formats** - Support for CSV and JSON output
- **Data Anonymization** - Optional SHA-256 hashing for sensitive fields
- **Cross-platform** - Windows, Linux, and macOS support
- **Modern C++17** - Built with modern C++ standards
- **Intelligent Fallback** - Graceful handling when DCMTK is unavailable

## Dependencies

- **C++17** - Modern C++ standard with filesystem support
- **CMake 3.15+** - Cross-platform build system
- **DCMTK** - DICOM Toolkit for medical image processing
- **nlohmann/json** - JSON parsing and generation library
- **fmt** - Modern formatting library (optional, enhances output)

## Installation

### Linux (Ubuntu/Debian)

```bash
# Install system dependencies
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libdcmtk-dev nlohmann-json3-dev libfmt-dev

# Clone and build
git clone <repository-url>
cd MedMetaExtractor
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### macOS

```bash
# Install dependencies via Homebrew
brew install cmake dcmtk nlohmann-json fmt

# Clone and build
git clone <repository-url>
cd MedMetaExtractor
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

### Windows

```powershell
# Install vcpkg package manager
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install dependencies
.\vcpkg install dcmtk nlohmann-json fmt

# Clone and build project
git clone <repository-url>
cd MedMetaExtractor
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
mingw32-make
```

## Build Instructions

### Standard CMake Build

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --parallel

# Optional: Install system-wide
sudo cmake --install .
```

### Development Build with Debug Info

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Generator-Specific Builds (Windows)

#### Visual Studio
```cmd
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

#### MinGW
```cmd
cmake .. -G "MinGW Makefiles"
mingw32-make
```

#### Ninja (if installed)
```cmd
cmake .. -G "Ninja"
ninja
```

## Dependencies

The project automatically handles dependencies:

- **nlohmann/json**: Modern C++ JSON library for parsing and generating JSON data. Downloaded via CMake FetchContent. Provides intuitive syntax for handling medical metadata, configuration files, and API responses.
- **fmt**: Fast and safe C++ formatting library with Python-style format strings. Detected via find_package (install separately if needed). Used for generating clean, readable log messages and formatted reports.
- **DCMTK**: Optional DICOM toolkit with intelligent fallback. Automatically detected and used when compatible libraries are available. When unavailable, DicomReader operates in safe fallback mode.

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
├── CMakeLists.txt          # Build configuration with DCMTK detection
├── README.md               # This file
├── .gitignore             # Version control exclusions
├── config.json            # Application configuration
├── cmake/
│   └── test_dcmtk.cpp     # DCMTK linkability test
├── src/
│   ├── main.cpp           # Main application
│   ├── ConfigParser.hpp   # Configuration parser header
│   ├── ConfigParser.cpp   # Configuration parser implementation
│   ├── DicomReader.hpp    # DICOM reader header
│   └── DicomReader.cpp    # DICOM reader implementation
└── build/                 # Build directory (created)
    └── bin/
        └── medmeta        # Executable
```

## DICOM Support

### DicomReader Class

The `DicomReader` class provides robust DICOM file processing with intelligent fallback:

- **Constructor**: `DicomReader(const std::string& filePath)`
- **Extract Fields**: `extractFields(const std::vector<std::string>& fields, bool anonymize = false)`
- **Supported Tags**: PatientID, StudyDate, Modality, StudyDescription, PatientName, StudyInstanceUID
- **Anonymization**: SHA-256 hashing for PatientID when enabled
- **Error Handling**: Graceful handling of corrupted or missing DICOM files

### DCMTK Integration Status

#### When DCMTK is Available
- Full DICOM tag extraction from `.dcm` files
- Real metadata parsing using DCMTK libraries
- Complete DICOM standard compliance

#### Fallback Mode (DCMTK Unavailable)
- Safe operation without crashes
- Returns "N/A" for all DICOM fields
- Clear user feedback about DCMTK status
- Application remains fully functional

### Installing DCMTK (Optional)

#### Windows
```cmd
# Using vcpkg (recommended)
vcpkg install dcmtk

# Using Chocolatey (runtime only)
choco install dcmtk
```

#### Ubuntu/Debian
```bash
sudo apt-get install libdcmtk-dev
```

#### macOS
```bash
brew install dcmtk
```

**Note**: The build system automatically detects DCMTK compatibility. If libraries are found but not linkable with your compiler, the system gracefully falls back to safe mode.

## Configuration

MedMetaExtractor uses JSON configuration files to specify extraction parameters and output settings:

- **output_format**: Output format ("csv" or "json")
- **output_file**: Optional output file path (omit for stdout)
- **fields**: Array of DICOM field names to extract
- **anonymize**: Enable SHA-256 hashing for sensitive data

### Example Configuration Files

#### Research Profile (`config/research_profile.json`)
```json
{
    "output_format": "json",
    "output_file": "research_results.json",
    "fields": [
        "PatientID",
        "StudyDate",
        "Modality",
        "StudyDescription",
        "SeriesDescription",
        "ImageType",
        "SliceThickness",
        "PixelSpacing"
    ],
    "anonymize": true
}
```

#### Clinical Profile (`config/clinical_profile.json`)
```json
{
    "output_format": "csv",
    "fields": [
        "PatientName",
        "PatientID",
        "StudyDate",
        "Modality",
        "InstitutionName",
        "StudyDescription"
    ],
    "anonymize": false
}
```

## Usage

### Command Line Interface

```bash
medmeta --input <dicom_file_or_directory> --config <config_file>
```

**Options:**
- `--input`: Path to DICOM file or directory containing .dcm files
- `--config`: Path to JSON configuration file
- `--help`: Display usage information

### Example Commands

```bash
# Process research data from directory to JSON file
./medmeta --input sample_data/ --config config/research_profile.json

# Extract clinical data from single file to CSV (stdout)
./medmeta --input file.dcm --config config/clinical_profile.json

# Process entire study directory with anonymization
./medmeta --input /path/to/study --config config/research_profile.json

# Windows example
medmeta.exe --input "C:\DICOM\Study001" --config "config\research_profile.json"
```

## Output Examples

### CSV Output (Clinical Profile)
```csv
FileName,PatientName,PatientID,StudyDate,Modality,InstitutionName,StudyDescription
sample.dcm,John Doe,12345,20231201,CT,General Hospital,Chest CT with contrast
study.dcm,Jane Smith,67890,20231202,MRI,Medical Center,Brain MRI T1 weighted
image.dcm,Bob Johnson,11111,20231203,XR,City Clinic,Chest X-ray PA view
```

### JSON Output (Research Profile with Anonymization)
```json
[
    {
        "FileName": "sample.dcm",
        "PatientID": "a1b2c3d4e5f67890abcdef1234567890abcdef12",
        "StudyDate": "20231201",
        "Modality": "CT",
        "StudyDescription": "Chest CT with contrast",
        "SeriesDescription": "Axial CT 5mm",
        "ImageType": "ORIGINAL\\PRIMARY\\AXIAL",
        "SliceThickness": "5.0",
        "PixelSpacing": "0.625\\0.625"
    },
    {
        "FileName": "study.dcm",
        "PatientID": "f6e5d4c3b2a1098765432109876543210987654321",
        "StudyDate": "20231202",
        "Modality": "MRI",
        "StudyDescription": "Brain MRI T1 weighted",
        "SeriesDescription": "T1 MPRAGE sagittal",
        "ImageType": "ORIGINAL\\PRIMARY\\T1",
        "SliceThickness": "1.0",
        "PixelSpacing": "1.0\\1.0"
    }
]
```

## Project Structure

```
MedMetaExtractor/
├── src/
│   ├── main.cpp              # Application entry point and CLI handling
│   ├── ConfigParser.hpp      # JSON configuration file parser
│   ├── ConfigParser.cpp
│   ├── DicomReader.hpp       # DICOM file reader and metadata extractor
│   ├── DicomReader.cpp
│   ├── OutputFormatter.hpp   # CSV/JSON output formatting
│   ├── OutputFormatter.cpp
│   ├── Logger.hpp            # Colored console logging system
│   └── Logger.cpp
├── config/
│   ├── research_profile.json # Research-focused configuration
│   └── clinical_profile.json # Clinical workflow configuration
├── CMakeLists.txt            # CMake build configuration
├── config.json              # Default configuration file
└── README.md                # Project documentation
```

## Key Features

### DICOM Processing
- **Robust File Handling**: Supports single files and recursive directory processing
- **DCMTK Integration**: Professional-grade DICOM parsing with intelligent fallback
- **Error Resilience**: Graceful handling of corrupted or invalid files
- **Cross-platform**: Native support for Windows, Linux, and macOS

### Data Management
- **Configurable Extraction**: JSON-based field selection for flexible workflows
- **Privacy Protection**: SHA-256 anonymization for sensitive patient data
- **Multiple Formats**: CSV for spreadsheet compatibility, JSON for programmatic use
- **Batch Processing**: Efficient handling of large datasets

### Developer Experience
- **Modern C++17**: Leverages filesystem library and modern language features
- **Colored Logging**: Visual feedback with green/yellow/red console output
- **Comprehensive Error Handling**: Detailed error messages and status reporting
- **Professional Architecture**: Clean separation of concerns and modular design

## Troubleshooting

### Common Build Issues

**Missing Dependencies**
```bash
# Ubuntu/Debian
sudo apt-get install libdcmtk-dev nlohmann-json3-dev

# macOS
brew install dcmtk nlohmann-json

# Windows (vcpkg)
vcpkg install dcmtk nlohmann-json fmt
```

**CMake Configuration Issues**
```bash
# Specify toolchain file for vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake

# Set custom library paths
cmake .. -DCMAKE_PREFIX_PATH="/usr/local;/opt/homebrew"
```

### Runtime Issues

**Permission Errors**
- Ensure read access to DICOM files: `chmod +r *.dcm`
- Verify write permissions for output directory
- Run with appropriate user privileges

**DCMTK Not Available**
- Application continues with limited functionality
- Install DCMTK for full DICOM support
- Check library installation and linking

### Debug Mode

For detailed troubleshooting:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
# Run with verbose output
./medmeta --input test.dcm --config config.json
```

## Performance Considerations

- **Memory Usage**: Efficient streaming for large datasets
- **Processing Speed**: Optimized for batch operations
- **Disk I/O**: Minimal file system overhead
- **Scalability**: Handles directories with thousands of files

## Contributing

Contributions are welcome! Please ensure:
- Code follows C++17 standards
- All tests pass before submission
- Documentation is updated for new features
- Commit messages are descriptive

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---

**MedMetaExtractor** - Professional DICOM metadata extraction for medical imaging workflows.