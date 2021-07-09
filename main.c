//
// Created by Karl Henning on 14.01.21.
//

// Basic SDL2 Application

#include <stdio.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"

int main(int argc, char* argv[]) {

    int gamemode;

    printf("\n\t\t\t\t\t _____ _ _   _           _          _____ _         _____             _____         \n\t\t\t\t\t|  |  | | |_|_|_____ ___| |_ ___   |_   _|_|___ ___|_   _|___ ___ ___|_   _|___ ___ \n\t\t\t\t\t|  |  | |  _| |     | .'|  _| -_|    | | | |  _|___| | | | .'|  _|___| | | | . | -_|\n\t\t\t\t\t|_____|_|_| |_|_|_|_|__,|_| |___|    |_| |_|___|     |_| |__,|___|     |_| |___|___|\n\n");
    printf("\nRegeln\n\tEs wird auf einem Brett mit 9x9 Feldern gespielt, das in neun kleinere lokale Bretter von 3x3 Feldern eingeteilt ist. Die Spieler,\n\tX und O, ziehen abwechselnd, X beginnt. Der Spieler am Zug traegt sein Symbol in ein noch freies Feld ein (setzt in diesesFeld)\n\n\tDer Startspieler kann sein erstes Symbol in ein beliebiges Feld eintragen. Danach muss jeder Zug in dem lokalen Brett gesetzt\n\twerden, das durch die Position des vorhergehenden Zuges in dessen lokalem Brett gegeben ist. Wenn z. B. in das obere\n\trechte Feld eines lokalen Bretts gesetzt wird, muss der naechste Zug im oberen rechten lokalen Brett erfolgen. Der ziehende\n\tSpieler kann also nur waehlen, in welches freie Feld des gegebenen lokalen Bretts er setzt.\n\n\tZiel des Spiels ist es, drei lokale Bretter zu gewinnen, die ihrerseits auf dem globalen Brett eine waagerechte, senkrechte oder\n\tdiagonale Reihe bilden. Wenn keine Zuege mehr moeglich sind und kein Spieler die Siegbedingung erfuellt hat, endet das Spielunentschieden.\n\n\n");
    printf("\nDen naechsten Zug vorwegnehmen: \n\n\tJeder Zug, der auf einem lokalen Brett gespielt wird, bestimmt, wo der naechste Zug des Gegners gespielt werden darf.\n\tDies kann dazu fuehren, dass Bewegungen, die im normalen Tic-Tac-Toe-Bereich als schlecht angesehen werden, realisierbar sind, da der Gegner an ein\n\tanderes lokales Brett geschickt wird und moeglicherweise nicht sofort auf den gemachten Zug reagieren kann. Daher sind die Spieler gezwungen, das groessere\n\tSpielbrett in Betracht zu ziehen, anstatt sich nur auf das lokale Brett zu konzentrieren.\n\n\n");
    printf("\nVorhersehen der Zugfolgen: \n\n\tDas Visualisieren zukuenftiger Zweige des Spielbaums ist schwieriger als das Einbrett-Tic-Tac-Toe. Jede Bewegung bestimmt die\n\tnaechste Bewegung, und daher folgt die Vorhersehung zukuenftige Bewegungen einem viel weniger linearen Pfad. Zukuenftige Positionen sind nicht mehr\n\taustauschbar, jeder Schritt fuehrt zu stark unterschiedlichen moeglichen zukuenftigen Positionen. Dies macht es schwierig, den Spielbaum zu visualisieren,\n\twodurch moeglicherweise viele moegliche Pfade uebersehen werden. Auf gegnerische Zuege kann moeglicherweise nicht sofort reagiert werden. Daher sind die\n\tSpieler gezwungen, das groessere Spielbrett in Betracht zu ziehen, anstatt sich nur auf das lokale Brett zu konzentrieren.\n\n\n");
    printf("\nDas Spiel gewinnen:\n\n\tAufgrund der Regeln des Meta-Tic-Tac-Toe ist das globale Brett niemals direkt betroffen. Es wird nur durch Aktionen beeinflusst, die auf\n\tlokalen Brettern stattfinden. Dies bedeutet, dass jeder gespielte lokale Zug nicht dazu gedacht ist, das lokale Brett, sondern das globale Brett zu gewinnen.\n\tLokale Gewinne sind nicht wertvoll, wenn sie nicht zum Gewinnen des globalen Bretts verwendet werden koennen. Tatsaechlich kann es strategisch sein, dem\n\tGegner ein lokales Brett zu opfern, um selbst ein wichtigeres lokales Brett zu gewinnen. Diese zusaetzliche Komplexitaet macht es fuer den Menschen schwieriger,\n\tdie relative Wichtigkeit und Bedeutung von Bewegungen zu analysieren, und folglich ist es schwieriger, gut zu spielen.\n\n\n");
    printf("Enter Gamemode.\n1 : 2 Player local\n2 : 1 Player Level 1\n3 : 1 Player Level 2\n4 : 1 Player Level 3\n\n");
    scanf("%d", &gamemode);

    // Init video-Subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("Ultimate TicTacToe", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow ERROR: %s\n", SDL_GetError()); // stderr ist für Fehlermeldungen
        return EXIT_FAILURE;
    }

    // Create renderer -> Manages the window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (window == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer ERROR: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    appState app = {
            {
                {0, 0, 0, 0, 0, 0, 0, 0, 0}, //ttt array
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0}
            },
            {0, 0, 0, 0, 0, 0, 0, 0, 0}, //stateTTTWon
            {1, 1, 1, 1, 1, 1, 1, 1, 1}, //playableAreas
            0, //statePlayerWon
            1, //player
            gamemode
    };

    SDL_Event e; // current event
    int quit = 0; // time to quit

    // Event loop
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(app.mode == 0){
                        if(e.button.x > SCREEN_WIDTH/2 - 200 &&
                           e.button.x < SCREEN_WIDTH/2 + 200 &&
                           e.button.y > 2*SCREEN_HEIGHT/7 - 40 &&
                           e.button.y < 2*SCREEN_HEIGHT/7 + 40){
                            app.mode = 1;
                        } else if(e.button.x > SCREEN_WIDTH/2 - 200 &&
                           e.button.x < SCREEN_WIDTH/2 + 200 &&
                           e.button.y > 3*SCREEN_HEIGHT/7 - 40 &&
                           e.button.y < 3*SCREEN_HEIGHT/7 + 40){
                            app.mode = 2;
                        } else if(e.button.x > SCREEN_WIDTH/2 - 200 &&
                           e.button.x < SCREEN_WIDTH/2 + 200 &&
                           e.button.y > 4*SCREEN_HEIGHT/7 - 40 &&
                           e.button.y < 4*SCREEN_HEIGHT/7 + 40){
                            app.mode = 3;
                        } else if(e.button.x > SCREEN_WIDTH/2 - 200 &&
                           e.button.x < SCREEN_WIDTH/2 + 200 &&
                           e.button.y > 5*SCREEN_HEIGHT/7 - 40 &&
                           e.button.y < 5*SCREEN_HEIGHT/7 + 40){
                            app.mode = 4;
                        }
                    }
                    else {
                        click(&app,  e.button.x, e.button.y);
                    }
                default: { } // Do nothing
            }
        }

        // Render Frame
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);

        if(app.mode == 0) {
            renderMenu(renderer, &app);
        } else {
            renderApp(renderer, &app);
        }

        SDL_RenderPresent(renderer);

        usleep(1000);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
