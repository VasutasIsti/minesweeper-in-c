#include "mswgame.h"
#include <stdlib.h>
#include <string.h>
#include "debugmalloc.h"

MinesweeperGame Init_Game(char *user, int x, int y, double diff){
    MinesweeperGame game;
    strcpy(game.user, user);
    game.state = INGAME;
    game.board = Init_Board(x, y, diff);
    game.flagsRemaining = DifficultyToBombCount(x, y, diff);
    game.notvisited = (x*y)-game.flagsRemaining;
    SetupGame(&game.board);

    game.dInfo = (debugInfos){0};

    return game;
}

void SetupGame(Board *board){
    // The board is already initialized
    PlaceBombs(board);
    SetNeighbourCounts(board);
}

void Flagging(MinesweeperGame *game, int x, int y){
    // To not have more flags than bombs
    if (game->flagsRemaining == 0 && !game->board.cells[x][y].isFlaged)
        return;
    if (game->board.cells[x][y].isVisited)
        return;
    bool state = FlagCell(&game->board.cells[x][y]);
    if (state) game->flagsRemaining--;
    else       game->flagsRemaining++;
}

int FindAdjacentEmpties(MinesweeperGame *game, int x, int y){
    int size = game->board.sizeX * game->board.sizeY;
    int **empties = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        empties[i] = (int *)malloc(2*sizeof(int));
    int n = 0;
    CheckAdjacents(&game->board, x, y, empties, &n);
    for (int i = 0; i < n; i++) {
        if (!game->board.cells[empties[i][0]][empties[i][1]].isVisited)
            game->notvisited--;
        game->board.cells[empties[i][0]][empties[i][1]].isVisited = true;
    }
    for (int i = 0; i < size; i++)
        free(empties[i]);
    free(empties);
    return n;
}

void VisitedSelected(MinesweeperGame *game, int x, int y){
    if (NeighbouringFlags(&game->board, x, y) !=
        game->board.cells[x][y].neighbours)
        return;
    int **localempties = (int **)malloc(8*sizeof(int *));
    for (int i = 0; i < 8; i++)
        localempties[i] = (int *)malloc(2*sizeof(int));
    int localn = 0;
    int i = 0, j = 0;
    // If any of the VALID cells are bombs and not flagged, the loop terminates early
    for (i = x-1; i <= x+1; i++){
        for (j = y-1; j <= y+1; j++){
            if ((i == x && j == y) || (!IsOnBoard(&game->board, i, j)) ||
                (game->board.cells[i][j].isVisited) || (game->board.cells[i][j].isFlaged) )
                continue;
            else if (game->board.cells[i][j].isBomb && !game->board.cells[i][j].isFlaged){
                break;
            }
            else {
                localempties[localn][0] = i;
                localempties[localn][1] = j;
                localn+=1;
            }
        }
        if (j != y+2)
            break;
    }
    if (i != x+2){
        for (int k = 0; k < 8; k++)
            free(localempties[i]);
        free(localempties);
        Lose(game);
    }
    else {
        // i and j were used, so every loop var is k now. Yeaaaah
        int size = game->board.sizeX * game->board.sizeY;
        int **empties = (int **)malloc(size*sizeof(int *));
        for (int k = 0; k < size; k++)
            empties[k] = (int *)malloc(2*sizeof(int));
        
        int n = 0;
        for (int k = 0; k < localn; k++){
            if (IsEmptyListed(empties, n, localempties[k][0], localempties[k][1]))
                continue;
            else if (game->board.cells[localempties[k][0]][localempties[k][1]].neighbours == 0)
                CheckAdjacents(&game->board, localempties[k][0], localempties[k][1], empties, &n);
            else{
                empties[n][0] = localempties[k][0];
                empties[n][1] = localempties[k][1];
                n++;
            }
        }

        for (int k = 0; k < n; k++) {
            if (!game->board.cells[empties[k][0]][empties[k][1]].isVisited)
                game->notvisited--;
            game->board.cells[empties[k][0]][empties[k][1]].isVisited = true;
        }
        for (int k = 0; k < 8; k++)
            free(localempties[k]);
        free(localempties);
        for (int k = 0; k < size; k++)
            free(empties[k]);
        free(empties);
    }
}

enum VisitOutcome VisitCell(Cell *cell){
    if (cell->isVisited) return VISITED;
    if (cell->isFlaged) return FLAGGED;
    cell->isVisited = true;
    if (cell->neighbours > 0) return NUMBER;
    if (cell->isBomb) return BOMB;
    return EMPTY;
}

void Next(MinesweeperGame *game, int x, int y){
    switch (VisitCell(&game->board.cells[x][y])){
        case FLAGGED:
            // Nothing to be done
            break;
        case NUMBER:
            /*  The cell is visited by VisitCell(),*/
            game->notvisited--;
            break;
        case EMPTY:
            game->dInfo.emptyCount = FindAdjacentEmpties(game, x, y);
            game->notvisited--;
            break;
        case VISITED: 
            VisitedSelected(game, x, y);
            break;
        case BOMB:
            Lose(game);
            break;
    }

    /* TODO: The only problem with the section bellow, is that you must try
       to visit not flagged cells if there's a critical state, which in most
       of the minesweepers is not needed, if you guess the correct flagging, it
       automatically wins the game... Not a big problem, but requires some
       attention sooner or later. */ 
    if (game->notvisited == 0)
        Win(game);
}

void Win(MinesweeperGame *game){
    /*TODO:*/
    game->state = WIN;
}

void Lose(MinesweeperGame *game){
    /*TODO:*/
    game->state = LOSE;
}

void ResetAllDebugInfo(debugInfos *dInfo){
    *dInfo = (debugInfos){0};
}