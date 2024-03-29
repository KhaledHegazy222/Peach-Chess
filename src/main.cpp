#include <stdlib.h>

#include <exception>
#include <iostream>

#include "Game.hpp"

int
main(int argc, char **argv) {
    const int FPS = 150;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Game *game = new Game();
    game->init("Peach Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
               800, 600);

    while (game->running()) {
        frameStart = SDL_GetTicks();
        try {
            game->handleEvents();
        } catch (...) {
            std::cout << "Exception Occured\n";
        }
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clear();
    delete game;

    //_CrtDumpMemoryLeaks();
    return 0;
}
