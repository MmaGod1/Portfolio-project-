#include "raycasting.h"

int main(int argc, char *argv[])
{
    GameStats gameStats;

    memset(&gameStats, 0, sizeof(GameStats));

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <mapfile>\n", argv[0]);
        return (1);
    }

    /** Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", 
                SDL_GetError());
        return (1);
    }

    gameStats.window = SDL_CreateWindow("Go-Maze",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!gameStats.window)
    {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", 
                SDL_GetError());
        SDL_Quit();
        return (1);
    }

    gameStats.renderer = SDL_CreateRenderer(gameStats.window, -1,
        SDL_RENDERER_ACCELERATED);
    
    if (!gameStats.renderer)
    {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", 
                SDL_GetError());
        SDL_DestroyWindow(gameStats.window);
        SDL_Quit();
        return (1);
    }

    Player player = { .x = 2.0, .y = 2.0, .angle = 0.0,
                      .moveSpeed = 0.05, .rotSpeed = 0.05 };

    if (load_map(&gameStats, argv[1]) != 0)
    {
        fprintf(stderr, "Failed to load map\n");
        SDL_DestroyRenderer(gameStats.renderer);
        SDL_DestroyWindow(gameStats.window);
        SDL_Quit();
        return (1);
    }

    gameStats.wallTextures[0] = load_texture(&gameStats, 
        "./images/wall1.jpg");
    gameStats.wallTextures[1] = load_texture(&gameStats, 
        "./images/wall2.jpg");
    gameStats.wallTextures[2] = load_texture(&gameStats, 
        "./images/wall3.jpg");
    gameStats.wallTextures[3] = load_texture(&gameStats, 
        "./images/wall4.jpg");
    gameStats.floorTexture = load_texture(&gameStats, 
        "./images/floor.jpg");

    /** Game loop */
    bool running = true;
    while (running)
    {
        handle_input(&player, &running, &gameStats);
        render(&player, &gameStats);
        SDL_Delay(16);
    }

    for (int i = 0; i < 4; i++)
    {
        if (gameStats.wallTextures[i].texture)
        {
            SDL_DestroyTexture(gameStats.wallTextures[i].texture);
            gameStats.wallTextures[i].texture = NULL;
        }
    }

    if (gameStats.floorTexture.texture)
    {
        SDL_DestroyTexture(gameStats.floorTexture.texture);
        gameStats.floorTexture.texture = NULL;
    }

    /** Clean up and quit SDL */
    SDL_DestroyRenderer(gameStats.renderer);
    SDL_DestroyWindow(gameStats.window);
    SDL_Quit();

    return (0);
}
