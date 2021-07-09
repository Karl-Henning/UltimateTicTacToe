//
// Created by Karl Henning on 18.01.21.
//

#include <SDL2/SDL.h>
#include "game.h"
#include "rendering.h"

void renderApp(SDL_Renderer *renderer, const appState  *app) {
    drawPlayable(renderer, app);
    drawWon(renderer, app);
    renderGrid(renderer, app);
    renderSpielstand(renderer, app);
}

void renderGrid (SDL_Renderer *renderer, const appState *app) {
    //Render big lines
    SDL_Rect VerticalLeft = {(SCREEN_WIDTH/3)-2, 5, 4, SCREEN_HEIGHT-10}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT
    SDL_Rect VerticalRight = {(2*SCREEN_WIDTH/3)-2, 5, 4, SCREEN_HEIGHT-10}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT
    SDL_Rect HorizontalTop = {5, (SCREEN_HEIGHT/3)-2, SCREEN_WIDTH-10, 4}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT
    SDL_Rect HorizontalBottom = {5, (2*SCREEN_HEIGHT/3)-2, SCREEN_WIDTH-10, 4}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderFillRect(renderer, &VerticalLeft);
    SDL_RenderFillRect(renderer, &VerticalRight);
    SDL_RenderFillRect(renderer, &HorizontalBottom);
    SDL_RenderFillRect(renderer, &HorizontalTop);

    //render small lines
    for(int i=0; i<9; i++){
        SDL_Rect VerticalLeft     = {((SCREEN_WIDTH/9)-1)+(i%3)*SCREEN_WIDTH/3, 10 + (i/3)*SCREEN_HEIGHT/3, 2, (SCREEN_HEIGHT/3)-20}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT
        SDL_Rect VerticalRight    = {((2*SCREEN_WIDTH/9)-1)+(i%3)*SCREEN_WIDTH/3, 10 + (i/3)*SCREEN_HEIGHT/3, 2, (SCREEN_HEIGHT/3)-20}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT
        SDL_Rect HorizontalTop    = {10 + (i%3)*SCREEN_WIDTH/3, ((SCREEN_HEIGHT/9)-1)+(i/3)*SCREEN_HEIGHT/3, (SCREEN_WIDTH/3)-20, 2}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT
        SDL_Rect HorizontalBottom = {10 + (i%3)*SCREEN_WIDTH/3, ((2*SCREEN_HEIGHT/9)-1)+(i/3)*SCREEN_HEIGHT/3, (SCREEN_WIDTH/3)-20, 2}; //x, y, SCREEN_WIDTH, SCREEN_HEIGHT

        if(app->player == 1 && app->payableAreas[i] == 1) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);//Farbe: blau
        } else if (app->player == 2 && app->payableAreas[i] == 1){
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 0);//Farbe: rot
        } else {
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 0);//Farbe: grau
        }

        SDL_RenderFillRect(renderer, &VerticalLeft);
        SDL_RenderFillRect(renderer, &VerticalRight);
        SDL_RenderFillRect(renderer, &HorizontalBottom);
        SDL_RenderFillRect(renderer, &HorizontalTop);
    }
}

void renderSpielstand(SDL_Renderer *renderer, const appState *app){

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 0);//Farbraum: rgba

    for(int i=0; i<81; i++){ //f�r jedes Element im ttt array
        if(app->TTT[i/9][i%9] == 1){
            //zeichne ein x
            int fieldX = (i%3)*(SCREEN_WIDTH/9) + ((i/9)%3)*SCREEN_WIDTH/3;
            int fieldY = ((i/3)%3)*(SCREEN_HEIGHT/9) + (i/27)*SCREEN_HEIGHT/3;

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);//Farbe: blau

            SDL_RenderDrawLine(renderer, fieldX + 5, fieldY + 5, fieldX + SCREEN_WIDTH/9 - 5, fieldY + SCREEN_WIDTH/9 - 5);
            SDL_RenderDrawLine(renderer, fieldX + SCREEN_WIDTH/9 - 5, fieldY + 5, fieldX + 5, fieldY + SCREEN_WIDTH/9 - 5);

        } else if (app->TTT[i/9][i%9] == 2) {
            //zeichne ein o
            int fieldXMid = (i%3)*(SCREEN_WIDTH/9) + ((i/9)%3)*SCREEN_WIDTH/3 + SCREEN_WIDTH/18;
            int fieldYMid = ((i/3)%3)*(SCREEN_HEIGHT/9) + (i/27)*SCREEN_HEIGHT/3 + SCREEN_HEIGHT/18;

            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 0);//Farbe: rot

            DrawCircle(renderer, fieldXMid, fieldYMid, (SCREEN_WIDTH/18)-10);
        }
    }
}

