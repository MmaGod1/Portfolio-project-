#include "raycasting.h"


int initialize_sdl(GameStats *gameStats)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
                SDL_GetError());
        return (1);
    }

    gameStats->window = SDL_CreateWindow("Go-Maze",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!gameStats->window)
    {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_Quit();
        return (1);
    }

    gameStats->renderer = SDL_CreateRenderer(gameStats->window, -1,
        SDL_RENDERER_ACCELERATED);

    if (!gameStats->renderer)
    {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow(gameStats->window);
        SDL_Quit();
        return (1);
    }

    return (0);
}


int initialize_game(GameStats *gameStats, Player *player, char *mapFile)
{
    memset(gameStats, 0, sizeof(GameStats));

    if (initialize_sdl(gameStats) != 0)
    {
        return (1);
    }

    player->x = 2.0;
    player->y = 2.0;
    player->angle = 0.0;
    player->moveSpeed = 0.05;
    player->rotSpeed = 0.05;

    if (load_map(gameStats, mapFile) != 0)
    {
        fprintf(stderr, "Failed to load map\n");
        cleanup(gameStats);
        return (1);
    }

    load_game_textures(gameStats);

    return (0);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <mapfile>\n", argv[0]);
        return (1);
    }

    GameStats gameStats;
    Player player;

    if (initialize_game(&gameStats, &player, argv[1]) != 0)
    {
        return (1);
    }

    /** Game loop */
    bool running = true;
    while (running)
    {
        handle_input(&player, &running, &gameStats);
        render(&player, &gameStats);
        SDL_Delay(16);
    }

    clean_up(&gameStats);

    return (0);
}
