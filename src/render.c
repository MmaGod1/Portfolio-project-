#include "raycasting.h"
#include <math.h>

/**
 * normalize_angle - Normalize an angle to be within the range [0, 2 * PI).
 *
 * @angle: The angle in radians to be normalized.
 * @return: The normalized angle.
 */
float normalize_angle(float angle)
{
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
}

/**
 * calculate_wall_dimensions - Calculate the wall dimensions
 * and correct for fisheye effect.
 *
 * @distance: The distance to the wall (float for precision).
 * @player: Pointer to the Player structure containing player's attributes.
 * @rayAngle: The angle of the ray being cast.
 * @wallHeight: Pointer to store the calculated wall height.
 * @wallTop: Pointer to store the top position of the wall.
 * @wallBottom: Pointer to store the bottom position of the wall.
 */
void calculate_wall_dimensions(float distance, Player *player, float rayAngle, int *wallHeight, int *wallTop, int *wallBottom)
{
    // Normalize ray angle
    rayAngle = normalize_angle(rayAngle);

    // Correct the distance to avoid fisheye effect
    float correctedDistance = distance * cos(rayAngle - player->angle);

    // Avoid division by zero or extremely small distances
    if (correctedDistance < 0.1f) {
        correctedDistance = 0.1f;
    }

    // Calculate the wall height based on the corrected distance
    *wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
    *wallTop = (SCREEN_HEIGHT / 2) - (*wallHeight / 2);
    *wallBottom = (SCREEN_HEIGHT / 2) + (*wallHeight / 2);

    // Ensure values are within screen bounds
    if (*wallTop < 0) *wallTop = 0;
    if (*wallBottom >= SCREEN_HEIGHT) *wallBottom = SCREEN_HEIGHT - 1;
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
 */
void render_single_wall(GameStats *gameStats, int x, int wallHeight, int wallTop, float wallX, int wallTextureIndex)
{
    SDL_Rect srcRect, dstRect;
    int texWidth, texHeight, texX;

    texWidth = gameStats->wallTextures[wallTextureIndex].width;  
    texHeight = gameStats->wallTextures[wallTextureIndex].height; 

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
 * @gameStats: Pointer to the GameStats structure containing game data.
 * @player: Pointer to the Player structure containing player's attributes.
 */
void render_walls(GameStats *gameStats, Player *player)
{
    int x, mapX, mapY, wallHeight, wallTop, wallBottom;
    float rayAngle, distance, wallX;
    int wallTextureIndex;

    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        rayAngle = normalize_angle(player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH));

        // Cast the ray to get the distance to the wall
        distance = cast_ray(player->x, player->y, rayAngle);
        printf("Ray angle: %f, Distance: %f\n", rayAngle, distance);  // Debug output

        calculate_wall_dimensions(distance, player, rayAngle, &wallHeight, &wallTop, &wallBottom);

        wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY);
        wallTextureIndex = maze_map[mapX][mapY] - 1;
        
        if (wallTextureIndex < 0) {
            wallTextureIndex = 0; // or handle error
        }

        printf("MapX: %d, MapY: %d, WallHeight: %d, WallTop: %d, WallBottom: %d\n",
               mapX, mapY, wallHeight, wallTop, wallBottom);  // Debug output

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
