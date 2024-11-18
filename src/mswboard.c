#include "mswboard.h"
#include <stdlib.h>
#include "debugmalloc.h"

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

void Destruct_Board(Board *board){
    for (int i = 0; i < board->sizeX; i++)
        free(board->cells[i]);
    free(board->cells);
    // Self Note: board is not dynamically allocated, so
    // there's no need to free it up. Nearly tried it. 
}

bool IsOnBoard(Board *board, int x, int y){
    return (x >= 0 &&
            y >= 0 &&
            x < board->sizeX &&
            y < board->sizeY);
}

int NeighbourCount(Board *board, int x, int y){
    // Assuming the choosen cell is not a bomb
    int result = 0;
    for (int i = x-1; i <= x+1; i++)
        for (int j = y-1; j <= y+1; j++)
            if ((i == x && j == y) || !IsOnBoard(board, i, j))
                continue;
            else if (board->cells[i][j].isBomb)
                result++;
    return result;
}

double BombCountToDifficulty(int x, int y, int bombCount){
    return (x*y)/(double)bombCount;
}

int DifficultyToBombCount(int x, int y, double diff){
    return (int)(x * y * diff);
}

// In the n long empties list of int[2]s (x and y coordinates), returns if position
// is in the empties list. 
bool IsEmptyListed(int **empties, int n, int x, int y){
    bool isInList = false;
    for (int i = 0; i < n; i++)
        if (empties[i][0] == x && empties[i][1] == y)
            isInList = true;
    return isInList;
}

int NeighbouringFlags(Board *board, int x, int y){
    int flags = 0;
    for (int i = x-1; i <= x+1; i++)
        for (int j = y-1; j <= y+1; j++)
            if (!IsOnBoard(board, i, j) || (i == x && j == y))
                continue;
            else if (board->cells[i][j].isFlaged)
                flags++;
    return flags;
}

void PlaceBombs(Board *board){
    int allBombs = DifficultyToBombCount(board->sizeX, board->sizeY, board->difficulty);
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
    (*n)++;        // -Wall is killing me here, like *n++; is killing him...
    for (int i = x-1; i <= x+1; i++)
        for (int j = y-1; j <= y+1; j++)
            if ((i == x && j == y) ||
                (!IsOnBoard(board, i, j)) ||
                (IsEmptyListed(empties, *n, i, j)))
                    continue;
            else if((board->cells[i][j].neighbours == 0) &&
                    (!board->cells[i][j].isVisited))
                CheckAdjacents(board, i, j, empties, n);
            else if (board->cells[i][j].neighbours > 0) {
                empties[*n][0] = i;
                empties[*n][1] = j;
                (*n)++;
            }

}

int FindAdjacentEmpties(Board *board, int x, int y){
    int size = board->sizeX * board->sizeY;
    int **empties = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        empties[i] = (int *)malloc(2*sizeof(int));
    int n = 0;
    CheckAdjacents(board, x, y, empties, &n);
    for (int i = 0; i < n; i++)
        board->cells[empties[i][0]][empties[i][1]].isVisited = true;

    for (int i = 0; i < size; i++)
        free(empties[i]);
    free(empties);
    return n;
}
