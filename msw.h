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
    double difficulty;    // 0-1 kozott, bombak aranya
} Board;

Cell Init_Cell(bool bomb, bool flaged, bool visited, int neighbours);
Board Init_Board(int x, int y, double diff, Board *board);
void PlaceBombs(Board *board);
int CountNeighbours(Board *board, int x, int y);
void SetNeigbourCount(Board *board, int x, int y, int neighbourCount);

