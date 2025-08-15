cmake_minimum_required(VERSION 3.16)
project(MedMetaExtractor VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)

# Include FetchContent for header-only libraries
include(FetchContent)

# Fetch nlohmann/json (header-only)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Optional: Find DCMTK (commented out for now, uncomment when needed)
# find_package(DCMTK QUIET)
# if(DCMTK_FOUND)
#     message(STATUS "DCMTK found: ${DCMTK_VERSION}")
#     add_definitions(-DHAVE_DCMTK)
# else()
#     message(STATUS "DCMTK not found - DICOM functionality will be disabled")
# endif()

# Create the executable
add_executable(medmeta src/main.cpp)

# Link libraries
target_link_libraries(medmeta PRIVATE
    nlohmann_json::nlohmann_json
    fmt::fmt
)

# Optional: Link DCMTK when available (uncomment when needed)
# if(DCMTK_FOUND)
#     target_link_libraries(medmeta PRIVATE ${DCMTK_LIBRARIES})
#     target_include_directories(medmeta PRIVATE ${DCMTK_INCLUDE_DIRS})
# endif()

# Set output directory
set_target_properties(medmeta PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Compiler-specific options
if(MSVC)
    target_compile_options(medmeta PRIVATE /W4)
else()
    target_compile_options(medmeta PRIVATE -Wall -Wextra -Wpedantic)
endif()