#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>





class Game {
    private:

        bool isRunning;

        SDL_Window* window;

    public:

        static SDL_Renderer* renderer;
        static int width;
        static int height;
        Game();
        ~Game();

        void init(const char *title,int x,int y,int w,int h);
        void handleEvents();
        void update();
        void render();
        void clear();

        bool running() {return isRunning;}
        
        





};