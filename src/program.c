#include <stdio.h>
#include "mswgame.h"
#include "debugmalloc.h"
#include "fileman.h"

#define LINUX

#ifdef WIN
// TODO: make rendererWin.h
#endif
#ifdef LINUX
    #include "rendererlinux.h"
#endif
#ifdef SDL
// TODO: make rendererSDL.h
#endif

void GetInformation(char *name, int *x, int *y, double *diff){
    int locx = *x, locy = *y;
    double locd = *diff;
    // Hard limits
    int min = 5, max = 50;
    double minD = 0.01, maxD = 1.0;

    printf("Add the following paramaters.\n");
    printf("Name (max 49 characters): ");
    scanf("%s", name);
    int num;
    while (true){
        printf("Board width (5 - 50)[%d]: ", locx);
        num = scanf("%d", x);
        if (num != 1) {
            scanf("%*[^\n]");   // It's kinda stolen from infoc...
            printf("Not A Number, Try again.\n");
            num = scanf("%d", x);
        }
        else if (*x < min || *x > max) {
            printf("Not in range (5..50). Try again.\n");
        }
        else {
            // Assuming now the number is correct.
            break;
        }
    }
    while (true){
        printf("Board height (5 - 50)[%d]: ", locy);
        num = scanf("%d", y);
        if (num != 1) {
            scanf("%*[^\n]");
            printf("Not A Number, Try again.\n");
            num = scanf("%d", y);
        }
        else if (*y < min || *y > max) {
            printf("Not in range (5 - 50). Try again.\n");
        }
        else {
            // Assuming now the number is correct.
            break;
        }
    }
    while (true){
        printf("Bomb percentage (0.01 - 1.0)[%g]: ", locd);
        num = scanf("%lf", diff);
        if (num == 0) break;
        if (num != 1) {
            scanf("%*[^\n]");
            printf("Not A Number, Try again.\n");
            num = scanf("%lf", diff);
        }
        else if (*diff < minD || *diff > maxD) {
            printf("Not in range (0.01 - 1.0). Try again.\n");
        }
        else {
            // Assuming now the number is correct.
            break;
        }
    }

}

int main(){
    char name[50];
    int sizeX = 10, sizeY = 10;
    double difficulty = 0.2;
    char *fileName = "leaderboard.txt\0";
    GetInformation(name, &sizeX, &sizeY, &difficulty);
    MinesweeperGame game = Init_Game(name, sizeX, sizeY, difficulty);
    Cursor cursor = Init_Cursor();

    #ifdef LINUX
    GameLoop(&game, &cursor);
    #endif

    // printf("%s\n", fileName);
    if (game.state == WIN)
        fileStuffs(&game, fileName);

    Destruct_Board(&game.board);
}