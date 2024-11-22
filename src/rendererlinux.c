#include <stdio.h>
#include <stdlib.h>
#include "mswgame.h"
#include "econio.h"
#include "rendererlinux.h"
#include "debugmalloc.h"

void StartRendering() {
    printf(ENTER_AS);
    fflush(stdout);
    econio_set_title("Minesweeper in C - TEST");
    econio_rawmode();
}

void FinishRendering() {
    econio_normalmode();
    printf(EXIT_AS);
    fflush(stdout);
}

Cursor Init_Cursor() {
    Cursor cursor = {0, 0};
    return cursor;
}

ScreenPositions SetPositions(MinesweeperGame *game) {
    // WARNING: Not all the variables are set, use it at your risk.
    ScreenPositions sp;
    sp.corner_BR = (pos){game->board.sizeX*3+2 , game->board.sizeY+1};
    sp.belowTable = (pos){0, game->board.sizeY+2};
    
    return sp;
}

void CursorToCenter(Board *board, Cursor *cursor) {
    cursor->x = board->sizeX/2;
    cursor->y = board->sizeY/2;
}

char RenderedValue(Cell *cell) {
    if (cell->isFlaged) return '#';
    else if (!cell->isVisited) return '-';
    else if (cell->neighbours == 0) return ' ';
    else if (cell->neighbours > 0) return cell->neighbours + '0';
    else if (cell->isBomb) return '*';
    else return '?';
}

int RenderedValueColor(char c) {
    // Not the best way to implement, but it was fast
    if (c == '1') return COL_LIGHTBLUE;
    if (c == '2') return COL_GREEN;
    if (c == '3') return COL_RED;
    if (c == '4') return COL_BLUE;
    if (c == '5') return COL_BROWN;
    if (c == '6') return COL_CYAN;
    if (c == '7') return COL_BLACK;
    if (c == '8') return COL_LIGHTGRAY;
    if (c == '#') return COL_LIGHTGREEN;
    if (c == '-') return COL_DARKGRAY;
    if (c == '*') return COL_LIGHTRED;
    // And a default
    return COL_WHITE;
}

char *HorizontalBorder(Board *board) {
    char *hl = (char *)malloc((board->sizeX*3+2+1)*sizeof(char));
    int i;
    hl[0] = '+';
    for (i = 1; i < (board->sizeX*3) + 1; i++)
        hl[i] = '-';
    hl[i++] = '+';
    hl[i++] = '\0';
    return hl;
}

void RenderTable(Board *board) {

    // the upper and lower borders of the board
    char *hl = HorizontalBorder(board);
    econio_gotoxy(0,0);
    printf("%s\n", hl);
    for (int j = 0; j < board->sizeY; j++) {
        printf("|");
        for (int i = 0; i < board->sizeX; i++) {
            printf(" %c ", RenderedValue(&board->cells[i][j]));
        }
        printf("|\n");
    }
    printf("%s\n", hl);
    free(hl);
}

char RenderedValueD(Cell *cell) {
    if (cell->isFlaged) return '#';
    //else if (!cell->isVisited) return '-';
    else if (cell->neighbours == 0) return ' ';
    else if (cell->neighbours > 0) return cell->neighbours + '0';
    else if (cell->isBomb) return '*';
    else return '?';
}

void RenderDebugTable(Board *board) {
    char *hl = HorizontalBorder(board);
    econio_gotoxy(0, board->sizeY+10);
    printf("%s\n", hl);
    for (int j = 0; j < board->sizeY; j++) {
        printf("|");
        for (int i = 0; i < board->sizeX; i++) {
            printf(" %c ", RenderedValueD(&board->cells[i][j]));
        }
        printf("|\n");
    }
    printf("%s\n", hl);
    free(hl);
}

bool IsMovementValid(Board *board, Cursor *cursor, CursorDir dir) {
    if ((cursor->x == 0 && dir == LEFT) ||
        (cursor->x == board->sizeX-1 && dir == RIGHT) ||
        (cursor->y == 0 && dir == UP) ||
        (cursor->y == board->sizeY-1 && dir == DOWN))
            return false;
    return true;
}

