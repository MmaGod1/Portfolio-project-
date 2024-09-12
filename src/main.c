#include <SDL2/SDL.h>
#include "init.h"
#include "player.h"
#include "constants.h"

/* Rotation speed in radians */
#define ROTATION_SPEED 0.05

void handle_input(Player *player) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    float rotateSpeed = 0.05; // Speed of rotation

    if (state[SDL_SCANCODE_LEFT]) {
        // Rotate player to the left
        float oldDirX = player->dirX;
        player->dirX = player->dirX * cos(-rotateSpeed) - player->dirY * sin(-rotateSpeed);
        player->dirY = oldDirX * sin(-rotateSpeed) + player->dirY * cos(-rotateSpeed);

        float oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(-rotateSpeed) - player->planeY * sin(-rotateSpeed);
        player->planeY = oldPlaneX * sin(-rotateSpeed) + player->planeY * cos(-rotateSpeed);
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        // Rotate player to the right
        float oldDirX = player->dirX;
        player->dirX = player->dirX * cos(rotateSpeed) - player->dirY * sin(rotateSpeed);
        player->dirY = oldDirX * sin(rotateSpeed) + player->dirY * cos(rotateSpeed);

        float oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(rotateSpeed) - player->planeY * sin(rotateSpeed);
        player->planeY = oldPlaneX * sin(rotateSpeed) + player->planeY * cos(rotateSpeed);
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
