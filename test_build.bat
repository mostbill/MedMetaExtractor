@echo off
echo ========================================
echo MedMetaExtractor Build and Test Script
echo ========================================
echo.

echo [1] Testing simple compilation (no dependencies)...
echo.

REM Test simple version first
where g++ >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo ✓ g++ compiler found
    g++ -std=c++17 -o medmeta_simple.exe src\simple_main.cpp
    if %ERRORLEVEL% == 0 (
        echo ✓ Simple build successful!
        echo Running simple version:
        echo ----------------------------------------
        medmeta_simple.exe
        echo ----------------------------------------
        echo.
    ) else (
        echo ✗ Simple build failed
    )
) else (
    echo ✗ g++ compiler not found
)

echo [2] Testing CMake build (with dependencies)...
echo.

REM Test CMake build
where cmake >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo ✓ CMake found
    
    REM Clean and create build directory
    if exist build rmdir /s /q build
    mkdir build
    cd build
    
    echo Configuring project...
    cmake .. >nul 2>&1
    if %ERRORLEVEL% == 0 (
        echo ✓ CMake configuration successful
        echo Building project...
        cmake --build . >nul 2>&1
        if %ERRORLEVEL% == 0 (
            echo ✓ CMake build successful!
            if exist bin\medmeta.exe (
                echo Running full version:
                echo ----------------------------------------
                bin\medmeta.exe
                echo ----------------------------------------
            ) else (
                echo ✗ Executable not found in expected location
            )
        ) else (
            echo ✗ CMake build failed (likely missing dependencies)
            echo   Try installing: Visual Studio Build Tools, fmt library
        )
    ) else (
        echo ✗ CMake configuration failed (likely missing compiler)
        echo   Try installing: Visual Studio Build Tools or MinGW
    )
    cd ..
) else (
    echo ✗ CMake not found
)

echo.
echo ========================================
echo Build Summary:
echo ========================================
if exist medmeta_simple.exe (
    echo ✓ Simple version: medmeta_simple.exe (ready to use)
) else (
    echo ✗ Simple version: failed to build
)

if exist build\bin\medmeta.exe (
    echo ✓ Full version: build\bin\medmeta.exe (with dependencies)
) else (
    echo ✗ Full version: not built (install dependencies)
)

echo.
echo Next steps:
echo - Use medmeta_simple.exe for basic testing
echo - Install Visual Studio Build Tools + fmt for full version
echo - See README.md for detailed build instructions
echo.
pause