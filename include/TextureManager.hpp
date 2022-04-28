#pragma once
#include "Game.hpp"

class TextureManager {
    public:

        static SDL_Texture* loadTexture(const char* imgName);
        static void drawTexture(SDL_Texture* tex,SDL_Rect src,SDL_Rect dest);
};