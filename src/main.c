#include <SDL2/SDL.h>
#include "init.h"
#include "player.h"
#include "constants.h"

/* Rotation speed in radians */
#define ROTATION_SPEED 0.05

void handle_input(Player *player)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_LEFT:
                    /* Rotate camera left */
                    player->angle -= ROTATION_SPEED;
                    if (player->angle < 0)
                    {
                        player->angle += 2 * M_PI;
                    }
                    break;
                case SDLK_RIGHT:
                    /* Rotate camera right */
                    player->angle += ROTATION_SPEED;
                    if (player->angle >= 2 * M_PI)
                    {
                        player->angle -= 2 * M_PI;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player = { .x = 2, .y = 2, .angle = 0 }; /* Initial player position and angle */

    if (init_sdl(&window, &renderer) != 0)
    {
        return (1);
    }

    while (1)
    {
        handle_input(&player);
        draw(renderer, player);
        SDL_Delay(16); /* Limit frame rate to ~60 FPS */
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (0);
}
