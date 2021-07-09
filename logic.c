#include <SDL2/SDL.h>
#include "game.h"
#include "logic.h"

void click(appState *app, int x, int y) {
    int colum = (int) x / ( (double) SCREEN_WIDTH / 9.0);
    int row = (int) y / ( (double)SCREEN_HEIGHT / 9.0);

    int bixX = (row/3)*3;
    int bigY = colum/3 + 1;
    int big = bixX + bigY;

    int smallX = (row%3)*3;
    int smallY = colum%3 + 1;
    int small = smallX + smallY;

    printf("row: %d, colum: %d, \nbig: %d, small: %d; \nFeld: %d\nMode: %d\n", row, colum, big, small, app->TTT[big][small], app->mode);

    big--;
    small--;

    int bot = 0;
    if (app->TTT[big][small] == 0 && app->payableAreas[big] != 0) {
        bot = 1;
    }

    makeMove(app, big, small);

    if (bot) {
        if(app->mode != 1) {
            mcts(app, app->mode);
        }
    }
}

void makeMove(appState *app, int big, int small){
    if (app->TTT[big][small] == 0 && app->payableAreas[big] != 0) {
        app->TTT[big][small] = app->player;

        if (checkSmallTTT(app, big)) {
            //printf("Player %d has Won in TTT %d\n", app->statePlayerWon, big);
            if (checkBigTTT(app)) {
                for (int i = 0; i < 9; ++i)
                    app->payableAreas[i] = 0;
                return;
                //printf("Player %d has Won! Congratulations.\n", app->statePlayerWon);
            }
        }

        if (app->stateTTTWon[small]) {
            int tie = 1;
            for (int i = 0; i < 9; i++) {
                if (app->stateTTTWon[i] == 0) {
                    tie = 0;
                    app->payableAreas[i] = 1;
                }
            }
            if (tie && checkBigTTT(app) == 0) {
                app->statePlayerWon = 3;
                //printf("No one has Won! Tie.\n");
                //for (int i=0; i<9; i++)
                  //  app->payableAreas[i] = 0;
            }
            app->payableAreas[small] = 0;
        } else {
            for (int i = 0; i < 9; ++i) {
                app->payableAreas[i] = 0;
            }
            app->payableAreas[small] = 1;
        }
        /*for (int i = 0; i < 9; ++i) {
            printf("%d, ", app->payableAreas[i]);
            if ((i+1)%3 == 0) {
                printf("\n");
            }
        }
        printf("\n");*/


        if (app->player == 1) {
            app->player = 2;
        } else {
            app->player = 1;
        }
    }
}

int checkSmallTTT(appState *app, int big) {
    // check rows
    for (int colum = 0; colum < 3; ++colum) {
        if (app->TTT[big][colum] != 0 && app->TTT[big][colum] == app->TTT[big][colum+3] && app->TTT[big][colum] == app->TTT[big][colum+6]) {
            app->stateTTTWon[big] = app->player;
            return 1;
        }
    }

    // check rows
    for (int row = 0; row < 3; ++row) {
        if (app->TTT[big][row*3] != 0 && app->TTT[big][row*3] == app->TTT[big][row*3+1] && app->TTT[big][row*3] == app->TTT[big][row*3+2]) {
            app->stateTTTWon[big] = app->player;
            return 1;
        }
    }

    // check diagonals
    if (app->TTT[big][0] != 0 && app->TTT[big][0] == app->TTT[big][4] && app->TTT[big][0] == app->TTT[big][8]) {
        app->stateTTTWon[big] = app->player;
        return 1;
    }
    if (app->TTT[big][2] != 0 && app->TTT[big][4] == app->TTT[big][2] && app->TTT[big][2] == app->TTT[big][6]) {
        app->stateTTTWon[big] = app->player;
        return 1;
    }

    int tie = 1;
    for (int i = 0; i < 9; ++i) {
        if (app->TTT[big][i] != 0) {
            tie = 0;
        }
    }

    if (tie) {
        app->stateTTTWon[big] = 3;
    }

    return 0;
}

