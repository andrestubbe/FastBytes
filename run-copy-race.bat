@echo off
chcp 65001 > nul
echo ⚡ Installing FastBytes...
call mvn -q install -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
cd examples
echo 🚀 Preparing dependencies...
call mvn -q package -DskipTests
echo 🚀 Running 1GB Copy Race...
java -Xmx4G --enable-native-access=ALL-UNNAMED -cp "target/classes;target/lib/*;../target/fastbytes-v0.1.0.jar" fastbytes.Demo
cd ..
pause