#include "raycasting.h"

void process_movement(Player *player, SDL_Keycode key, float *newX, float *newY)
{
    float moveStep = player->moveSpeed;
    float moveAngle = player->angle;

    switch (key)
    {
        case SDLK_w:
            *newX = player->x + cos(moveAngle) * moveStep;
            *newY = player->y + sin(moveAngle) * moveStep;
            break;
        case SDLK_s:
            *newX = player->x - cos(moveAngle) * moveStep;
            *newY = player->y - sin(moveAngle) * moveStep;
            break;
        case SDLK_a:
            *newX = player->x + cos(moveAngle - M_PI / 2) * moveStep;
            *newY = player->y + sin(moveAngle - M_PI / 2) * moveStep;
            break;
        case SDLK_d:
            *newX = player->x + cos(moveAngle + M_PI / 2) * moveStep;
            *newY = player->y + sin(moveAngle + M_PI / 2) * moveStep;
            break;
        default:
            *newX = player->x;
            *newY = player->y;
            break;
    }
}

void update_player_angle(Player *player, SDL_Keycode key)
{
    if (key == SDLK_LEFT)
    {
        player->angle -= player->rotSpeed;
        if (player->angle < 0)
            player->angle += 2 * M_PI;
    }
    else if (key == SDLK_RIGHT)
    {
        player->angle += player->rotSpeed;
        if (player->angle > 2 * M_PI)
            player->angle -= 2 * M_PI;
    }
}

bool check_new_position(GameStats *gameStats, float newX, float newY)
{
    if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
    {
        if (gameStats->maze_map[(int)newX][(int)newY] == 0)
        {
            return true;
        }
    }
    return false;
}

void handle_input(Player *player, bool *running, GameStats *gameStats)
{
    SDL_Event event;
    float newX, newY;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;

        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_l)
            {
                *running = false;
            }
            else if (event.key.keysym.sym == SDLK_m)
            {
                gameStats->showMap = !gameStats->showMap;
            }
            else
            {
                update_player_angle(player, event.key.keysym.sym);
                process_movement(player, event.key.keysym.sym, &newX, &newY);

                if (check_new_position(gameStats, newX, newY))
                {
                    player->x = newX;
                    player->y = newY;
                }
            }
        }
    }
}
