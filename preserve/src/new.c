#include "raycasting.h"

/**
 * calculate_step_and_side_dist - Calculate the step direction and
 * initial side distances.
 *
 * @rayDirX: The X direction of the ray.
 * @rayDirY: The Y direction of the ray.
 * @playerX: The player's X position.
 * @playerY: The player's Y position.
 * @mapX: Pointer to the map's X coordinate.
 * @mapY: Pointer to the map's Y coordinate.
 * @stepX: Pointer to store the step in the X direction.
 * @stepY: Pointer to store the step in the Y direction.
 * @sideDistX: Pointer to store the initial distance to the next X side.
 * @sideDistY: Pointer to store the initial distance to the next Y side.
 */
void calculate_step_and_side_dist(float rayDirX, float rayDirY,
	float playerX, float playerY, int *mapX, int *mapY,
	int *stepX, int *stepY, float *sideDistX, float *sideDistY)
{
	if (rayDirX < 0)
	{
		*stepX = -1;
		*sideDistX = (playerX - *mapX) * fabs(1 / rayDirX);
	}
	else
	{
		*stepX = 1;
		*sideDistX = (*mapX + 1.0 - playerX) * fabs(1 / rayDirX);
	}

	if (rayDirY < 0)
	{
		*stepY = -1;
		*sideDistY = (playerY - *mapY) * fabs(1 / rayDirY);
	}
	else
	{
		*stepY = 1;
		*sideDistY = (*mapY + 1.0 - playerY) * fabs(1 / rayDirY);
	}
}

/**
 * perform_DDA - Performs the Digital Differential Analyzer (DDA) algorithm
 * to find the next wall hit by the ray.
 *
 * @stepX: The step in the x-direction.
 * @stepY: The step in the y-direction.
 * @sideDistX: Pointer to the distance to the next x-side.
 * @sideDistY: Pointer to the distance to the next y-side.
 * @mapX: Pointer to the current x-coordinate in the map.
 * @mapY: Pointer to the current y-coordinate in the map.
 * @hit: Pointer to an integer indicating if a wall has been hit.
 * @side: Pointer to an integer indicating which side was
 *		hit (0 for x, 1 for y).
 * @rayAngle: The angle of the ray being cast.
 */
void perform_DDA(int stepX, int stepY, float *sideDistX,
	float *sideDistY, int *mapX, int *mapY, int *hit,
	int *side, float rayAngle)
{
	float rayDirX = cos(rayAngle); /* Calculate ray direction in X */
	float rayDirY = sin(rayAngle); /* Calculate ray direction in Y */

	while (*hit == 0)
	{
		if (*sideDistX < *sideDistY)
		{
			*sideDistX += fabs(1 / rayDirX);
			*mapX += stepX;
			*side = 0;
		}
		else
		{
			*sideDistY += fabs(1 / rayDirY);
			*mapY += stepY;
			*side = 1;
		}

		if (maze_map[*mapX][*mapY] == 1)
			*hit = 1;
	}
}

/**
 * cast_ray - Casts a ray from the player's position to determine
 * the distance to the nearest wall.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 *
 * Return: The perpendicular distance from the player to the wall
 * hit by the ray.
 */
float cast_ray(float playerX, float playerY, float rayAngle)
{
	float rayDirX = cos(rayAngle);
	float rayDirY = sin(rayAngle);
	int mapX = (int)playerX;
	int mapY = (int)playerY;
	float sideDistX, sideDistY;
	int stepX, stepY, hit = 0, side;
	float perpWallDist;

	calculate_step_and_side_dist(rayDirX, rayDirY, playerX,
		playerY, &mapX, &mapY, &stepX, &stepY,
		&sideDistX, &sideDistY);

	perform_DDA(stepX, stepY, &sideDistX, &sideDistY,
		&mapX, &mapY, &hit, &side, rayAngle);

	if (side == 0)
		perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
	else
		perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;

	return (perpWallDist);
}

/**
 * get_wall_hit_coordinates - Determines the coordinates of the wall hit
 * by a ray from the player's position.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 * @mapX: A pointer to an integer set to the X map coordinate of the wall hit.
 * @mapY: A pointer to an integer set to the Y map coordinate of the wall hit.
 *
 * Return: The fractional X-coordinate of the wall hit relative to its width.
 */
