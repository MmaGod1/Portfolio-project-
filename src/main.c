#include "raycasting.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Texture wallTextures[4]; 
Texture floorTexture;

/**
 * initialize_sdl - Initializes SDL, creates a window and a renderer.
 *
 * Return: 0 on success, or 1 if an error occurs.
 */
int initialize_sdl(void)
{
	window = SDL_CreateWindow("Go-Maze",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
				SDL_GetError());
		SDL_Quit();
		return (1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
				SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return (1);
	}
	return (0);
}

/**
 * initialize_player_and_load_resources - Initializes player and loads resources.
 *
 * @player: Pointer to the Player struct.
 * @mapfile: The name of the map file to load.
 *
 * Return: 0 on success, or 1 if an error occurs.
 */
int initialize_player_and_load_resources(Player *player, char *mapfile)
{
	int i;

	player->x = 2.0;
	player->y = 2.0;
	player->angle = 0.0;
	player->moveSpeed = 0.05;
	player->rotSpeed = 0.05;

	/* Load the map */
	if (load_map(mapfile, maze_map) != 0)
	{
		fprintf(stderr, "Failed to load map\n");
		return (1);
	}

	/* Load textures */
	wallTextures[0] = load_texture("./images/wall1.jpg");
	wallTextures[1] = load_texture("./images/wall2.jpg");
	wallTextures[2] = load_texture("./images/wall3.jpg");
	wallTextures[3] = load_texture("./images/wall4.jpg");
	floorTexture = load_texture("./images/floor.jpg");

	return (0);
}

/**
 * main - Entry point of the Go-Maze game.
 *
 * This function initializes SDL, creates a window and renderer, loads the map
 * and textures, and enters the main game loop where input is handled and the
 * game is rendered. It cleans up resources before exiting.
 *
 * @argc: The number of command-line arguments.
 * @argv: The array of command-line argument strings.
 *
 * Return: 0 on successful execution, or 1 if an error occurs.
 */
int main(int argc, char *argv[])
{
	Player player;
	bool running = true;

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

	if (initialize_sdl() != 0)
		return (1);

	if (initialize_player_and_load_resources(&player, argv[1]) != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return (1);
	}

	/* Game loop */
	while (running)
	{
		handle_input(&player, &running, maze_map);
		render(&player);
		SDL_Delay(16); /* Cap the frame rate to ~60 FPS */
	}

	/* Cleanup textures */
	for (int i = 0; i < 4; i++)
	{
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
