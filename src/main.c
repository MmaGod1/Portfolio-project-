#include "raycasting.h"

/**
 * initialize_sdl - Initializes SDL and creates the window and renderer.
 *
 * @window: Pointer to the SDL_Window to create.
 * @renderer: Pointer to the SDL_Renderer to create.
 *
 * Return: 0 on success, or 1 if an error occurs.
 */
int initialize_sdl(SDL_Window **window, SDL_Renderer **renderer)
{
    *window = SDL_CreateWindow("Go-Maze",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window)
    {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_Quit();
        return (1);
    }
    
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow(*window);
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
 * cleanup - Cleans up resources used by the game.
 *
 * @renderer: The SDL_Renderer to clean up.
 * @window: The SDL_Window to clean up.
 * @gameStats: Pointer to the GameStats structure containing textures.
 *
 * Return: void
 */
void cleanup(SDL_Renderer *renderer, SDL_Window *window, GameStats *gameStats) {
    for (int i = 0; i < 4; i++) {
        if (gameStats->wallTextures[i].texture) {
            SDL_DestroyTexture(gameStats->wallTextures[i].texture);
            gameStats->wallTextures[i].texture = NULL;
        }
    }

    if (gameStats->floorTexture.texture) {
        SDL_DestroyTexture(gameStats->floorTexture.texture);
        gameStats->floorTexture.texture = NULL;
    }

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
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    GameStats gameStats;
   SDL_Event event;

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

    if (initialize_sdl(&window, &renderer) != 0)
        return (1);

    initialize_player(&player);

    gameStats.renderer = renderer;
    if (load_resources(&gameStats, argv[1]) != 0)
    {
        cleanup(renderer, window, &gameStats);
        return (1);
    }

    /* Game loop */
    while (running)
    {
       while (SDL_PollEvent(&event))
       {
          void process_quit_event(event, &running);
          if (event.type == SDL_KEYDOWN)
             process_keydown_event(event, &player, &running, maze_map, showMap);
       }
        render(&player, showMap, renderer, &gameStats);
        SDL_Delay(16); /* Cap the frame rate to ~60 FPS */
    }

    cleanup(renderer, window, &gameStats);
    return (0);
}
