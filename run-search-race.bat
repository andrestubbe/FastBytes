@echo off
echo ⚡ Building FastBytes...
call mvn clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
echo 🚀 Running SIMD Search Race...
cd examples
call mvn compile exec:java -Dexec.mainClass=fastbytes.Demo2
cd ..
pause
