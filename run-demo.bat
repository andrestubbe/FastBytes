@echo off
echo Building and Running FastBytes Unified Interactive Demo...
cd examples
set MAVEN_OPTS=-Xmx4G
call mvn compile exec:java -Dexec.mainClass=fastbytes.Demo
cd ..
