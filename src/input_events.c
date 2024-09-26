#include "raycasting.h"

int showMap = 1;  /* 1 to show map, 0 to hide map */

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
 *
 * This function handles key presses for movement, rotation, and toggling
 * the mini-map, delegating movement and rotation to other helper functions.
 *
 * Return: void
 */
void process_keydown_event(SDL_Event event, Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
	float moveStep = player->moveSpeed;
	float moveAngle = player->angle;

	switch (event.key.keysym.sym)
	{
		case SDLK_l:  /* Leave game */
			*running = false;
			break;
		case SDLK_m:  /* Toggle mini-map */
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

/**
 * handle_input - Processes player input and updates the player's state.
 * @player: Pointer to the Player structure containing position and angle.
 * @running: Pointer to a boolean indicating if the game is running.
 * @maze_map: 2D array representing the maze layout for collision detection.
 *
 * This function processes all user inputs and updates the player's state.
 * It handles quit events, key presses for movement, rotation, and toggling
 * the mini-map. Key press handling is delegated to the process_keydown_event function.
 *
 * Return: void
 */
void handle_input(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT])
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		process_quit_event(event, running);

		if (event.type == SDL_KEYDOWN)
		{
			process_keydown_event(event, player, running, maze_map);
		}
	}
}
