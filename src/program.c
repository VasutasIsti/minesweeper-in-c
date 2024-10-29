#include <stdio.h>
#include "mswgame.h"
#include "../../debugmalloc.h"


int main(){
    int sizeX = 10, sizeY = 10;
    double difficulty = 0.3;
    MinesweeperGame game = Init_Game(sizeX, sizeY, difficulty);
    Destruct_Board(&game.board);
}