@echo off
REM Build ArchTrade with g++ (if available)
REM Alternatively: use CMake (cmake -B build && cmake --build build)
cd /d "%~dp0"
if not exist "src\main.cpp" ( echo Error: src\main.cpp not found. & exit /b 1 )
g++ -std=c++17 -O2 -o ArchTrade.exe src\main.cpp src\Workload.cpp -Isrc
if %ERRORLEVEL% equ 0 ( echo Build succeeded: ArchTrade.exe ) else ( echo Build failed. )
pause
