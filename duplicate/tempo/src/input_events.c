#include "raycasting.h"

void handle_input(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
    SDL_Event event;
    float newX, newY;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;

        if (event.type == SDL_KEYDOWN)
        {
            float moveStep = player->moveSpeed;
            float moveAngle = player->angle;

            switch (event.key.keysym.sym)
            {
                case SDLK_l:
                    *running = false;
                    break;
                case SDLK_m:
                    showMap = !showMap;
                    break;
                case SDLK_LEFT:
                    player->angle -= player->rotSpeed;
                    if (player->angle < 0) player->angle += 2 * M_PI;
                    break;
                case SDLK_RIGHT:
                    player->angle += player->rotSpeed;
                    if (player->angle > 2 * M_PI) player->angle -= 2 * M_PI;
                    break;
                case SDLK_w:
                    newX = player->x + cos(moveAngle) * moveStep;
                    newY = player->y + sin(moveAngle) * moveStep;
                    break;
                case SDLK_s:
                    newX = player->x - cos(moveAngle) * moveStep;
                    newY = player->y - sin(moveAngle) * moveStep;
                    break;
                case SDLK_a:
                    newX = player->x + cos(moveAngle - M_PI / 2) * moveStep;
                    newY = player->y + sin(moveAngle - M_PI / 2) * moveStep;
                    break;
                case SDLK_d:
                    newX = player->x + cos(moveAngle + M_PI / 2) * moveStep;
                    newY = player->y + sin(moveAngle + M_PI / 2) * moveStep;
                    break;
                default:
                    continue;
            }

            /* Perform collision detection once, checking both newX and newY */
            if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
            {
                if (maze_map[(int)newX][(int)newY] == 0)
                {
                    player->x = newX;
                    player->y = newY;
                }
            }
        }
    }
}
