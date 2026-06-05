@echo off
echo ðŸš€ Running Hero Demo...
cd examples\src
call mvn compile exec:java -Dexec.mainClass=fastbytes.benchmark.JMH_Copy
cd ..\..
pause
