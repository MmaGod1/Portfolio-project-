#include "raycasting.h"

/**
 * initialize_sdl - Initializes SDL and creates the window and renderer.
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
 * initialize_player - Initializes the player with starting values.
 *
 * @player: Pointer to the Player struct to initialize.
 */
void initialize_player(Player *player)
{
    player->x = 2.0;
    player->y = 2.0;
    player->angle = 0.0;
    player->moveSpeed = 0.05;
    player->rotSpeed = 0.05;
}

/**
 * cleanup - Cleans up SDL resources.
 *
 * @renderer: The SDL_Renderer used to create the textures.
 */
void cleanup(SDL_Renderer *renderer)
{
    int i;

    for (i = 0; i < 4; i++)
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
    bool showMap = 1; /* 1 = show mini-map, 0 = hide mini-map */
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

    initialize_player(&player);

    if (load_resources(renderer, argv[1]) != 0)
    {
        cleanup(renderer);
        return (1);
    }

    /* Game loop */
    while (running)
    {
        handle_input(&player, &running, maze_map);
        render(&player);
        SDL_Delay(16); /* Cap the frame rate to ~60 FPS */
    }

    cleanup(renderer);

    return (0);
}
