#include "raycasting.h"

/**
 * draw_map_tiles - Draws the tiles of the mini-map based on the maze map.
 * @mapStartX: The starting x-coordinate for the map on the screen.
 * @mapStartY: The starting y-coordinate for the map on the screen.
 * @tileSize: The size of each tile on the mini-map.
 *
 * This function iterates through the maze map and renders each tile
 * on the mini-map. Walls are drawn in red, and empty spaces are
 * drawn in white.
 */
void draw_map_tiles(int mapStartX, int mapStartY, int tileSize)
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

			/* Set the draw color based on the maze map */
			if (maze_map[x][y] == 1)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}

			/* Render the tile */
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

/**
 * draw_mini_map - Draws the mini-map, including the player and their line of sight.
 * @player: Pointer to the player's structure containing position and angle.
 * @showMap: Boolean indicating whether to show the map or not.
 *
 * This function renders the mini-map in the top-left corner of the screen,
 * showing the player's position in green and their line of sight in blue.
 * The mini-map is drawn only if @showMap is true. It calls the helper function
 * draw_map_tiles to render the map's tiles.
 */
void draw_mini_map(Player *player, bool showMap)
{
	int mapStartX, mapStartY, mapWidth, mapHeight, tileSize;
	SDL_Rect playerRect;
	float mapPlayerX, mapPlayerY, rayAngle, endX, endY;
	int i;

	if (!showMap)
		return;

	/* Position the map in the top-left corner */
	mapStartX = 10;
	mapStartY = 10;
	mapWidth = 160;
	mapHeight = 120;
	tileSize = TILE_SIZE;

	/* Draw the map tiles */
	draw_map_tiles(mapStartX, mapStartY, tileSize);

	/* Draw the player's position on the map */
	mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
	mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	playerRect.x = (int)mapPlayerX - 2;
	playerRect.y = (int)mapPlayerY - 2;
	playerRect.w = 4;
	playerRect.h = 4;
	SDL_RenderFillRect(renderer, &playerRect);

	/* Draw the player's line of sight */
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
		endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
		endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
		SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
	}
}
