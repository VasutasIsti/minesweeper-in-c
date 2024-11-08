#include <stdio.h>
#include "econio.h"
#include "mswgame.h"
#include "rendererlinux.h"

void CreateAS(){
    printf(ENTER_AS);
    fflush(stdout);
}

void DestructAS(){
    printf(EXIT_AS);
    fflush(stdout);
}

char RenderedValue(Cell *cell){
    if (cell->isFlaged) return '#';
    else if (!cell->isVisited) return '-';
    else if (cell->neighbours == 0) return ' ';
    else if (cell->neighbours > 0) return cell->neighbours + '0';
    else if (cell->isBomb) return '*';
    else return '?';
}

void RenderTable(Board *board){
    printf("+%.*s+", board->sizeX, "---");
    for (int j = 0; j < board->sizeY; j++){
        printf("|");
        for (int i = 0; i < board->sizeX; i++){
            printf(" %c ", RenderedValue(&board->cells[i][j]));
        }
        printf("|");
    }
    printf("+%.*s+", board->sizeX, "---");
}

