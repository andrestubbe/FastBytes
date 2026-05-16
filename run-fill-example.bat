@echo off
echo ⚡ Building FastBytes...
call mvn clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
echo 🚀 Running Technical Fill Example...
cd examples
call mvn compile exec:java -Dexec.mainClass=fastbytes.FillDemo
cd ..
pause
