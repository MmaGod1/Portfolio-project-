#include "raycasting.h"

int showMap = 1;  /* 1 to show map, 0 to hide map */

/**
 * process_movement - Processes player movement based on input.
 *
 * This function calculates the player's new position based on the current
 * angle and movement direction and ensures that the player does not collide
 * with walls in the maze.
 *
 * @player: Pointer to the Player structure containing position and angle.
 * @maze_map: 2D array representing the maze layout for collision detection.
 * @moveStep: The distance the player should move.
 * @moveAngle: The angle at which the player should move.
 *
 * Return: void
 */
void process_movement(Player *player, int maze_map[MAP_WIDTH][MAP_HEIGHT], float moveStep, float moveAngle)
{
	float newX, newY;
	int newXInt, newYInt;

	newX = player->x + cos(moveAngle) * moveStep;
	newY = player->y + sin(moveAngle) * moveStep;

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

/**
 * process_rotation - Processes player rotation based on input.
 *
 * This function updates the player's angle based on left or right
 * rotation keys.
 *
 * @player: Pointer to the Player structure containing position and angle.
 * @direction: Rotation direction (-1 for left, 1 for right).
 *
 * Return: void
 */
void process_rotation(Player *player, int direction)
{
	player->angle += direction * player->rotSpeed;
	if (player->angle < 0)
		player->angle += 2 * M_PI;
	else if (player->angle > 2 * M_PI)
		player->angle -= 2 * M_PI;
}

/**
 * handle_input - Processes player input and updates the player's state.
 *
 * This function handles user input for player movement and actions.
 * It listens for keyboard events to move the player in different
 * directions, rotate the player, toggle the mini-map display, or
 * exit the game. It delegates movement and rotation to helper functions.
 *
 * @player: Pointer to the Player structure containing position and angle.
 * @running: Pointer to a boolean that indicates if the game is running.
 * @maze_map: 2D array representing the maze layout for collision detection.
 *
 * Return: void
 */
void handle_input(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
	SDL_Event event;
	float moveStep, moveAngle;

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
				case SDLK_l:  /* Leave game when 'l' is pressed */
					*running = false;
					break;
				case SDLK_m:  /* Toggle mini-map display */
					showMap = !showMap;
					break;
				case SDLK_LEFT:  /* Rotate left */
					process_rotation(player, -1);
					break;
				case SDLK_RIGHT:  /* Rotate right */
					process_rotation(player, 1);
					break;
				case SDLK_w:  /* Move forward */
					process_movement(player, maze_map, moveStep, moveAngle);
					break;
				case SDLK_s:  /* Move backward */
					process_movement(player, maze_map, -moveStep, moveAngle);
					break;
				case SDLK_a:  /* Strafe left */
					process_movement(player, maze_map, moveStep, moveAngle - M_PI / 2);
					break;
				case SDLK_d:  /* Strafe right */
					process_movement(player, maze_map, moveStep, moveAngle + M_PI / 2);
					break;
				default:
					break;
			}
		}
	}
}
