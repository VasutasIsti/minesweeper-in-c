#include "mswgame.h"

MinesweeperGame Init_Game(int x, int y, double diff){
    MinesweeperGame game;
    game.board = Init_Board(x, y, diff);
    game.flagsRemaining =
        DifficultyToBombCount(game.board.sizeX, game.board.sizeY, game.board.difficulty);
    return game;
}

enum VisitOutcome VisitCell(Cell *cell){
    if (cell->isVisited) return VISITED;
    cell->isVisited = true;
    if (cell->neighbours > 0) return NUMBER;
    if (cell->isBomb) return LOST;
    return EMPTY;
}

void Flagging(MinesweeperGame *game, int x, int y){
    // Maybe this was not a good idea...
    FlagCell(&game->board.cells[x][y]);
    if (game->board.cells[x][y].isFlaged)
        game->flagsRemaining--;
    else
        game->flagsRemaining++;
}

void SetupGame(Board *board){
    // The board is already initialized
    PlaceBombs(board);
    SetNeighbourCounts(board);
}

void Next(MinesweeperGame *game, int x, int y){
    switch (VisitCell(&game->board.cells[x][y])){
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
            if (NeighbouringFlags(&game->board, x, y) ==
                game->board.cells[x][y].neighbours)
                {}
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
