#include "mswboard.h"
#pragma once

typedef struct MinesweeperGame{
    Board board;
    int flagsRemaining;
    int notvisited; // Count of cells that are not bombs nor visited
    // Time and other features can come here
} MinesweeperGame;

enum VisitOutcome{
    NUMBER,
    EMPTY,
    VISITED,
    BOMB
};

MinesweeperGame Init_Game(int x, int y, double diff);
// Note: Maybe a game destructer is needed if I want to make a
//       specific behaviour.

void SetupGame(Board *board);
void Flagging(MinesweeperGame *game, int x, int y);
void VisitedSelected(MinesweeperGame *game, int x, int y);

enum VisitOutcome VisitCell(Cell *cell);
void Next(MinesweeperGame *game, int x, int y);

// Placeholders at the moment
void Win(MinesweeperGame *game);
void Lose(MinesweeperGame *game);