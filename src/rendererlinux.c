#include <stdio.h>
#include <stdlib.h>
#include "mswgame.h"
#include "econio.h"
#include "rendererlinux.h"
#include "debugmalloc.h"

void StartRendering(){
    printf(ENTER_AS);
    fflush(stdout);
    econio_set_title("Minesweeper in C - TEST");
    econio_rawmode();
}

void FinishRendering(){
    econio_normalmode();
    printf(EXIT_AS);
    fflush(stdout);
}

Cursor Init_Cursor(){
    Cursor cursor = {0, 0};
    return cursor;
}

void CursorToCenter(Board *board, Cursor *cursor){
    cursor->x = board->sizeX/2;
    cursor->y = board->sizeY/2;
}

char RenderedValue(Cell *cell){
    if (cell->isFlaged) return '#';
    else if (!cell->isVisited) return '-';
    else if (cell->neighbours == 0) return ' ';
    else if (cell->neighbours > 0) return cell->neighbours + '0';
    else if (cell->isBomb) return '*';
    else return '?';
}

char *HorizontalBorder(Board *board){
    char *hl = (char *)malloc((board->sizeX*3+2+1)*sizeof(char));
    int i;
    hl[0] = '+';
    for (i = 1; i < (board->sizeX*3) + 1; i++)
        hl[i] = '-';
    hl[i++] = '+';
    hl[i++] = '\0';
    return hl;
}

void RenderTable(Board *board){

    // the upper and lower borders of the board
    char *hl = HorizontalBorder(board);
    econio_gotoxy(0,0);
    printf("%s\n", hl);
    for (int j = 0; j < board->sizeY; j++){
        printf("|");
        for (int i = 0; i < board->sizeX; i++){
            printf(" %c ", RenderedValue(&board->cells[i][j]));
        }
        printf("|\n");
    }
    printf("%s\n", hl);
    free(hl);
}

bool CheckCursorMovement(Board *board, Cursor *cursor, CursorDir dir){
    if ((cursor->x == 0 && dir == LEFT) ||
        (cursor->x == board->sizeX-1 && dir == RIGHT) ||
        (cursor->y == 0 && dir == UP) ||
        (cursor->y == board->sizeY-1 && dir == DOWN))
            return false;
    return true;
}

void RenderCursor(Board *board, Cursor *cursor){
    econio_gotoxy(cursor->x*3+1, cursor->y+1);
    printf("<");
    econio_gotoxy(cursor->x*3+3, cursor->y+1);
    printf(">");
    econio_gotoxy(board->sizeX*3+2, board->sizeY+1);
    printf(" ");
    // put the cursor out of the table
}

void GameLoop(MinesweeperGame *game, Cursor *cursor){
    StartRendering();
    RenderTable(&game->board);
    CursorToCenter(&game->board, cursor);
    RenderCursor(&game->board, cursor);
    
}

