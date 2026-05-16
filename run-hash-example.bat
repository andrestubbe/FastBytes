@echo off
echo ⚡ Building FastBytes...
call mvn -q clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
echo 🚀 Running Technical Hash Example...
cd examples
call mvn compile exec:java -Dexec.mainClass=fastbytes.HashDemo
cd ..
pause
