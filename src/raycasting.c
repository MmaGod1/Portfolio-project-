#include "raycasting.h"

/**
 * cast_ray - Casts a ray from the player's position to determine
 *            the distance to the nearest wall.
 *
 * This function uses the DDA (Digital Differential Analyzer) algorithm
 * to traverse the map grid based on the player's angle and position.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 *
 * Return: The perpendicular distance from the player to the wall hit by the ray.
 */
float cast_ray(float playerX, float playerY, float rayAngle)
{
	/* Ray direction */
	float rayDirX = cos(rayAngle);
	float rayDirY = sin(rayAngle);

	/* Which box of the map we're in */
	int mapX = (int)playerX;
	int mapY = (int)playerY;

	/* Length of ray from one x or y side to next x or y side */
	float deltaDistX = fabs(1 / rayDirX);
	float deltaDistY = fabs(1 / rayDirY);

	/* What direction to step in (+1 or -1) */
	int stepX, stepY;
	float sideDistX, sideDistY;

	/* Calculate step and initial sideDist */
	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (playerX - mapX) * deltaDistX;
	} else
	{
		stepX = 1;
		sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
	}
	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (playerY - mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
	}

	/* Perform DDA */
	bool hit = false;
	int side;

	while (!hit)
	{
        /* Jump to next map square, either in x-direction or y-direction */
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			mapX += stepX;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			mapY += stepY;
			side = 1;
		}

        /* Check if ray has hit a wall */
		if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && maze_map[mapX][mapY] == 1)
			hit = true;
	}

	/* Calculate the distance to the hit */
	float perpWallDist;
	if (side == 0)
		perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
	else
		perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;

	return (perpWallDist);
}



/**
 * get_wall_hit_coordinates - Determines the coordinates of the wall hit
 *                            by a ray from the player's position.
 *
 * This function also implements the DDA algorithm to find the exact position
 * where the ray intersects with a wall, returning the wall's X-coordinate
 * as a fraction of the wall's width.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 * @mapX: A pointer to an integer that will be set to the X map coordinate of the wall hit.
 * @mapY: A pointer to an integer that will be set to the Y map coordinate of the wall hit.
 *
 * Return: The fractional X-coordinate of the wall hit relative to its width.
 */
float get_wall_hit_coordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY)
{
	/* DDA algorithm for raycasting */
	float rayDirX = cos(rayAngle);
	float rayDirY = sin(rayAngle);

	/* Map grid positions of the player */
	*mapX = (int)playerX;
	*mapY = (int)playerY;

	/* Length of ray to the next side in X and Y */
	float sideDistX, sideDistY;

	/* Length of the ray to the next X or Y side (constant) */
	float deltaDistX = fabs(1 / rayDirX);
	float deltaDistY = fabs(1 / rayDirY);
	int stepX, stepY;
	int hit = 0;
	int side;

	/* Calculate step and initial sideDist */
	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (playerX - *mapX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
	}
	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (playerY - *mapY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
	}

	/* Perform DDA */
	while (hit == 0)
	{
        /* Jump to next map square, either in X-direction or Y-direction */
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			*mapX += stepX;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			*mapY += stepY;
			side = 1;
		}

		/* Check if the ray has hit a wall */
		if (maze_map[*mapX][*mapY] > 0) hit = 1;
	}

	/* Calculate exact position of where the wall was hit */
	float wallX;
	if (side == 0)
	{
		wallX = playerY + ((sideDistX - deltaDistX) * rayDirY);
	}
	else
	{
		wallX = playerX + ((sideDistY - deltaDistY) * rayDirX);
	}
	wallX -= floor(wallX);

	/* Return the hit coordinate on the wall */
	return (wallX);
}
