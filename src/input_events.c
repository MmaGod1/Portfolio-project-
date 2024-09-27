#include "raycasting.h"

/**
 * process_rotation - Rotates the player by a certain amount.
 * @player: Pointer to the Player structure - the player's current angle.
 * @direction: The direction to rotate (-1 for left, 1 for right).
 *
 * This function rotates the player by modifying the player's angle.
 * The rotation speed is determined by player->rotSpeed. If the angle goes
 * below 0 or above 2 * M_PI (360 degrees), it wraps around to ensure the
 * angle stays within the valid range.
 *
 * Return: void
 */
void process_rotation(Player *player, int direction)
{
	/* Update the player's angle based on direction */
	player->angle += direction * player->rotSpeed;

	/* Ensure the angle stays between 0 and 2 * M_PI */
	if (player->angle < 0)
	{
		player->angle += 2 * M_PI;
	}
	else if (player->angle > 2 * M_PI)
	{
		player->angle -= 2 * M_PI;
	}
}

/**
 * process_movement - Handles player movement and collision detection.
 * @player: Pointer to the Player structure containing position and angle.
 * @maze_map: 2D array representing the maze layout for collision detection.
 * @moveStep: The step size for the player's movement - positive or negative.
 * @moveAngle: The angle at which the player is moving.
 *
 * This function moves the player in the direction of moveAngle by moveStep.
 * It checks the maze_map to ensure the player doesn't move into a wall.
 *
 * Return: void
 */
void process_movement(Player *player,
		int maze_map[MAP_WIDTH][MAP_HEIGHT],
		float moveStep, float moveAngle)
{
	float newX, newY;
	int newXInt, newYInt;

	/* Calculate new player position based on moveStep and moveAngle */
	newX = player->x + cos(moveAngle) * moveStep;
	newY = player->y + sin(moveAngle) * moveStep;

	/* Convert newX and newY to integers for map boundary checking */
	newXInt = (int)newX;
	newYInt = (int)newY;

	/* Ensure the new position is within bounds and not colliding with walls */
	if (newXInt >= 0 && newXInt < MAP_WIDTH &&
			newYInt >= 0 && newYInt < MAP_HEIGHT)
	{
		/* Check for collisions with walls */
		if (maze_map[newXInt][(int)player->y] == 0)  /* Check horizontal movement */
		{
			player->x = newX;  /* Update player's x position */
		}
		if (maze_map[(int)player->x][newYInt] == 0)  /* Check vertical movement */
		{
			player->y = newY;  /* Update player's y position */
		}
	}
}


/**
 * process_quit_event - Handles the quit event.
 * @event: The SDL_Event structure containing event information.
 * @running: Pointer to a boolean indicating if the game is running.
 *
 * This function checks if the quit event (SDL_QUIT) has occurred
 * and sets the running flag to false to exit the game.
 *
 * Return: void
 */
void process_quit_event(SDL_Event event, bool *running)
{
	if (event.type == SDL_QUIT)
	{
		*running = false;
	}
}

/**
 * process_keydown_event - Handles keydown events for player actions.
 * @event: The SDL_Event structure containing event information.
 * @player: Pointer to the Player structure containing position and angle.
 * @running: Pointer to a boolean indicating if the game is running.
 * @maze_map: 2D array representing the maze layout for collision detection.
 * @showMap: Pointer to an integer indicating if the mini-map is shown (1) or hidden (0).
 *
 * This function handles key presses for movement, rotation, and toggling
 * the mini-map, delegating movement and rotation to other helper functions.
 *
 * Return: void
 */
void process_keydown_event(
	SDL_Event event,
	Player *player,
	bool *running,
	int maze_map[MAP_WIDTH][MAP_HEIGHT],
	bool showMap
)
{
	float moveStep = player->moveSpeed;
	float moveAngle = player->angle;

	switch (event.key.keysym.sym)
	{
		case SDLK_l:  /* Leave game */
			*running = false;
			break;
		case SDLK_m:  /* Toggle mini-map */
			*showMap = !(*showMap);
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
