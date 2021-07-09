#ifndef ULTIMATETICTACTOE_GAME_H
#define ULTIMATETICTACTOE_GAME_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

typedef struct {
    int TTT[9][9];
    int stateTTTWon[9];
    int payableAreas[9];
    int statePlayerWon;
    int player;
    int mode; //0: in Menu, 1: in normal game, 2: KI level 1, 3: KI level 2, 4: KI level 3
} appState;


#endif //ULTIMATETICTACTOE_GAME_H
