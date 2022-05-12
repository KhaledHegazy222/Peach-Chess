#include "Game.hpp"
#include "Board.hpp"
#include "Engine.hpp"

#include <iostream>


Engine peachChess;

SDL_Renderer* Game::renderer;
int Game::PaddingLeft;
int Game::PaddingTop;
int Game::TileDimension;


Board* board = NULL;
const char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//const char* fen = "8/8/2K5/7r/7r/8/8/k7 b - - 0 1";
//const char* fen = "r6k/pn2bprp/4pNp1/2p1PbQ1/3p1P2/5NR1/PPP3PP/2B2RK1 w - - 0 1";



Game::Game() {
	board = new Board();
}
Game::~Game() {
	delete board;
}


void Game::init(const char* title, int x, int y, int w, int h) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		

		window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_RESIZABLE);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 120, 120, 120, 0);
		
		

		
		board->init(fen);
		width = w;
		height = h;
		isRunning = true;
		
		
		
		

	}

}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	
	
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			width = event.window.data1;
			height = event.window.data2;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX,&mouseY);
		board->hold((mouseY - PaddingTop) / TileDimension, (mouseX - PaddingLeft) / TileDimension);
		break;
	case SDL_MOUSEBUTTONUP:
		SDL_GetMouseState(&mouseX, &mouseY);
		board->release((mouseY - PaddingTop) / TileDimension, (mouseX - PaddingLeft) / TileDimension);
		break;
	default:
		break;
	}
	
	
}

void Game::update() {
	

	if (!endGame && board->turn == BLACK) {
		
		render();
		auto reply = peachChess.getBest(*board);
		if(~reply.first.row)
			board->performMove(reply.first, reply.second);
		else {
			endGame = true;
		}

	}
}

void Game::render() {
	resize();

	SDL_RenderClear(renderer);
	board->render();
	SDL_RenderPresent(renderer);
}



void Game::clear() {
	board->clear();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}


bool Game::running() {
	return isRunning;
}



void Game::resize() {
	TileDimension = std::min(height / 8, width / 8);
	PaddingTop = (height - TileDimension * 8) / 2;
	PaddingLeft = (width - TileDimension * 8) / 2;
}


