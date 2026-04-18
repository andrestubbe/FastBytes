# Build Instructions

## Prerequisites

- Java 17+ (JDK with JNI headers)
- Visual Studio 2019+ (Community Edition works)
- Windows 10/11 x64
- SSE4.2+ CPU (Intel i3/i5/i7/i9 or AMD Ryzen)

## Quick Build

```bash
# Build native DLL
compile.bat

# Build JAR
mvn clean package -DskipTests
```

## Manual Build

### 1. Build Native DLL

```bash
# Setup Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

# Compile
cl.exe /O2 /arch:AVX2 /EHsc /MD /LD /W3 ^
    /I"%JAVA_HOME%\include" /I"%JAVA_HOME%\include\win32" ^
    native\fastbytes.cpp ^
    /Febuild\fastbytes.dll
```

### 2. Build Java

```bash
mvn clean compile
```

### 3. Run Tests

```bash
mvn test
```

## Troubleshooting

**"Cannot find jni.h"** → Set JAVA_HOME environment variable

**"cl.exe not found"** → Install Visual Studio C++ tools

**"AVX2 not supported"** → Remove `/arch:AVX2`, use `/arch:SSE2`
