@echo off
PUSHD %~dp0
start /b /w g++ -o out/main.exe src/main.cpp
cd out
start /b /w main.exe > rendered.ppm
cd %~dp0
popd
