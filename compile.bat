@echo off
setlocal EnableDelayedExpansion

REM FastBytes Native DLL Build Script for Windows
REM Requires: Visual Studio 2019+ with C++ tools, JDK 17+

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not defined JAVA_HOME (
    set "JAVA_HOME=C:\Program Files\Java\jdk-25"
)
if not exist "%JAVA_HOME%\include\jni.h" (
    set "JAVA_HOME=C:\Program Files\Java\jdk-25"
)

set "JNI_INCLUDE=%JAVA_HOME%\include"
set "JNI_WIN=%JAVA_HOME%\include\win32"

REM Find Visual Studio
if not exist "%VSWHERE%" (
    echo ERROR: vswhere.exe not found. Install Visual Studio 2019+.
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VS_PATH=%%i"
)

if not defined VS_PATH (
    echo ERROR: Visual Studio with C++ tools not found.
    exit /b 1
)

echo Found Visual Studio at: %VS_PATH%

REM Setup build environment
call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 (
    echo ERROR: Failed to setup VC environment
    exit /b 1
)

REM Create build directory
if not exist build mkdir build

REM Compile with AVX2 and SSE4.2 support
echo Compiling FastBytes DLL...
cl.exe /O2 /arch:AVX2 /EHsc /MD /LD /W3 /nologo ^
    /I"%JNI_INCLUDE%" /I"%JNI_WIN%" ^
    native\fastbytes.cpp ^
    /Fobuild\fastbytes.obj ^
    /Febuild\fastbytes.dll ^
    /link /MACHINE:X64

if errorlevel 1 (
    echo ERROR: Compilation failed
    exit /b 1
)

echo.
echo ===================================
echo Build successful!
echo Output: build\fastbytes.dll
dir build\fastbytes.dll
echo ===================================

endlocal
