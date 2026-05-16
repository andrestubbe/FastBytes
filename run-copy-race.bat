@echo off
echo ⚡ Building FastBytes...
call mvn clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
echo 🚀 Running 1GB Copy Race...
cd examples
call mvn compile exec:java -Dexec.mainClass=fastbytes.Demo
cd ..
pause
