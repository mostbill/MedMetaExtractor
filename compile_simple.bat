@echo off
echo Compiling MedMetaExtractor (simple version)...

REM Try different compilers that might be available
REM Check for g++
where g++ >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo Using g++ compiler...
    g++ -std=c++17 -o medmeta.exe src\simple_main.cpp
    if %ERRORLEVEL% == 0 (
        echo Build successful! Running medmeta.exe...
        medmeta.exe
        goto :end
    )
)

REM Check for clang++
where clang++ >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo Using clang++ compiler...
    clang++ -std=c++17 -o medmeta.exe src\simple_main.cpp
    if %ERRORLEVEL% == 0 (
        echo Build successful! Running medmeta.exe...
        medmeta.exe
        goto :end
    )
)

REM Check for cl (MSVC)
where cl >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo Using MSVC compiler...
    cl /std:c++17 /EHsc src\simple_main.cpp /Fe:medmeta.exe
    if %ERRORLEVEL% == 0 (
        echo Build successful! Running medmeta.exe...
        medmeta.exe
        goto :end
    )
)

echo No suitable C++ compiler found.
echo Please install one of the following:
echo - Visual Studio (with C++ tools)
echo - MinGW-w64
echo - Clang
echo.
echo Or use the CMake build process with proper compiler setup.

:end
pause