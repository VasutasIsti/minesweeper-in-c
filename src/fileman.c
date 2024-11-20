#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileman.h"
#include "mswgame.h"

char *strfile(FILE *f, int n)
{
    char c;
    char *str;

    bool end = fscanf(f, "%c", &c) != 1 || c == '\n';

    str = end ? (char *)malloc((n + 1) * sizeof(char)) : strfile(f, n+1);
    str[n] = end ? '\0' : c;

    return str;
}

char *readline(FILE *f)
{
    return strfile(f, 0);
}

// This portion of the program was made in a last minute
// enviroment, so there are many things that isn't the best
// I can do, but time is my enemy, and he it seems like
// she's wining.

void Init_FileMan(Entry *entries, char *fileName){
    if (fileName == NULL) {
        // Something is wrong within if...
        printf("Error: File Name not set");
        return;
    }
    printf("Filename in InitMan()=%s\n", fileName);
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file \"%s\"...\n", fileName);
        return;
    }
    // Only the best 10 entries are saved. Pretend that the file
    // is always correct, and no error can arrais from it being
    // messed up. Deadline moments

    // Oh, and the entries in the file are not in any order.
    for (int i = 0; i < 10; i++) {
        char *line = readline(file);
        Entry *e = entries+i;

        char booleaner = '\0';
        sscanf(line, "%[^;]", e->name);
        sscanf(line+strlen(e->name), ";%d;%d;%lf;%c", &e->x, &e->y, &e->diff, &booleaner);

        if (booleaner != '\0')
            entries[i].won = booleaner - '0';
        
        free(line);
    }
    fclose(file);
}

void WriteEntry(Entry *entry){
    printf("%s;%d;%d;%g;%d\n", entry->name, entry->x,
            entry->y, entry->diff, entry->won);
}

void WriteFileContent(Entry *entries){
    for (int i = 0; i < 10; i++)
        WriteEntry(&entries[i]);
}

void fileStuffs(MinesweeperGame *game, char *fileName){
    Entry *entries = (Entry *)malloc(10*sizeof(Entry));
    if (entries == NULL) {
        printf("Cannot allocate memory for file operations.\n");
        return;
    }
    Init_FileMan(entries, fileName);
    if (entries[0].name == NULL) {
        printf("Broken data in file \"%s\".\n", fileName);
        return;
    }
    // WriteFileContent(entries);
    if (entries == NULL)
        return;
    Entry *current = (Entry *)malloc(sizeof(Entry));
    strcpy(current->name, game->user);
    current->x = game->board.sizeX;
    current->y = game->board.sizeY;
    current->diff = game->board.difficulty;
    current->won = (game->state == WIN);

    // printf("Current "); WriteEntry(current);
    
    // quick info: the best entries are chosed by
    //      1. greater size (if equal, the latest)
    //      2. difficulty (same)
    //      3. was this a wining game (same)
    // THIS LOGIC IS TEMPORARY. Deadline moments

    // searching the least significant entry
    int mini = 0;
    for (int i = 1; i < 10; i++) {
        if ((entries[i].x * entries[i].y) >
            (entries[mini].x * entries[mini].y))
            continue;
        else if ((entries[i].x * entries[i].y) ==
            (entries[mini].x * entries[mini].y)) {
            if (entries[i].diff > entries[mini].diff)
                continue;
            else if (entries[i].diff == entries[mini].diff)
                if (entries[i].won || (!entries[i].won && !entries[mini].won))
                    continue;
        }
        mini = i;
        //printf("r%d=%d ", i, mini);
    }
    // printf("Falls out ");
    // WriteEntry(&entries[mini]);

    // A schizophrenic early return style or something 
    if ((current->x * current->y) < (entries[mini].x * entries[mini].y))
        ;
    else if (current->diff < entries[mini].diff)
        ;
    else if (!current->won || (!current->won && entries[mini].won))
        ;
    else {
        printf("Congrats, You've made it to the leaderboards.\n");
        strcpy(entries[mini].name, current->name);
        entries[mini].x = current->x;
        entries[mini].y = current->y;
        entries[mini].diff = current->diff;
        entries[mini].won = current->won;
    }

    FILE *file = fopen(fileName, "w");
    for (int i = 0; i < 10; i++)
        fprintf(file, "%s;%d;%d;%lf;%d\n",
                    entries[i].name,
                    entries[i].x,
                    entries[i].y,
                    entries[i].diff,
                    entries[i].won);
    printf("(\"%s\" has been updated.)\n", fileName);
    fclose(file);
}