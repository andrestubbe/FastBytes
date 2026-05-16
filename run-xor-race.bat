@echo off
chcp 65001 > nul
echo ⚡ Installing FastBytes...
call mvn -q clean install -DskipTests
if %ERRORLEVEL% NEQ 0 ( pause & exit /b )
cd examples
echo 🚀 Preparing dependencies...
call mvn -q clean package -DskipTests
cls
echo 🚀 Running XOR Visualizer...
java -Xmx4G --enable-native-access=ALL-UNNAMED --add-opens=java.base/java.lang=ALL-UNNAMED --add-opens=java.base/jdk.internal.misc=ALL-UNNAMED -cp "target/classes;target/lib/*;../target/fastbytes-v0.1.0.jar" fastbytes.XorRace
cd ..
pause
