#include "raycasting.h"

/**
 * draw_sky - Renders the sky in the game window.
 *
 * This function sets the draw color to a night blue and fills
 * the top half of the screen with a rectangle representing the sky.
 *
 * Return: void
 */
void draw_sky(void)
{
	SDL_Rect skyRect;
	skyRect.x = 0;
	skyRect.y = 0;
	skyRect.w = SCREEN_WIDTH;
	skyRect.h = SCREEN_HEIGHT / 2;

	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
	SDL_RenderFillRect(renderer, &skyRect);
}


void draw_floor(Player *player)
{
    // Declare all variables at the top
    int y, x, texX, texY;
    float rayAngle, floorX, floorY;
    float floorDist; // Declare floorDist here
    SDL_Rect srcRect; // Declare srcRect at the top
    SDL_Rect dstRect; // Declare dstRect at the top

    // Loop over the lower half of the screen
    for (y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
    {
        // Calculate the distance to the floor
        floorDist = SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT);

        // Loop over the screen width
        for (x = 0; x < SCREEN_WIDTH; x++)
        {
            // Calculate the ray angle for the current column
            rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
    
            /* Compute floor X and Y coordinates */
            floorX = player->x + cos(rayAngle) * floorDist;
            floorY = player->y + sin(rayAngle) * floorDist;

            /* Determine the texture coordinates */
            texX = (int)(floorX * floorTexture.width) % floorTexture.width;
            texY = (int)(floorY * floorTexture.height) % floorTexture.height;

            /* Ensure texture coordinates are valid */
            if (texX < 0) texX = 0;
            if (texY < 0) texY = 0;

            /* Set the values for the srcRect structure */
            srcRect.x = texX;
            srcRect.y = texY;
            srcRect.w = 1;
            srcRect.h = 1;

            /* Set the values for the dstRect structure */
            dstRect.x = x;
            dstRect.y = y;
            dstRect.w = 1;
            dstRect.h = 1;

            /* Render the floor pixel */
            SDL_RenderCopy(renderer, floorTexture.texture, &srcRect, &dstRect);
        }
    }
}

/**
 * draw_mini_map - Renders a mini-map in the game window.
 *
 * This function draws a mini-map showing the maze layout and the player's
 * position. It highlights walls in red, free spaces in white, and marks
 * the player's position in green. Additionally, it visualizes the player's
 * line of sight with blue rays.
 *
 * @player: Pointer to the Player structure containing the player's position and angle.
 * @showMap: Boolean flag indicating whether to display the mini-map.
 *
 * Return: void
 */
void draw_mini_map(Player *player, bool showMap)
{
    // Declare all variables at the top
    int mapStartX, mapStartY, mapWidth, mapHeight, tileSize;
    int y, x, i;
    float mapPlayerX, mapPlayerY, rayAngle, endX, endY;

    // Check if the mini map should be displayed
    if (!showMap) return;

    /* Position the map in the top-left corner */
    mapStartX = 10;
    mapStartY = 10;
    mapWidth = 160;
    mapHeight = 120;
    tileSize = TILE_SIZE;

    /* Draw the map tiles */
    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            SDL_Rect rect; // Declare rect here

            /* Position of the tile */
            rect.x = mapStartX + x * tileSize;
            rect.y = mapStartY + y * tileSize;
            rect.w = tileSize;
            rect.h = tileSize;

            /* Set the draw color based on the maze map */
            if (maze_map[x][y] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for walls
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for empty space
            }

            /* Render the tile */
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    /* Draw the player's position on the map */
    mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
    mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for player position
    SDL_Rect playerRect; // Declare playerRect here
    playerRect.x = (int)mapPlayerX - 2;
    playerRect.y = (int)mapPlayerY - 2;
    playerRect.w = 4;
    playerRect.h = 4;
    
    /* Render the player's position */
    SDL_RenderFillRect(renderer, &playerRect);

    /* Draw the player's line of sight on the map */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for line of sight

    for (i = 0; i < SCREEN_WIDTH; i++)
    {
        rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);

        /* Adjust length for rays (shorter) */
        endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
        endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
        
        /* Render the line of sight */
        SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}
