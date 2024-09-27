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
 * @gameStats: Pointer to the GameStats structure containing game data, 
 *             including textures and the renderer.
 * @x: The x position on the screen to render the wall slice.
 * @wallHeight: The height of the wall slice to be rendered.
 * @wallTop: The top position on the screen for the wall slice.
 * @wallX: The x coordinate on the texture corresponding to the wall slice.
 * @wallTextureIndex: The index of the wall texture to use for rendering.
 *
 * This function calculates the appropriate texture coordinates and 
 * dimensions for rendering a wall slice based on the player's perspective.
 * It then uses SDL_RenderCopy to draw the slice on the screen.
 */
void render_single_wall(GameStats *gameStats, int x, int wallHeight, int wallTop, float wallX, int wallTextureIndex)
{
    SDL_Rect srcRect, dstRect;
    int texWidth, texHeight, texX;

    texWidth = gameStats->wallTextures[wallTextureIndex].width;  
    texHeight = gameStats->wallTextures[wallTextureIndex].height; 

    SDL_QueryTexture(gameStats->wallTextures[wallTextureIndex].texture, NULL, NULL, &texWidth, &texHeight);

    texX = (int)(wallX * texWidth) % texWidth;

    srcRect.x = texX;
    srcRect.y = 0;
    srcRect.w = 1;
    srcRect.h = texHeight;

    dstRect.x = x;
    dstRect.y = wallTop;
    dstRect.w = 1;
    dstRect.h = wallHeight;

    SDL_RenderCopy(gameStats->renderer, gameStats->wallTextures[wallTextureIndex].texture, &srcRect, &dstRect);
}

/**
 * render_walls - Render the walls of the maze based on the player's position.
 *
 * This function calculates the wall dimensions and renders them on the screen.
 *
 * @player: Pointer to the Player structure containing player's attributes.
 */
void render_walls(GameStats *gameStats, Player *player)
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

        calculate_wall_dimensions(distance, player,
                &wallHeight, &wallTop, &wallBottom);
        wallX = get_wall_hit_coordinates(player->x,
                player->y, rayAngle, &mapX, &mapY);
        wallTextureIndex = maze_map[mapX][mapY] - 1;

        render_single_wall(gameStats, x, wallHeight, wallTop, wallX, wallTextureIndex);
    }
}


/**
 * render - Clears the renderer and draws the current frame of the game,
 *          including the sky, floor, and walls.
 *
 * This function handles the rendering process, including clearing the screen,
 * drawing the sky and floor, casting rays to render the walls, and drawing
 * a mini-map of the maze.
 *
 * @player: Pointer to the Player structure, which contains the player's
 *          position and angle.
 * @showMap: Boolean indicating whether to display the mini-map.
 * @renderer: Pointer to the SDL_Renderer used for rendering.
 * @gameStats: Pointer to the GameStats structure containing textures and
 *             renderer information.
 *
 * Return: void
 */
void render(Player *player, bool showMap, SDL_Renderer *renderer, GameStats *gameStats)
{
    SDL_RenderClear(renderer);

    draw_sky(renderer);  
    draw_floor(renderer, player, gameStats->floorTexture.texture);
    /* Cast rays and render walls */
    render_walls(gameStats, player); // Pass gameStats to render_walls

    if (showMap)
    {
        draw_mini_map(renderer, player, showMap);
    }

    SDL_RenderPresent(renderer);
}
