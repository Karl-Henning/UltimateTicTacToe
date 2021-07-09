//
// Created by Karl Henning on 18.01.21.
//

#ifndef ULTIMATETICTACTOE_LOGIC_H
#define ULTIMATETICTACTOE_LOGIC_H

void click(appState *app, int x, int y);
void makeMove(appState *app, int big, int small);
int checkSmallTTT(appState *app, int big);
int checkBigTTT(appState *app);
void mcts(appState *app, int mode);
int simulateGame(appState *app, int bigStart, int smallStart);

#endif //ULTIMATETICTACTOE_LOGIC_H
