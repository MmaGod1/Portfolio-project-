#include "raycasting.h"
#include "map.h"
#include "camera.h"
#include <math.h>

void cast_ray(float camera_x, float camera_y, float camera_angle) {
    // Calculate the ray direction
    float ray_dir_x = cos(camera_angle * M_PI / 180.0f);
    float ray_dir_y = sin(camera_angle * M_PI / 180.0f);

    // Cast rays for each column on the screen
    for (int x = 0; x < 640; x++) {
        // Calculate the ray origin
        float ray_origin_x = camera_x;
        float ray_origin_y = camera_y;

        // Calculate the distance to the wall
        float distance = 0.0f;
        int hit_wall = 0;
        while (!hit_wall && distance < 100.0f) {
            // Check if the ray hits a wall
            int map_x = (int)ray_origin_x;
            int map_y = (int)ray_origin_y;
            if (map_x >= 0 && map_x < MAP_WIDTH && map_y >= 0 && map_y < MAP_HEIGHT && map[map_x][map_y] == 1) {
                hit_wall = 1;
            } else {
                // Move the ray forward
                ray_origin_x += ray_dir_x;
                ray_origin_y += ray_dir_y;
                distance += 0.1f;
            }
        }

        // Calculate the wall height
        float wall_height = 480.0f / distance;

        // Draw the wall
        SDL_Rect wall_rect;
        wall_rect.x = x;
        wall_rect.y = 240 - wall_height / 2;
        wall_rect.w = 1;
        wall_rect.h = wall_height;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &wall_rect);
    }
}
