#include <stdio.h>
#include "mswgame.h"
#include "debugmalloc.h"

#define LINUX

#ifdef WIN
#endif
#ifdef LINUX
#include "rendererlinux.h"
#endif

int main(){
    int sizeX = 10, sizeY = 10;
    double difficulty = 0.3;
    MinesweeperGame game = Init_Game(sizeX, sizeY, difficulty);

    #ifdef LINUX
    CreateAS();
    RenderTable(&game.board);
    #endif

    getchar();

    #ifdef LINUX
    DestructAS();
    #endif

    Destruct_Board(&game.board);
}