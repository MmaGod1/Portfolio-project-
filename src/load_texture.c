#include "raycasting.h"

Texture load_texture(GameStats *gameStats, const char *filename)
{
    SDL_Surface *surface = IMG_Load(filename);
    Texture texture;

    if (!surface)
    {
        fprintf(stderr, "Error loading texture: %s\n", IMG_GetError());
        return (Texture){NULL, 0, 0};
    }

    texture.texture = SDL_CreateTextureFromSurface(gameStats->renderer, surface);
    SDL_FreeSurface(surface);
    texture.width = surface->w;
    texture.height = surface->h;

    return (texture);
}
