#include "raycasting.h"

/**
 * load_texture - Loads a texture from a file and creates an SDL texture.
 * @renderer: The SDL_Renderer used to create the texture.
 * @filename: The path to the image file to load.
 *
 * This function loads an image from the specified filename into an SDL_Surface
 * and then creates an SDL_Texture from that surface. It also frees the surface
 * once the texture has been created. If the image cannot be loaded, it logs
 * an error and returns a Texture with a NULL pointer and zero dimensions.
 *
 * Return: Texture structure containing the loaded texture and its dimensions.
 */
Texture load_texture(SDL_Renderer *renderer, const char *filename)
{
	Texture texture, temp;
	SDL_Surface *surface = IMG_Load(filename);

	if (!surface)
	{
		fprintf(stderr, "Error loading texture: %s\n", IMG_GetError());
		temp.texture = NULL;
		temp.width = 0;
		temp.height = 0;

		return (temp);
	}

	texture.texture = SDL_CreateTextureFromSurface(renderer, surface);
	texture.width = surface->w;
	texture.height = surface->h;
	SDL_FreeSurface(surface);
	return (texture);
}

/**
 * load_resources - Loads the map and textures required for the game.
 * @renderer: The SDL_Renderer used for creating textures.
 * @mapfile: The name of the map file to load.
 * @floorTexture: Pointer to store the loaded floor texture.
 *
 * Return: 0 on success, or 1 if an error occurs.
 */
int load_resources(SDL_Renderer *renderer, char *mapfile, 
	SDL_Texture **floorTexture)
{
	if (load_map(mapfile, maze_map) != 0)
	{
		fprintf(stderr, "Failed to load map\n");
		return (1);
	}

	/* Load textures */
	wallTextures[0] = load_texture(renderer, "./images/wall1.jpg");
	wallTextures[1] = load_texture(renderer, "./images/wall2.jpg");
	wallTextures[2] = load_texture(renderer, "./images/wall3.jpg");
	wallTextures[3] = load_texture(renderer, "./images/wall4.jpg");
	*floorTexture = load_texture(renderer, "./images/floor.jpg");

	return (0);
}
