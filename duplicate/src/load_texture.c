#include "raycasting.h"

Texture load_texture(GameStats *gameStats, const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface)
    {
        fprintf(stderr, "Error loading texture: %s\n", IMG_GetError());
        return (Texture){NULL, 0, 0};
    }
    
    Texture texture;
    texture.texture = SDL_CreateTextureFromSurface(gameStats->renderer, surface);
    SDL_FreeSurface(surface);
    texture.width = surface->w;
    texture.height = surface->h;
    
    return texture;
}


int load_game_textures(GameStats *gameStats)
{
    gameStats->wallTextures[0] = load_texture(gameStats, 
        "./images/wall1.jpg");
    gameStats->wallTextures[1] = load_texture(gameStats, 
        "./images/wall2.jpg");
    gameStats->wallTextures[2] = load_texture(gameStats, 
        "./images/wall3.jpg");
    gameStats->wallTextures[3] = load_texture(gameStats, 
        "./images/wall4.jpg");
    gameStats->floorTexture = load_texture(gameStats, 
        "./images/floor.jpg");

    return (0);
}
