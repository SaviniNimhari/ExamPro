@echo off
setlocal enabledelayedexpansion

echo ============================================================
echo               ExamPro Build Script (C++)
echo ============================================================

:: 1. Check if cl.exe is already in PATH
where cl.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo [Info] Found MSVC cl.exe in PATH. Compiling...
    cl.exe /EHsc /std:c++17 /W3 main.cpp /Fe:ExamPro.exe
    goto check_result
)

:: 2. Check standard Visual Studio 2022 / 2019 installation paths
set "VS_VCVARS="
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set "VS_VCVARS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

if defined VS_VCVARS (
    echo [Info] Initializing MSVC environment via: "!VS_VCVARS!"
    call "!VS_VCVARS!" >nul 2>&1
    cl.exe /EHsc /std:c++17 /W3 main.cpp /Fe:ExamPro.exe
    goto check_result
)

:: 3. Check if g++ is available
where g++ >nul 2>&1
if %errorlevel% equ 0 (
    echo [Info] Found g++ compiler. Compiling with MinGW GCC...
    g++ -std=c++17 -Wall -O2 main.cpp -o ExamPro.exe
    goto check_result
)

echo [Error] No supported C++ compiler (MSVC or g++) was found in PATH or standard directories.
echo Please install Visual Studio 2022 or MinGW GCC, or run this from Developer Command Prompt.
exit /b 1

:check_result
if %errorlevel% equ 0 (
    echo ============================================================
    echo [SUCCESS] Build succeeded! Executable generated: ExamPro.exe
    echo ============================================================
) else (
    echo ============================================================
    echo [ERROR] Build failed. Please check compiler errors above.
    echo ============================================================
    exit /b %errorlevel%
)
