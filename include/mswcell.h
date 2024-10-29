#include <stdbool.h>
#pragma once

typedef struct Cell{
    bool isBomb;
    bool isFlaged;
    bool isVisited;
    int neighbours;
} Cell;

Cell Init_Cell();

bool FlagCell(Cell *cell);