#include <stdbool.h>
#pragma once

typedef struct Cell{
    bool isBomb;
    bool isFlaged;
    bool isVisited;
    int neighbours;
} Cell;

typedef struct Board{
    Cell **cells;     // This is the table
    int sizeX;
    int sizeY;
    double difficulty;    // 0.0-1.0, bombs/all cells ratio
} Board;

typedef struct MinesweeperGame{
    Board board;
    int flagsRemaining;
    // Time and other features can come here
} MinesweeperGame;

enum VisitOutcome{
    NUMBER,
    EMPTY,
    VISITED,    // TODO: To implement that mechanic
    WON,
    LOST
};

// Board related functions

Cell Init_Cell();
Board Init_Board(int x, int y, double diff);
MinesweeperGame Init_Game(int x, int y, double diff);
void Destruct_Board(Board *board);
// Note: Maybe a game destructer is needed if I want to make a
//       specific behaviour.

// Helper functions

bool IsOnBoard(Board *board, int x, int y);
double BombCountToDifficulty(int x, int y, int bombCount);
int DifficultyToBombCount(Board *board);
int NeighbourCount(Board *board, int x, int y);
bool IsEmptyListed(int **empties, int n, int x, int y);

// Readability enhancers :D

void PlaceBombs(Board *board);
void SetNeighbourCounts(Board *board);
void CheckAdjacents(Board *board, int x, int y, int **empties, int *n);
void FindAdjacentEmpties(Board *board, int x, int y);

// Actions

enum VisitOutcome VisitCell(Cell *cell, int x, int y);
void FlagCell(MinesweeperGame *game, int x, int y);

// Game Logic

void SetupGame(Board *board);
void Next(MinesweeperGame *game, int x, int y);