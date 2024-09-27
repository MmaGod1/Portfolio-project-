#include "raycasting.h"

/**
 * load_texture - Loads an image from a file and creates an SDL texture.
 * @renderer: The SDL_Renderer used for creating the texture.
 * @file: The path to the image file to be loaded.
 *
 * This function loads an image from the specified file, converts it into
 * an SDL texture using the provided renderer, and returns a pointer to
 * the created texture. If the image loading or texture creation fails, 
 * it returns NULL and logs an error message.
 *
 * Return: A pointer to the created SDL_Texture on success, or NULL on failure.
 */
SDL_Texture *load_texture(SDL_Renderer *renderer, const char *file)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = IMG_Load(file);

    if (!surface)
    {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n",
            file, IMG_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        fprintf(stderr, "Unable to create texture from %s! SDL Error: %s\n",
            file, SDL_GetError());
        return NULL;
    }

    return (texture);
}


/**
 * load_resources - Loads the map and textures required for the game.
 * @renderer: The SDL_Renderer used for creating textures.
 * @mapfile: The name of the map file to load.
 * @floorTexture: A pointer to the SDL_Texture pointer for the floor texture.
 *
 * This function loads the maze map and the textures for the walls and 
 * floor. It populates the wallTextures array and sets the floorTexture 
 * to the loaded texture. 
 *
 * Return: 0 on success, or 1 if an error occurs.
 */
int load_resources(SDL_Renderer *renderer, char *mapfile, SDL_Texture **floorTexture)
{
    if (load_map(mapfile, maze_map) != 0)
    {
        fprintf(stderr, "Failed to load map\n");
        return (1);
    }

    /* Load textures */
    wallTextures[0].texture = load_texture(renderer, "./images/wall1.jpg");
    wallTextures[1].texture = load_texture(renderer, "./images/wall2.jpg");
    wallTextures[2].texture = load_texture(renderer, "./images/wall3.jpg");
    wallTextures[3].texture = load_texture(renderer, "./images/wall4.jpg");
    
    *floorTexture = load_texture(renderer, "./images/floor.jpg");

    return (0);
}