void MoveCursor(Board *board, Cursor *cursor, CursorDir dir) {
    if (dir == UNSET) return;
    if (IsMovementValid(board, cursor, dir)) {
        DeRenderCursor(board, cursor);
        if (dir == UP)         cursor->y-=1;
        else if (dir == DOWN)  cursor->y+=1;
        else if (dir == LEFT)  cursor->x-=1;
        else if (dir == RIGHT) cursor->x+=1;
    }
    RenderCursor(board, cursor);
}

void RenderCursor(Board *board, Cursor *cursor) {
    econio_gotoxy(cursor->x*3+1, cursor->y+1);
    printf("<");
    econio_gotoxy(cursor->x*3+3, cursor->y+1);
    printf(">");
    econio_gotoxy(board->sizeX*3+2, board->sizeY+1);
    printf(" ");
    // put the cursor out of the table
}

void DeRenderCursor(Board *board, Cursor *cursor) {
    econio_gotoxy(cursor->x*3+1, cursor->y+1);
    printf(" ");
    econio_gotoxy(cursor->x*3+3, cursor->y+1);
    printf(" ");
}

void RenderLose(Board *board, Cursor *cursor) {
    char *hl = HorizontalBorder(board);
    econio_gotoxy(0, 0);
    printf("%s\n", hl);
    char v;
    for (int j = 0; j < board->sizeY; j++) {
        printf("|");
        for (int i = 0; i < board->sizeX; i++) {
            if (board->cells[i][j].isFlaged && board->cells[i][j].isBomb)
                v = '#';
            else if (board->cells[i][j].neighbours == 0)
                v = ' ';
            else if (board->cells[i][j].neighbours > 0)
                v = board->cells[i][j].neighbours + '0';
            else if ((board->cells[i][j].isBomb) &&
                (i == cursor->x && j == cursor->y))
                v = 'X';
            else if (board->cells[i][j].isBomb)
                v = '*';
            else
                v = '?';

            printf(" %c ", v);
        }
        printf("|\n");
    }
    printf("%s\n", hl);
    free(hl);
}

void GameLoop(MinesweeperGame *game, Cursor *cursor) {
    StartRendering();
    ScreenPositions sp = SetPositions(game);
    RenderTable(&game->board);
    RenderDebugTable(&game->board);
    CursorToCenter(&game->board, cursor);
    RenderCursor(&game->board, cursor);
    int key = 0;
    CursorDir dir = UNSET;

    while (true) {
        ResetAllDebugInfo(&game->dInfo);
        key = econio_getch();
        if (key == KEY_UP) dir = UP;
        else if (key == KEY_DOWN) dir = DOWN;
        else if (key == KEY_LEFT) dir = LEFT;
        else if (key == KEY_RIGHT) dir = RIGHT;
        else if (key == KEY_ESCAPE) break;
        else if (key == ' ') Next(game, cursor->x, cursor->y);
        else if (key == 'f' || key == 'F')
            Flagging(game, cursor->x, cursor->y);
        
        /* The lines below are used to debugging. Uncomment the
           necessary lines as needed. Not the best, I know, but
           time is ticking and I'm running out of it.*/

        econio_gotoxy(sp.belowTable.x, sp.belowTable.y+1);
        //printf("Empty=%d", game->dInfo.emptyCount);
        printf("FlagsRemaining = %03d\n", game->flagsRemaining);
        printf("NotVisited = %03d\n", game->notvisited);
        printf("GameState = %d\n", game->state);

        if (game->state == INGAME) {
            MoveCursor(&game->board, cursor, dir);
            RenderTable(&game->board);
            RenderCursor(&game->board, cursor);
            dir = UNSET;
        }
        else if (game->state == LOSE) {
            RenderLose(&game->board, cursor);
            econio_gotoxy(sp.belowTable.x, sp.belowTable.y);
            printf("Game Failed. The game exits in 3 seconds.\n");
            
            econio_sleep(3.0);
            break;
        }
        else if (game->state == WIN) {
            econio_gotoxy(sp.belowTable.x, sp.belowTable.y);
            printf("Game Won. Well done. The game exits in 3 seconds.\n");
            econio_sleep(3.0);
            break;
        }
    }

    FinishRendering();
}

