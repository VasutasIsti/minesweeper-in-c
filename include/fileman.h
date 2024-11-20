#include <stdio.h>
#include <stdbool.h>
#include "mswgame.h"

typedef struct Entry{
    char name[50+1];
    int x;
    int y;
    double diff;
    bool won; // It is a nightmare
} Entry;

void Init_FileMan(Entry *entries, char *fileName);
void fileStuffs(MinesweeperGame *game, char *fileName); // Last minute naming

void WriteEntry(Entry *entry);
void WriteFileContent(Entry *entries);
char *strfile(FILE *file, int n);
char *readline(FILE *file);