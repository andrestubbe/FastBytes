@echo off
chcp 65001 >nul
cd /d "%~dp0"
echo [FastBytes] Running Race Benchmark...
cd examples
set MAVEN_OPTS=-Xmx4G
call mvn compile exec:java -Dexec.mainClass=fastbytes.CopyRace
cd ..\..
pause
