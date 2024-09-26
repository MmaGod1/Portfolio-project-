#include "raycasting.h"

/**
 * render_walls - Renders the walls of the maze by casting rays from the
 * player's position and calculating their heights based on distance.
 *
 * This function loops through each vertical slice of the screen and determines
 * the angle for each ray. It calculates the distance to the nearest wall,
 * adjusts the distance to correct for fisheye effects, and renders the
 * appropriate wall texture based on the hit coordinates.
 *
 * @player: A pointer to the Player structure containing
 * the player's position and angle.
 */
void render_walls(Player *player)
{
	int x, mapX, mapY, wallHeight, wallTop, wallBottom. wallTextureIndex, texX;
	float rayAngle, distance, wallX, correctedDistance;
	
	for (x = 0; x < SCREEN_WIDTH; x++)
	{
		rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
		distance = cast_ray(player->x, player->y, rayAngle);
        
		if (distance > 10.0)
			distance = 10.0;

		/* Correct the distance to avoid fisheye effect */
		correctedDistance = distance * cos(rayAngle - player->angle);
		wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
		wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
		wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

		/* Ensure values are within screen bounds */
		if (wallTop < 0)
			wallTop = 0;
		if (wallBottom >= SCREEN_HEIGHT)
			wallBottom = SCREEN_HEIGHT - 1;

		/* Determine which wall texture to use based on map hit */
		wallX = get_wall_hit_coordinates(
				player->x, player->y,
				rayAngle, &mapX, &mapY);
		wallTextureIndex = maze_map[mapX][mapY] - 1;

		/* Texture coordinates */
		int texWidth = wallTextures[wallTextureIndex].width;
		texX = (int)(wallX * texWidth) % texWidth;
		SDL_Rect srcRect = {
			texX, 0, 1,
			wallTextures[wallTextureIndex].height
		};
		SDL_Rect dstRect = { x, wallTop, 1, wallHeight };

		/*  Render wall slice */
		SDL_RenderCopy(
				renderer, wallTextures[wallTextureIndex].texture,
				&srcRect, &dstRect);
	}
}

/**
 * render - Clears the renderer and draws the current frame of the game,
 *          including the sky, floor, and walls.
 *
 * This function handles the rendering process, including clearing the screen,
 * drawing the sky and floor, casting rays to render the walls, and,
 * drawing a mini-map of the maze.
 *
 * @player: points to Player structure - the player's position and angle.
 */
void render(Player *player)
{
	SDL_RenderClear(renderer);

	draw_sky();
	draw_floor(player);

	/* Cast rays and render walls */
	render_walls(player);

	if (showMap)
	{
		draw_mini_map(player, showMap);
	}

	SDL_RenderPresent(renderer);
}