int checkBigTTT(appState *app) { //returns 1 if 3 in a row (of anything)
    // check rows
    for (int colum = 0; colum < 3; ++colum) {
        if (app->stateTTTWon[colum] != 0 && app->stateTTTWon[colum] == app->stateTTTWon[colum+3] && app->stateTTTWon[colum] == app->stateTTTWon[colum+6]) {
            if(app->stateTTTWon[colum] != 3)
                return 1;
        }
    }

    // check rows
    for (int row = 0; row < 3; ++row) {
        if (app->stateTTTWon[row*3] != 0 && app->stateTTTWon[row*3] == app->stateTTTWon[row*3+1] && app->stateTTTWon[row*3] == app->stateTTTWon[row*3+2]) {
            if(app->stateTTTWon[row*3] != 3)
                return 1;
        }
    }

    // check diagonals
    if (app->stateTTTWon[0] != 0 && app->stateTTTWon[0] == app->stateTTTWon[4] && app->stateTTTWon[0] == app->stateTTTWon[8]) {
        if(app->stateTTTWon[0] != 3)
            return 1;
    }
    if (app->stateTTTWon[2] != 0 && app->stateTTTWon[2] == app->stateTTTWon[4] && app->stateTTTWon[2] == app->stateTTTWon[6]) {
        if(app->stateTTTWon[2] != 3)
            return 1;
    }

    app->statePlayerWon = app->player;

    return 0;
}

void mcts(appState *app, int mode) {
    int iterations;
    if (mode == 2){
        iterations = 1000;
    } else if (mode == 3) {
        iterations = 5000;
    } else if (mode == 4) {
        iterations = 10000;
    }

    int chances[9][9][3]; //big field, small field, winrates
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            for(int k=0; k<3; k++)
                chances[i][j][k] = 0; //fills Array with 0

    for(int i=0; i<9; i++){
        if(app->payableAreas[i] == 1){
            for(int j=0; j<9; j++){
                if(app->TTT[i][j] == 0) {
                    for(int k=0; k<iterations; k++)
                        chances[i][j][simulateGame(app, i, j)-1] += 1;
                    printf("big: %d small: %d : 1:%d, 2:%d, 3:%d\n", i, j, chances[i][j][0], chances[i][j][1], chances[i][j][2]); //gibt fuer jedes freie feld die gewinnchancen mit zufaelligen darauffolgenden zuegen
                }
            }
        }
    }

    int bestBig, bestSmall, mostWins;
    mostWins = 0;

    for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
            if(chances[i][j][1] > mostWins){
                mostWins = chances[i][j][1];
                bestBig = i;
                bestSmall = j;
            }
        }
    }
    makeMove(app, bestBig, bestSmall);
}

int simulateGame(appState *app, int bigStart, int smallStart) { //simuliert ein Spiel bis zum Ende
    appState simulatedApp;
    memcpy(&simulatedApp, app, sizeof(*app)); //kopiert app struct in simulatedApp
    makeMove(&simulatedApp, bigStart, smallStart); //macht den ersten move der bewertet werden soll

    while(checkBigTTT(&simulatedApp) == 0 && simulatedApp.statePlayerWon != 3) {
        makeMove(&simulatedApp, rand()%10, rand()%10); //macht zufaellige Zuege bis jemand gewinnt
        if(simulatedApp.statePlayerWon == 3)
            break;
    }
    /*printf("\n");
    for (int i = 0; i < 9; ++i) {
        printf("%d, ", simulatedApp.stateTTTWon[i]);
        if ((i+1)%3 == 0) {
            printf("\n");
        }
    }
    printf("\ngewonnen hat: %d\n", simulatedApp.statePlayerWon);*/
    return simulatedApp.statePlayerWon; //gibt den Gewinner zurueck oder 3 bei unentschieden
}
