#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
class Game {
private :

	SDL_Window* window;
	int width;
	int height;
	bool isRunning = false;


public:

	void resize();
	static SDL_Renderer* renderer;

	static int PaddingLeft;
	static int PaddingTop;
	static int TileDimension;

	// Methods
	Game();
	~Game();
	void init(const char* title,int x,int y,int w,int h);
	void handleEvents();
	void update();
	void render();
	void clear();
	bool running();
	
	
};
