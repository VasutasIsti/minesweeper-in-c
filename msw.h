#include <stdbool.h>
#pragma once

typedef struct Cell{
    bool isBomb;
    bool isFlaged;
    bool isVisited;
    int neighbours;
} Cell;

typedef struct Board{
    Cell **cells;     // Ez lesz maga a tabla
    int sizeX;
    int sizeY;
    double difficulty;    // 0.0-1.0 kozott, bombak aranya
} Board;

Cell Init_Cell(bool bomb, bool flaged, bool visited, int neighbours);
Board Init_Board(int x, int y, double diff);
void PlaceBombs(Board *board);
int CountNeighbours(Board *board, int x, int y);
void SetNeigbourCount(Cell *cell, int neighbourCount);
double BombCountToDifficulty(int x, int y, int bombCount);
void VisitAllEmpty(Board *board);
