#include "msw.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#pragma region Board related functions

Cell Init_Cell(){
    Cell cell = {false, false, false, 0};
    return cell;
}

Board Init_Board(int x, int y, double diff){
    Board board;
    board.sizeX = x;
    board.sizeY = y;
    board.difficulty = diff;
    board.cells = (Cell **)malloc(x * sizeof(Cell *));
    for (int i = 0; i < x; i++)
        board.cells[i] = (Cell *)malloc(y * sizeof(Cell));
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
            board.cells[i][j] = Init_Cell();
    return board;
}

MinesweeperGame Init_Game(int x, int y, double diff){
    MinesweeperGame game;
    game.board = Init_Board(x, y, diff);
    game.flagsRemaining = DifficultyToBombCount(&game.board);
}

void Destruct_Board(Board *board){
    for (int i = 0; i < board->sizeX; i++)
        free(board->cells[i]);
    free(board->cells);
    // Self Note: board is not dynamically allocated, so
    // there's no need to free it up. Nearly tried it. 
}

#pragma endregion
#pragma region Helper functions

bool IsOnBoard(Board *board, int x, int y){
    return (x >= 0 &&
            y >= 0 &&
            x < board->sizeX &&
            y < board->sizeY);
}

double BombCountToDifficulty(int x, int y, int bombCount){
    return (x*y)/(double)bombCount;
}

int DifficultyToBombCount(Board *board){
    return board->sizeX * board->sizeY * board->difficulty;
}

int NeighbourCount(Board *board, int x, int y){
    // Assuming the choosen cell is not a bomb
    int result = 0;
    for (int i = x-1; i <= x+1; i++)
        for (int j = y-1; j <= y+1; j++)
            if ((i == x && j == y) || !IsOnBoard(board, x, y))
                continue;
            else
                if (board->cells[i][j].isBomb)
                    result++;
    return result;
}

bool IsEmptyListed(int **empties, int n, int x, int y){
    bool isInList = false;
    for (int i = 0; i < n; i++)
        if (empties[i][0] == x && empties[i][1] == y)
            isInList = true;
    return isInList;
}

#pragma endregion
#pragma region Readability enhancers

void PlaceBombs(Board *board){
    int allBombs = DifficultyToBombCount(board);
    while (allBombs > 0){
        int x = rand() % board->sizeX;
        int y = rand() % board->sizeY;
        if (!board->cells[x][y].isBomb){
            board->cells[x][y].isBomb = true;
            board->cells[x][y].neighbours = -1;
            allBombs--;
        }
    }
}

void SetNeighbourCounts(Board *board){
    for (int i = 0; i < board->sizeX; i++)
        for (int j = 0; j < board->sizeY; j++)
            if (!board->cells[i][j].isBomb)
                board->cells[i][j].neighbours = NeighbourCount(board, i, j);
}

void CheckAdjacents(Board *board, int x, int y, int **empties, int *n){
    empties[*n][0] = x;
    empties[*n][1] = y;
    *n++;
    for (int i = x-1; i <= x+1; i++)
        for (int j = y-1; j <= y+1; j++)
            if ((i == x && j == y) ||
                board->cells[i][j].neighbours != 0 ||
                IsEmptyListed(empties, n, x, y))
                    continue;
            else if (!board->cells[i][j].isVisited)
                // It's just an optimization, if my math is mathing
                // It wont check cells that was visited before.
                CheckAdjacents(board, i, j, empties, n);
}

void FindAdjacentEmpties(Board *board, int x, int y){
    int entry[2];
    int **empties = (int **)malloc(board->sizeX * board->sizeY * sizeof(entry));
    int n = 0;
    CheckAdjacents(board, x, y, empties, &n);
    for (int i = 0; i < n; i++)
        board->cells[empties[i][0]][empties[i][1]].isVisited = true;
    free(empties);
}

#pragma endregion
#pragma region Actions

enum VisitOutcome VisitCell(Cell *cell, int x, int y){
    if (cell->isVisited) return VISITED;
    cell->isVisited = true;
    if (cell->neighbours > 0) return NUMBER;
    if (cell->isBomb) return LOST;
    return EMPTY;
}

void FlagCell(MinesweeperGame *game, int x, int y){
    // Maybe this was not a good idea...
    game->board.cells[x][y].isFlaged = !game->board.cells[x][y].isFlaged;
    if (game->board.cells[x][y].isFlaged)
        game->flagsRemaining--;
    else
        game->flagsRemaining++;
}

#pragma endregion
#pragma region Game Logic

void SetupGame(Board *board){
    // The board is already initialized
    PlaceBombs(board);
    SetNeighbourCounts(board);
}

void Next(MinesweeperGame *game, int x, int y){
    switch (VisitCell(&game->board, x, y)){
        case NUMBER:
            /*  The cell is visited by VisitCell(),
                There's no more task to do
            */
            break;
        case EMPTY:
            FindAdjacentEmpties(&game->board, x, y);
            break;
        case VISITED:
            /*  TODO: Check if there's as many flaged neighbours
                as the neighbourcount. After that, is there any
                neighbouring bomb cell, that is not flagged.
            */
            // FindAdjacentEmpties(board, x, y);
            break;
        case LOST:
            /*  A big TODO:
                At the time of writing, the Lost and Won conditions
                will be ui dependent. The data, such as isVisited will
                remain the same.
            */
            break;
        case WON:
            // A big TODO: Same as LOST
            break;
    }
}

#pragma endregion
