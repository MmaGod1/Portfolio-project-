#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player {
    float posX, posY;    // Player's position in the world
    float dirX, dirY;    // Direction vector
    float planeX, planeY; // Camera plane for FOV
} Player;

#endif
