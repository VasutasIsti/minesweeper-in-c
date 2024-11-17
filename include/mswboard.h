#include "mswcell.h"
#pragma once

typedef struct Board{
    Cell **cells;     // This is the table
    int sizeX;
    int sizeY;
    double difficulty;    // 0.0-1.0, bombs/all cells ratio
} Board;

Board Init_Board(int x, int y, double diff);
void Destruct_Board(Board *board);

bool IsOnBoard(Board *board, int x, int y);
int NeighbourCount(Board *board, int x, int y);

double BombCountToDifficulty(int x, int y, int bombCount);
int DifficultyToBombCount(int x, int y, double diff);

bool IsEmptyListed(int **empties, int n, int x, int y);
int NeighbouringFlags(Board *board, int x, int y);

void PlaceBombs(Board *board);
void SetNeighbourCounts(Board *board);
void CheckAdjacents(Board *board, int x, int y, int **empties, int *n);
int FindAdjacentEmpties(Board *board, int x, int y);