#include <stdio.h>
#include "mswgame.h"
#include "debugmalloc.h"

#define LINUX

#ifdef WIN
// TODO: make rendererWin.h
#endif
#ifdef LINUX
    #include "rendererlinux.h"
#endif
#ifdef SDL
// TODO: make rendererSDL.h
#endif

int main(){
    int sizeX = 10, sizeY = 10;
    double difficulty = 0.3;
    MinesweeperGame game = Init_Game(sizeX, sizeY, difficulty);
    Cursor cursor = Init_Cursor();

    #ifdef LINUX
    GameLoop(&game, &cursor);
    #endif

    // Game Loops place
    getchar();

    #ifdef LINUX
    FinishRendering();
    #endif

    Destruct_Board(&game.board);
}