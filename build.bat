@echo off
::gcc -m64 -c -Wall -Werror -I.\include .\src\program.c -o minesweeper.exe
::echo Build successful!

gcc -I"include" -Wall -Werror -o minesweeper.exe src/*.c