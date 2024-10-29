#include "mswboard.h"
#pragma once

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

MinesweeperGame Init_Game(int x, int y, double diff);
// Note: Maybe a game destructer is needed if I want to make a
//       specific behaviour.

enum VisitOutcome VisitCell(Cell *cell);
void Flagging(MinesweeperGame *game, int x, int y);

// Game Logic

void SetupGame(Board *board);
void Next(MinesweeperGame *game, int x, int y);