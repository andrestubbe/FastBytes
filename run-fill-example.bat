@echo off
chcp 65001 > nul
echo ⚡ Installing FastBytes...
call mvn -q clean install -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
cd examples
echo 🚀 Preparing dependencies...
call mvn -q clean package -DskipTests
echo 🚀 Running Fill Example...
java -Xmx4G --enable-native-access=ALL-UNNAMED -cp "target/classes;target/lib/*;../target/fastbytes-v0.1.0.jar" fastbytes.FillDemo
cd ..
pause