#include <stdio.h>
#include <stdbool.h>
#include "mswgame.h"

typedef struct Entry{
    char name[50];
    int x;
    int y;
    double diff;
    bool won;
} Entry;

Entry *Init_FileMan(char *fileName);

void fileStuffs(MinesweeperGame *game, char *fileName); // Last minute naming