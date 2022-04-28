#include "Game.hpp"
#include "Board.hpp"

#include <iostream>
#include <stdlib.h>

SDL_Renderer* Game::renderer;
int Game::width;
int Game::height;


const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";


Board* board;


Game::Game(){}
Game::~Game(){}



void Game::init(const char *title,int x,int y,int w,int h){
    

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        

        window = SDL_CreateWindow(title,x,y,w,h,0);   

        renderer = SDL_CreateRenderer(window,-1,0);
        SDL_SetRenderDrawColor(renderer,120,120,120,0);
        
        isRunning = true;
        width = w;
        height = h;


        board = new Board(w,h);
        board->init(fen);
    }
}



void Game::handleEvents() {
    SDL_Delay(20);
    board->handleEvents();
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;
        
        default:
            break;
    }
}
void Game::update() {
    board->update();
}
void Game::render() {
    SDL_RenderClear(renderer);
    board->render();
    SDL_RenderPresent(renderer);
}
void Game::clear() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}