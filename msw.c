#include <msw.h>

Cell Init_Cell(bool bomb, bool flaged, bool visited, int neighbours){
    Cell cell = {bomb, flaged, visited, neighbours};
    return cell;
}

void Init_Board(int x, int y, double diff, Board *board) {
    board->sizeX = x;
    board->sizeY = y;
    board->difficulty = diff;
    board->cells[board->sizeX][board->sizeY];
}

void PlaceBombs(Board *board){
    
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

void SetNeighbourCount(Board *board, int x, int y, int neighbourCount){
    board->cells[x][y].neighbours = neighbourCount;
}