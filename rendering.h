//
// Created by Karl Henning on 18.01.21.
//

#ifndef ULTIMATETICTACTOE_RENDERING_H
#define ULTIMATETICTACTOE_RENDERING_H

void renderMenu(SDL_Renderer *renderer, const appState  *app);

void renderApp(SDL_Renderer *renderer, const appState  *app);
void renderGrid (SDL_Renderer *renderer, const appState  *app);
void renderSpielstand(SDL_Renderer *renderer, const appState  *app);
void DrawCircle(SDL_Renderer * renderer, int centreX, int centreY, int radius);
void drawPlayable(SDL_Renderer *renderer, const appState  *app);
void drawWon(SDL_Renderer *renderer, const appState  *app);
void renderMenu(SDL_Renderer *renderer, const appState  *app);
void renderMenuItems(SDL_Renderer *renderer);

#endif //ULTIMATETICTACTOE_RENDERING_H
