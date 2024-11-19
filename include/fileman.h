#include <stdio.h>
#include <stdbool.h>
#include "mswgame.h"

typedef struct Entry{
    char *name;
    int x;
    int y;
    double diff;
    bool won; // It is a nightmare
} Entry;

void Init_FileMan(Entry *entries, char *fileName);
void WriteFileContent(Entry *entries);
void fileStuffs(MinesweeperGame *game, char *fileName); // Last minute naming