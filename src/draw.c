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
	SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
	skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
	SDL_RenderFillRect(renderer, &skyRect);
}


void draw_floor(Player *player)
{
	int y, x, texX, texY;
	float rayAngle, floorX, floorY;
	SDL_Rect srcRect, dstRect;

	for (y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
	{
		float floorDist = SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT);

		for (x = 0; x < SCREEN_WIDTH; x++)
		{
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

			/* Render the floor pixel */
			srcRect = { texX, texY, 1, 1 };
			dstRect = { x, y, 1, 1 };
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
	int mapStartX, mapStartY, mapWidth, mapHeight, tileSize;
	int y, x, i;
	float mapPlayerX, mapPlayerY, rayAngle, endX, endY;
	
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
			SDL_Rect rect;
			
			/* Position of the tile */
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


	/* Draw the player's position on the map */
	mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
	mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_Rect playerRect = { (int)mapPlayerX - 2, (int)mapPlayerY - 2, 4, 4 };
	SDL_RenderFillRect(renderer, &playerRect);

	/* Draw the player's line of sight on the map */
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);

		/* Adjust length for rays (shorter) */
		endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
		endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
		SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
	}
}
