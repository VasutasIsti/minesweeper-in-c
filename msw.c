#include "msw.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

Cell Init_Cell(bool bomb, bool flaged, bool visited, int neighbours){
    Cell cell = {bomb, flaged, visited, neighbours};
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
            board.cells[i][j] = Init_Cell(false, false, false, 0);
    return board;
}

void PlaceBombs(Board *board){
    int allBombs = (int)(board->sizeX * board->sizeY * board->difficulty);
    while (allBombs > 0){
        int x = rand() % board->sizeX;
        int y = rand() % board->sizeY;
        if (!board->cells[x][y].isBomb){
            board->cells[x][y].isBomb = true;
            allBombs--;
        }
    }
}

int CountNeighbours(Board *board, int x, int y){
    int result = 0;
    for (int i = x-1; i <= x+1; i++)
        for (int j = y-1; j <= y+1; j++)
            if (i < 0 || j < 0 || i >= board->sizeX || j >= board->sizeY)
                continue;
            else
                if (board->cells[i][j].isBomb)
                    result++;
    return result;
}

void SetNeighbourCount(Cell *cell, int neighbourCount){
    cell->neighbours = neighbourCount;
}

double BombCountToDifficulty(int x, int y, int bombCount){
    return (x*y)/(double)bombCount;
}

void VisitAllEmpty(Board *board){
    bool **visitedCells = (bool **)malloc(board->sizeX * sizeof(bool *));
    for (int i = 0; i < board->sizeX; i++)
        visitedCells[i] = (bool *)malloc(board->sizeY * sizeof(bool));
    for (int i = 0; i < board->sizeX; i++)
        for (int j = 0; j < board->sizeY; j++)
            visitedCells[i][j] = false;
    
}

void IsVisited(bool **visitedCells, Board *board){
    
}