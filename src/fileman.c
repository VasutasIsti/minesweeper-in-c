#include "fileman.h"
#include "mswgame.h"
// This portion of the program was made in a last minute
// enviroment, so there are many things that isn't the best
// I can do, but time is my enemy, and he it seems like
// she's wining.

Entry *Init_FileMan(char *fileName){
    if (fileName == NULL /*|| fileName == ""*/) {
        // Something is wrong within if...
        printf("Error: File Name not set");
        return NULL;
    }
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error while file handling...\n");
        return NULL;
    }
    // Only the best 10 entries are saved. Pretend that the file
    // is always correct, and no error can arrais from it being
    // messed up. Deadline moments

    // Oh, and the entries in the file are not in any order.
    Entry list[10];
    char line[256];
    for (int i = 0; i < 10; i++) {
        fscanf(file, "%s[^;]%d;%d;%lf;%d",
                    list[i].name,
                    list[i].x,
                    list[i].y,
                    list[i].diff,
                    list[i].won);
    }
    fclose(file);
    return list;
}

void fileStuffs(MinesweeperGame *game, char *fileName){
    Entry entries[10] = Init_FileMan(fileName);
    if (entries == NULL)
        return;
    Entry current = (Entry){game->user,
                            game->board.sizeX,
                            game->board.sizeY,
                            game->board.difficulty,
                            (game->state == WIN)};
    // quick info: the best entries are chosed by
    //      1. greater size (if equal, the latest)
    //      2. difficulty (same)
    //      3. was this a wining game (same)
    // THIS LOGIC IS TEMPORARY. Deadline moments

    // searching the least significant entry
    int mini = 0;
    for (int i = 1; i < 10; i++) {
        if ((entries[i].x * entries[i].y) <
            (entries[mini].x * entries[mini].y))
            continue;
        if (entries[i].diff < entries[mini].diff)
            continue;
        if (!entries[i].won || (!entries[i].won && entries[mini].won))
            continue;
        mini = i;
    }
    // A schizophrenic early return style or something 
    if ((current.x * current.y) < (entries[mini].x * entries[mini].y))
        ;
    else if (current.diff < entries[mini].diff)
        ;
    else if (!current.won || (!current.won && entries[mini].won))
        ;
    else {
        printf("Congrats, You've made it to the leaderboards.");
        entries[mini] = current;
    }

    FILE *file = fopen(fileName, "w");
    for (int i = 0; i < 10; i++)
        fprintf(file, "%s;%d;%d;%lf;%d\n",
                    entries[i].name,
                    entries[i].x,
                    entries[i].y,
                    entries[i].diff,
                    entries[i].won);

    fclose(file);
}