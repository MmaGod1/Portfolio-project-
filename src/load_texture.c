#include "raycasting.h"


/**
 * load_texture - Loads a texture from a file and creates an SDL texture.
 *
 * This function loads an image from the specified filename into an SDL_Surface
 * and then creates an SDL_Texture from that surface. It also frees the surface
 * once the texture has been created. If the image cannot be loaded, it logs
 * an error and returns a Texture with a NULL pointer and zero dimensions.
 *
 * @filename: The path to the image file to load.
 *
 * Return: Textured structure containing the loaded texture and its dimensions.
 */
Texture load_texture(const char *filename)
{
	Texture texture;
	SDL_Surface *surface = IMG_Load(filename);

	if (!surface)
	{
		fprintf(stderr, "Error loading texture: %s\n", IMG_GetError());
		Texture temp;
		temp.texture = NULL;
		temp.width = 0;
		temp.height = 0;

		return (temp);
	}

	texture.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	texture.width = surface->w;
	texture.height = surface->h;
	return (texture);
}
