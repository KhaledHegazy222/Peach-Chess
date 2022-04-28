#include "Game.hpp"

Game* game;


int main(int argc,char** argv){
    game = new Game();


	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;
    
    game->init("Peach Chess",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600);
    

    while(game->running()){
        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();
        
		frameTime = SDL_GetTicks() - frameStart;
		if(frameDelay > frameTime){
			SDL_Delay(frameDelay - frameTime);
		}
    }

    game->clear();


    return 0;


}