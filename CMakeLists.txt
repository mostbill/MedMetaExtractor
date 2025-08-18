cmake_minimum_required(VERSION 3.16)
project(MedMetaExtractor VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find packages
find_package(PkgConfig QUIET)
find_package(fmt QUIET)

# Use FetchContent for nlohmann/json (header-only)
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Optional: DCMTK support (uncomment when needed)
# find_package(DCMTK QUIET)
# if(DCMTK_FOUND)
#     add_definitions(-DHAVE_DCMTK)
#     include_directories(${DCMTK_INCLUDE_DIRS})
# endif()

# Add executable
add_executable(medmeta 
    src/main.cpp
    src/ConfigParser.cpp
)

# Link libraries
target_link_libraries(medmeta PRIVATE nlohmann_json::nlohmann_json)

# Link fmt if found
if(fmt_FOUND)
    target_link_libraries(medmeta PRIVATE fmt::fmt)
    target_compile_definitions(medmeta PRIVATE HAVE_FMT)
else()
    message(STATUS "fmt library not found. Install with: vcpkg install fmt or apt-get install libfmt-dev")
endif()

# Optional: Link DCMTK when available
# if(DCMTK_FOUND)
#     target_link_libraries(medmeta PRIVATE ${DCMTK_LIBRARIES})
# endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(medmeta PRIVATE /W4)
else()
    target_compile_options(medmeta PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set executable name
set_target_properties(medmeta PROPERTIES OUTPUT_NAME "medmeta")

if(WIN32)
    set_target_properties(medmeta PROPERTIES OUTPUT_NAME "medmeta")
endif()