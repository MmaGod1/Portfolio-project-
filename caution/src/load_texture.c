#include "raycasting.h"


/**
 * load_texture - Loads a texture from a file and creates an SDL texture.
 *
 * @gameStats: Pointer to the GameStats struct containing the SDL renderer.
 * @filename: The path to the image file to load.
 *
 * Return: A Texture struct containing the SDL texture, width, and height.
 */
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


/**
 * load_game_textures - Loads all wall and floor textures for the game.
 *
 * @gameStats: Pointer to the GameStats struct containing texture arrays.
 *
 * Loads textures for the walls and floor from image files into the gameStats struct.
 * 
 * Return: 0 on success.
 */
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
