#include "mswgame.h"

#define ENTER_AS "\033[?1049h"
#define EXIT_AS "\033[?1049l"

typedef struct Cursor{
    int x;
    int y;
} Cursor;

typedef enum CursorDir{
    UP,
    DOWN,
    LEFT,
    RIGHT
} CursorDir;

void StartRendering();
void FinishRendering();
Cursor Init_Cursor();
void CursorToCenter(Board *board, Cursor *cursor);
bool CheckCursorMovement(Board *board, Cursor *cursor, CursorDir dir);
void RenderTable(Board *board);
void RenderCursor(Board *board, Cursor *cursor);

void GameLoop(MinesweeperGame *game, Cursor *cursor);