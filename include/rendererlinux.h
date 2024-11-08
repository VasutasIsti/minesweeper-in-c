#include "mswgame.h"

#define ENTER_AS "\033[?1049h"
#define EXIT_AS "\033[?1049l"

void CreateAS();
void DestructAS();
void RenderTable(Board *board);