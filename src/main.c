#include "raycasting.h"
  
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <mapfile>\n", argv[0]);
		return (1);
	}

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (1);
	}
    
	window = SDL_CreateWindow("Go-Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
    
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return (1);
	}
        
	/* Initialize player */
	Player player = { .x = 2.0, .y = 2.0, .angle = 0.0, .moveSpeed = 0.05, .rotSpeed = 0.05 };

	/* Load the map */
	if (load_map(argv[1], maze_map) != 0)
	{
		fprintf(stderr, "Failed to load map\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return (1);
	}

	/* Load textures */
	wallTextures[0] = loadTexture("./images/wall1.jpg");
	wallTextures[1] = loadTexture("./images/wall2.jpg");
	wallTextures[2] = loadTexture("./images/wall3.jpg");
	wallTextures[3] = loadTexture("./images/wall4.jpg");
	floorTexture = loadTexture("./images/floor.jpg");

	/* Game loop */
	bool running = true;
	while (running) {
		handle_input(&player, &running, maze_map);
		render(&player);

		/* Cap the frame rate to ~60 FPS */
		SDL_Delay(16);
	}

	/* Cleanup textures */
	for (int i = 0; i < 4; i++)
	{
		/* Free each texture if valid */
		if (wallTextures[i].texture)
		{
			SDL_DestroyTexture(wallTextures[i].texture);
			wallTextures[i].texture = NULL;
		}
	}
	if (floorTexture.texture)
	{
		SDL_DestroyTexture(floorTexture.texture);
		floorTexture.texture = NULL;
	}

	/* Clean up and quit SDL */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return (0);
}