float get_wall_hit_coordinates(float playerX,
	float playerY, float rayAngle, int *mapX, int *mapY)
{
	float rayDirX = cos(rayAngle);
	float rayDirY = sin(rayAngle);
	float sideDistX, sideDistY, wallX;
	int stepX, stepY;
	int hit = 0;
	int side;

	*mapX = (int)playerX;
	*mapY = (int)playerY;
	calculate_step_and_side_dist(rayDirX, rayDirY, playerX,
		playerY, mapX, mapY, &stepX, &stepY,
		&sideDistX, &sideDistY);

	perform_DDA(stepX, stepY, &sideDistX, &sideDistY, mapX,
		mapY, &hit, &side, rayAngle);

	if (side == 0)
	{
		wallX = playerY + ((sideDistX - fabs(1 / rayDirX)) * rayDirY);
	}
	else
	{
		wallX = playerX + ((sideDistY - fabs(1 / rayDirY)) * rayDirX);
	}
	wallX -= floor(wallX);

	return (wallX);
}

#include "raycasting.h"

/**
 * calculate_wall_dimensions - Calculate the wall dimensions
 * and correct for fisheye effect.
 *
 * @distance: The distance to the wall.
 * @player: Pointer to the Player structure containing player's attributes.
 * @wallHeight: Pointer to store the calculated wall height.
 * @wallTop: Pointer to store the top position of the wall.
 * @wallBottom: Pointer to store the bottom position of the wall.
 */
void calculate_wall_dimensions(int distance, Player *player,
	int *wallHeight, int *wallTop, int *wallBottom)
{
	float correctedDistance = distance * cos(player->angle - player->angle);
	*wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
	*wallTop = (SCREEN_HEIGHT / 2) - (*wallHeight / 2);
	*wallBottom = (SCREEN_HEIGHT / 2) + (*wallHeight / 2);

	/* Ensure values are within screen bounds */
	if (*wallTop < 0)
		*wallTop = 0;
	if (*wallBottom >= SCREEN_HEIGHT)
		*wallBottom = SCREEN_HEIGHT - 1;
}

/**
 * render_single_wall - Render a single wall slice based on the player's view.
 *
 * @x: The x position on the screen to render the wall slice.
 * @wallHeight: The height of the wall slice.
 * @wallTop: The top position of the wall slice.
 * @wallX: The x coordinate of the wall texture.
 * @wallTextureIndex: The index of the wall texture.
 */
void render_single_wall(int x, int wallHeight, int wallTop,
	float wallX, int wallTextureIndex)
{
	SDL_Rect srcRect, dstRect;
	int texWidth = wallTextures[wallTextureIndex].width;

	// Ensure wallX is within the range [0, 1]
	wallX = fmod(wallX, 1.0f);

	int texX = (int)(wallX * texWidth);
	srcRect.x = texX;
	srcRect.y = 0;
	srcRect.w = 1;
	srcRect.h = wallTextures[wallTextureIndex].height;

	dstRect.x = x;
	dstRect.y = wallTop;
	dstRect.w = 1;
	dstRect.h = wallHeight;

	// Render wall slice
	SDL_RenderCopy(renderer, wallTextures[wallTextureIndex].texture,
		&srcRect, &dstRect);
}

/**
 * render_walls - Render the walls of the maze based on the player's position.
 *
 * This function calculates the wall dimensions and renders them on the screen.
 *
 * @player: Pointer to the Player structure containing player's attributes.
 */
void render_walls(Player *player)
{
    int x, mapX, mapY, wallHeight, wallTop, wallBottom;
    int wallTextureIndex;
    float rayAngle, distance, wallX;

    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        distance = cast_ray(player->x, player->y, rayAngle);

        if (distance > 10.0)
            distance = 10.0;

        calculate_wall_dimensions(distance, player, &wallHeight, &wallTop, &wallBottom);
        wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY);
        wallTextureIndex = maze_map[mapX][mapY] - 1;

        render_single_wall(x, wallHeight, wallTop, wallX, wallTextureIndex);
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
