@echo off
chcp 65001 > nul
echo ⚡ Building FastBytes...
call mvn -q clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
set MAVEN_OPTS=-Xmx4G
echo 🚀 Running search example...
cd examples
call mvn -q compile exec:java -Dexec.mainClass=fastbytes.SearchDemo
cd ..
pause
