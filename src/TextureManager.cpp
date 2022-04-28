#include "TextureManager.hpp"


SDL_Texture* TextureManager::loadTexture(const char* imgName){
    SDL_Surface* tempSurface = IMG_Load(imgName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
}

void TextureManager::drawTexture(SDL_Texture* tex,SDL_Rect src,SDL_Rect dest){
    SDL_RenderCopy(Game::renderer,tex,&src,&dest);
}