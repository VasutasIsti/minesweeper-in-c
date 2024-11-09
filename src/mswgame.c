#include "mswgame.h"
#include <stdlib.h>
#include "debugmalloc.h"

MinesweeperGame Init_Game(int x, int y, double diff){
    MinesweeperGame game;
    game.board = Init_Board(x, y, diff);
    game.flagsRemaining = DifficultyToBombCount(x, y, diff);
    game.notvisited = (x*y)-game.flagsRemaining;
    SetupGame(&game.board);
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
    FlagCell(&game->board.cells[x][y]);
    if (game->board.cells[x][y].isFlaged)
        game->flagsRemaining--;
    else
        game->flagsRemaining++;
}

void VisitedSelected(MinesweeperGame *game, int x, int y){
    if (NeighbouringFlags(&game->board, x, y) ==
        game->board.cells[x][y].neighbours){
            int **localempties = (int **)malloc(8*sizeof(int[2]));
            int localn = 0;
            int i = 0, j = 0;
            // If any of the VALID cells are bombs and not flaged, the loop terminates early
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
                free(localempties);
                Lost(game);
            }
            else {
                int **empties = (int **)malloc((game->board.sizeX*game->board.sizeY)*sizeof(int[2]));
                int n = 0;
                for (int i = 0; i < localn; i++){
                    if (!IsEmptyListed(empties, n, localempties[i][0], localempties[i][1]))
                        CheckAdjacents(&game->board, localempties[i][0], localempties[i][1], empties, &n);
                }
                free(localempties);
                for (int i = 0; i < n; i++)
                    game->board.cells[empties[i][0]][empties[i][1]].isVisited = true;
                free(empties);
            }
    }
}

enum VisitOutcome VisitCell(Cell *cell){
    if (cell->isVisited) return VISITED;
    cell->isVisited = true;
    if (cell->neighbours > 0) return NUMBER;
    if (cell->isBomb) return BOMB;
    return EMPTY;
}

void Next(MinesweeperGame *game, int x, int y){
    switch (VisitCell(&game->board.cells[x][y])){
        case NUMBER:
            /*  The cell is visited by VisitCell(),
                There's no more task to do */
            break;
        case EMPTY:
            FindAdjacentEmpties(&game->board, x, y);
            break;
        case VISITED:
            VisitedSelected(game, x, y);
            break;
        case BOMB:
            Lost(game);
            break;
    }

    /* TODO: The only problem with the section bellow, is that you must try
       to visit not flagged cells if there's a critical state, which in most
       of the minesweepers is not needed, if you guess the correct flagging, it
       automatically wins the game... Not a big problem, but requires some
       attention sooner or later. */ 
    if (game->notvisited == 0 && game->flagsRemaining == 0)
        Win(game);
}

void Win(MinesweeperGame *game){
    /*TODO:*/

    Destruct_Board(&game->board);
}

void Lost(MinesweeperGame *game){
    /*TODO:*/

    Destruct_Board(&game->board);
}
