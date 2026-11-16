@echo off
if not exist "ExamPro.exe" (
    echo [Info] ExamPro.exe not found. Building first...
    call build.bat
    if errorlevel 1 exit /b 1
)
cls
ExamPro.exe
