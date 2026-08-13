@echo off
chcp 65001 >nul
cd /d "%~dp0"
echo ⚡ Building and Launching FastBytes Unified Demo...
cd examples
set MAVEN_OPTS=-Xmx4G
call mvn compile exec:java -Dexec.mainClass=fastbytes.Demo -q
cd ..
pause
