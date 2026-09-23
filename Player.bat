@echo off

g++ -std=c++17 source/private/*.cpp -o Player.exe -I"C:/msys64/mingw64/include/opencv5" -L"../lib" -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lSDL2main -lSDL2 -lsfml-audio -lsfml-system -fdiagnostics-color=never > log.txt 2>&1

if %errorlevel% neq 0 (
    echo Compilation failed. Check log.txt
) else (
    echo Compilation succeeded!
)

pause