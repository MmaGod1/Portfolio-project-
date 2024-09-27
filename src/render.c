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
    // Corrected distance calculation
    float correctedDistance = distance * cos(player->angle);
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
    int texWidth = gameStats->wallTextures[wallTextureIndex].width;  
    int texHeight = gameStats->wallTextures[wallTextureIndex].height; 

    texX = (int)(wallX * texWidth) % texWidth;

    srcRect.x = texX;          // X coordinate on texture
    srcRect.y = 0;             // Start from the top of the texture
    srcRect.w = 1;             // Width of the slice (1 pixel)
    srcRect.h = texHeight;     // Full height of the texture

    dstRect.x = x;             // X position on screen
    dstRect.y = wallTop;       // Top position for the wall slice
    dstRect.w = 1;             // Width of 1 pixel
    dstRect.h = wallHeight;    // Calculated wall height

    SDL_RenderCopy(gameStats->renderer, gameStats->wallTextures[wallTextureIndex].texture, &srcRect, &dstRect);
}

void render_walls(GameStats *gameStats, Player *player)
{
    int x, wallHeight, wallTop, wallBottom;
    int wallTextureIndex;
    float rayAngle, distance, wallX;

    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);

        // Normalize ray angle
        if (rayAngle < 0)
            rayAngle += 2 * M_PI;
        if (rayAngle > 2 * M_PI)
            rayAngle -= 2 * M_PI;

        distance = cast_ray(player->x, player->y, rayAngle);
        if (distance > 10.0)
            distance = 10.0;

        calculate_wall_dimensions(distance, player, &wallHeight, &wallTop, &wallBottom);

        wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY);
        wallTextureIndex = maze_map[mapX][mapY] - 1;

        render_single_wall(gameStats, x, wallHeight, wallTop, wallX, wallTextureIndex);
    }
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
        
        /* Normalize the ray angle to stay between 0 and 2 * PI */
        if (rayAngle < 0)
            rayAngle += 2 * M_PI;
        if (rayAngle > 2 * M_PI)
            rayAngle -= 2 * M_PI;

        distance = cast_ray(player->x, player->y, rayAngle);

        if (distance > 10.0)
            distance = 10.0;

        calculate_wall_dimensions(distance, player, &wallHeight, &wallTop, &wallBottom);

        wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY);
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
 * @player: points to the Player structure - the player's position and angle.
 * @showMap: boolean indicating whether to display the mini-map.
 * @renderer: Pointer to the SDL_Renderer used for rendering.
 * @gameStats: Pointer to the GameStats structure containing texture information.
 */
void render(Player *player, bool showMap, SDL_Renderer *renderer, GameStats *gameStats)
{
    SDL_RenderClear(renderer);
    draw_sky(renderer);
    draw_floor(renderer, player, gameStats->floorTexture);
    render_walls(gameStats, player);

    if (showMap)
    {
        draw_mini_map(renderer, player, showMap);
    }

    SDL_RenderPresent(renderer);
}
