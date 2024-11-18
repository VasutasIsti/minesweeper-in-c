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
    RIGHT,
    UNSET
} CursorDir;

typedef struct pos{
    int x;
    int y;
} pos;

typedef struct ScreenPositions{
    // For storing positions of counters, texts, etc. on the screen
    pos corner_BR;
    pos belowTable;
    
    pos stats;
    // The following pos-es are relative to stats
    pos timeText;
    pos timer;
    pos flagsText;
    pos flagsCounter;

} ScreenPositions;

void StartRendering();
void FinishRendering();
Cursor Init_Cursor();
ScreenPositions SetPositions(MinesweeperGame *game);
void CursorToCenter(Board *board, Cursor *cursor);
char RenderedValue(Cell *cell);
int RenderedValueColor(char c);
char *HorizontalBorder(Board *board);
bool IsMovementValid(Board *board, Cursor *cursor, CursorDir dir);
void RenderTable(Board *board);
void MoveCursor(Board *board, Cursor *cursor, CursorDir dir);
void RenderCursor(Board *board, Cursor *cursor);
void DeRenderCursor(Board *board, Cursor *cursor);

void RenderLose(Board *board, Cursor *cursor);

void GameLoop(MinesweeperGame *game, Cursor *cursor);