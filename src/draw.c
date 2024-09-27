#include "raycasting.h"

/**
 * draw_sky - Renders the sky in the game window.
 * @renderer: The SDL_Renderer used to render the sky.
 *
 * This function sets the draw color to a sky blue and fills
 * the top half of the screen with a rectangle representing the sky.
 *
 * Return: void
 */
void draw_sky(SDL_Renderer *renderer)
{
	SDL_Rect skyRect;

	skyRect.x = 0;
	skyRect.y = 0;
	skyRect.w = SCREEN_WIDTH;
	skyRect.h = SCREEN_HEIGHT / 2;

	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
	SDL_RenderFillRect(renderer, &skyRect);
}

/**
 * draw_floor - Renders the floor based on the player's position and angle.
 * @renderer: The SDL_Renderer used to render the floor.
 * @player: Pointer to Player structure with position and angle info.
 *
 * Loops through the lower half of the screen, calculating floor texture
 * coordinates and rendering them using SDL_RenderCopy.
 */
void draw_floor(SDL_Renderer *renderer, Player *player)
{
	int y, x, texX, texY;
	float rayAngle, floorX, floorY, floorDist;
	SDL_Rect srcRect, dstRect;

	for (y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
	{
		floorDist = SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT);

		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
			floorX = player->x + cos(rayAngle) * floorDist;
			floorY = player->y + sin(rayAngle) * floorDist;
			texX = (int)(floorX * floorTexture.width) % floorTexture.width;
			texY = (int)(floorY * floorTexture.height) % floorTexture.height;

			if (texX < 0)
				texX = 0;
			if (texY < 0)
				texY = 0;

			srcRect.x = texX;
			srcRect.y = texY;
			srcRect.w = 1;
			srcRect.h = 1;

			dstRect.x = x;
			dstRect.y = y;
			dstRect.w = 1;
			dstRect.h = 1;

			SDL_RenderCopy(renderer, floorTexture.texture, &srcRect, &dstRect);
		}
	}
}


/**
 * draw_map_tiles - Draws the tiles of the mini-map based on the maze map.
 * @renderer: The SDL_Renderer used to render the map tiles.
 * @mapStartX: The starting x-coordinate for the map on the screen.
 * @mapStartY: The starting y-coordinate for the map on the screen.
 * @tileSize: The size of each tile on the mini-map.
 *
 * This function iterates through the maze map and renders each tile
 * on the mini-map. Walls are drawn in red, and empty spaces are
 * drawn in white.
 */
void draw_map_tiles(SDL_Renderer *renderer, int mapStartX, int mapStartY, int tileSize)
{
	int y, x;
	SDL_Rect rect;

	for (y = 0; y < MAP_HEIGHT; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			rect.x = mapStartX + x * tileSize;
			rect.y = mapStartY + y * tileSize;
			rect.w = tileSize;
			rect.h = tileSize;

			if (maze_map[x][y] == 1)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}

			SDL_RenderFillRect(renderer, &rect);
		}
	}
}


/**
 * draw_mini_map - Draws the mini-map, the player, and their line of sight.
 * @renderer: The SDL_Renderer used to render the mini-map.
 * @player: Pointer to the player's structure containing position and angle.
 * @showMap: Boolean indicating whether to show the map or not.
 *
 * This function renders the mini-map in the top-left corner of the screen,
 * showing the player's position in green and their line of sight in blue.
 * The mini-map is drawn only if @showMap is true. It calls the helper function
 * draw_map_tiles to render the map's tiles.
 */
void draw_mini_map(SDL_Renderer *renderer, Player *player, bool showMap)
{
	int mapStartX, mapStartY, mapWidth, mapHeight, tileSize;
	SDL_Rect playerRect;
	float mapPlayerX, mapPlayerY, rayAngle, endX, endY;
	int i;

	if (!showMap)
		return;

	mapStartX = 10;
	mapStartY = 10;
	mapWidth = 160;
	mapHeight = 120;
	tileSize = TILE_SIZE;

	draw_map_tiles(renderer, mapStartX, mapStartY, tileSize);

	mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
	mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	playerRect.x = (int)mapPlayerX - 2;
	playerRect.y = (int)mapPlayerY - 2;
	playerRect.w = 4;
	playerRect.h = 4;
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
		endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
		endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
		SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
	}
}
