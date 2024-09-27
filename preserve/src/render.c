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
    // Apply fisheye correction by using the difference between rayAngle and player angle
    float correctedDistance = distance * cos(player->angle - rayAngle);
    
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

	int texX = (int)(wallX * texWidth) % texWidth;
	/*Texture wallTextures[4];*/
	srcRect.x = texX;
	srcRect.y = 0;
	srcRect.w = 1;
	srcRect.h = wallTextures[wallTextureIndex].height;

	dstRect.x = x;
	dstRect.y = wallTop;
	dstRect.w = 1;
	dstRect.h = wallHeight;

	/* Render wall slice */
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
    float correctedDistance;

    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        /* Calculate the current ray angle based on FOV */
        rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);

        /* Normalize the ray angle to stay between 0 and 2 * PI */
        if (rayAngle < 0)
            rayAngle += 2 * M_PI;
        if (rayAngle > 2 * M_PI)
            rayAngle -= 2 * M_PI;

        /* Cast the ray to get the distance to the wall */
        distance = cast_ray(player->x, player->y, rayAngle);

        /* Cap the maximum distance to avoid overly large wall heights */
        if (distance > 10.0)
            distance = 10.0;

        /* Correct for fisheye effect by adjusting the distance */
        correctedDistance = distance * cos(player->angle - rayAngle);

        /* Calculate wall dimensions based on the corrected distance */
        calculate_wall_dimensions(correctedDistance, player, &wallHeight, &wallTop, &wallBottom);

        /* Get the X coordinate of the wall texture to be used */
        wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY);

        /* Determine which wall texture to use based on the map location */
        wallTextureIndex = maze_map[mapX][mapY] - 1;  /* Assuming map value corresponds to texture index */

        /* Render a single vertical slice of the wall */
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