void DrawCircle(SDL_Renderer *renderer, int centreX, int centreY, int radius) {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void drawPlayable(SDL_Renderer *renderer, const appState  *app){
    for (int i=0; i<9; i++){
        if(app->payableAreas[i] == 1){
            SDL_Rect playable = {(i%3)*SCREEN_WIDTH/3, (i/3)*SCREEN_HEIGHT/3, SCREEN_WIDTH/3, SCREEN_HEIGHT/3};
            //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 0);//Farbe: hellgelb
            SDL_RenderFillRect(renderer, &playable);
        }
    }
}

void drawWon(SDL_Renderer *renderer, const appState  *app){
    for (int i=0; i<9; i++){
        if(app->stateTTTWon[i] != 0){
            SDL_Rect playable = {(i%3)*SCREEN_WIDTH/3, (i/3)*SCREEN_HEIGHT/3, SCREEN_WIDTH/3, SCREEN_HEIGHT/3};
            //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
            if (app->stateTTTWon[i] == 1)
                SDL_SetRenderDrawColor(renderer, 100, 100, 255, 0);//Farbe: hellblau
            else
                SDL_SetRenderDrawColor(renderer, 255, 80, 80, 0);//Farbe: hellrot
            SDL_RenderFillRect(renderer, &playable);
        }
    }
}

void renderMenu(SDL_Renderer *renderer, const appState  *app){
    renderGrid(renderer, app);
    renderMenuItems(renderer);

}

void renderMenuItems(SDL_Renderer *renderer){
    //render Boxes
    SDL_Rect local = {SCREEN_WIDTH/2 - 200, 2*SCREEN_HEIGHT/7 - 40, 400, 80};
    SDL_Rect ki1 = {SCREEN_WIDTH/2 - 200, 3*SCREEN_HEIGHT/7 - 40, 400, 80};
    SDL_Rect ki2 = {SCREEN_WIDTH/2 - 200, 4*SCREEN_HEIGHT/7 - 40, 400, 80};
    SDL_Rect ki3 = {SCREEN_WIDTH/2 - 200, 5*SCREEN_HEIGHT/7 - 40, 400, 80};

    SDL_SetRenderDrawColor(renderer, 11, 252, 3, 0);//Farbe: hellgrün

    SDL_RenderFillRect(renderer, &local);
    SDL_RenderFillRect(renderer, &ki1);
    SDL_RenderFillRect(renderer, &ki2);
    SDL_RenderFillRect(renderer, &ki3);

    //render numbers
    SDL_Rect one = {SCREEN_WIDTH/2 - 4, 3*SCREEN_HEIGHT/7 - 30, 8, 60};
    SDL_Rect two1 = {SCREEN_WIDTH/2 - 12, 4*SCREEN_HEIGHT/7 - 30, 8, 60};
    SDL_Rect two2 = {SCREEN_WIDTH/2 + 4, 4*SCREEN_HEIGHT/7 - 30, 8, 60};
    SDL_Rect three1 = {SCREEN_WIDTH/2 - 20, 5*SCREEN_HEIGHT/7 - 30, 8, 60};
    SDL_Rect three2 = {SCREEN_WIDTH/2 - 4, 5*SCREEN_HEIGHT/7 - 30, 8, 60};
    SDL_Rect three3 = {SCREEN_WIDTH/2 + 12, 5*SCREEN_HEIGHT/7 - 30, 8, 60};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);//Farbe: schwarz

    SDL_RenderFillRect(renderer, &one);
    SDL_RenderFillRect(renderer, &two1);
    SDL_RenderFillRect(renderer, &two2);
    SDL_RenderFillRect(renderer, &three1);
    SDL_RenderFillRect(renderer, &three2);
    SDL_RenderFillRect(renderer, &three3);
}

