#include "raycasting.h"

extern int showMap = 1;  /* 1 to show map, 0 to hide map */

/**
 * handle_input - Processes player input and updates the player's state.
 *
 * This function handles user input for player movement and actions.
 * It listens for keyboard events to move the player in different
 * directions, rotate the player, toggle the mini-map display, or
 * exit the game. It also checks for collisions with walls in the maze.
 *
 * @player: Pointer to the Player structure containing position and angle.
 * @running: Pointer to a boolean that indicates if the game is running.
 * @maze_map: 2D array representing the maze layout for collision detection.
 *
 * Return: void
 */
void handle_input(Player *player,
		bool *running,
		int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
	SDL_Event event;
	float newX, newY. moveStep, moveAngle;
	int newXInt, newYInt;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			*running = false;
		}


		if (event.type == SDL_KEYDOWN)
		{
			moveStep = player->moveSpeed;
			moveAngle = player->angle;

			switch (event.key.keysym.sym)
			{
				/* Leave game when 'l' is pressed */
				case SDLK_l:
					*running = false;
					break;
				case SDLK_m:
					showMap = !showMap;
					break;
				case SDLK_LEFT:  /* Rotate left */
					player->angle -= player->rotSpeed;
					if (player->angle < 0)
						player->angle += 2 * M_PI;
					break;
				case SDLK_RIGHT:  /* Rotate right */
					player->angle += player->rotSpeed;
					if (player->angle > 2 * M_PI)
						player->angle -= 2 * M_PI;
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
					/* Skip rest of the loop if no relevant key is pressed */
			}

			/* Ensure the new position is within bounds */
			if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
			{
				newXInt = (int)newX;
				newYInt = (int)newY;

				/* Check for collisions with walls */
				if (maze_map[newXInt][(int)player->y] == 0)
				{
					player->x = newX;
				}
				if (maze_map[(int)player->x][newYInt] == 0)
				{
					player->y = newY;
				}
			}
		}
	}
}
