#include "raycasting.h"



Texture load_texture(const char *filename)
{

	SDL_Surface *surface = IMG_Load(filename);
	if (!surface)
	{
		fprintf(stderr, "Error loading texture: %s\n", IMG_GetError());
		return (Texture){NULL, 0, 0};
	}
	Texture texture;
	texture.texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	texture.width = surface->w;
	texture.height = surface->h;
	return (texture);
}
