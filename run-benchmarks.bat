@echo off
echo ⚡ Building FastBytes...
call mvn clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
echo ⚡ Building Benchmarks...
cd benchmarks
call mvn clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( cd .. & pause & exit /b )
echo 🚀 Running JMH Benchmarks...
java -jar target/benchmarks.jar
cd ..
pause
