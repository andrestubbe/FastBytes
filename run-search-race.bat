@echo off
echo ⚡ Building FastBytes...
call mvn -q clean package -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
set MAVEN_OPTS=-Xmx4G
echo 🚀 Running SIMD Search Race...
cd examples
call mvn compile exec:java -Dexec.mainClass=fastbytes.Demo2
cd ..
pause
