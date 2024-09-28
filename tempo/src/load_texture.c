#include "raycasting.h"

/**
 * load_texture - Loads an image from a file and creates an SDL texture.
 * @renderer: The SDL_Renderer used for creating the texture.
 * @file: The path to the image file to be loaded.
 *
 * This function loads an image from the specified file, converts it into
 * an SDL texture using the provided renderer, and returns a Texture struct
 * that includes the created texture and its dimensions. If the image loading 
 * or texture creation fails, it returns a Texture struct with a NULL texture 
 * and zero dimensions.
 *
 * Return: A Texture struct containing the created SDL_Texture and its width
 * and height.
 */
Texture load_texture(SDL_Renderer *renderer, const char *file) {
    Texture tex;
    tex.texture = NULL;
    SDL_Surface *surface = IMG_Load(file);

    if (!surface) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n",
            file, IMG_GetError());
        tex.width = 0;
        tex.height = 0;
        return tex;
    }

    tex.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex.texture) {
        fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n",
            file, SDL_GetError());
        tex.width = 0;
        tex.height = 0;
        return tex;
    }

    SDL_QueryTexture(tex.texture, NULL, NULL, &tex.width, &tex.height);
    
    return (tex);
}



/**
 * load_resources - Loads the map and textures required for the game.
 * @gameStats: A pointer to the GameStats structure containing 
 *             the renderer and textures.
 * @mapfile: The name of the map file to load.
 *
 * This function loads the maze map and the textures for the walls and 
 * floor. It populates the wallTextures array and sets the floorTexture 
 * to the loaded texture. 
 *
 * Return: 0 on success, or 1 if an error occurs.
 */
int load_resources(GameStats *gameStats, char *mapfile)
{
    if (load_map(mapfile, maze_map) != 0)
    {
        fprintf(stderr, "Failed to load map\n");
        return 1;
    }

    // Load textures
    gameStats->wallTextures[0] = load_texture(gameStats->renderer, "./images/wall1.jpg");
    gameStats->wallTextures[1] = load_texture(gameStats->renderer, "./images/wall2.jpg");
    gameStats->wallTextures[2] = load_texture(gameStats->renderer, "./images/wall3.jpg");
    gameStats->wallTextures[3] = load_texture(gameStats->renderer, "./images/wall4.jpg");

    gameStats->floorTexture = load_texture(gameStats->renderer, "./images/floor.jpg");

    return 0;
}
