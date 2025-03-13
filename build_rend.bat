@echo off
PUSHD %~dp0
START /b /w "" g++ -o out/main.exe src/main.cpp lib/toojpeg.cpp
CD out
IF EXIST "main.exe" (
  START /b /w main.exe
)
CD %~dp0
POPD
