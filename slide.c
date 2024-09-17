#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glut.h>  // Assuming you're using GLUT for handling input and graphics

// Player attributes
float playerX = 1.5f, playerY = 1.5f;  // Starting position
float playerAngle = 0.0f;  // Camera facing angle
float moveSpeed = 0.05f;   // Movement speed
float rotateSpeed = 0.05f; // Rotation speed

// Maze dimensions
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

int maze_map[MAP_WIDTH][MAP_HEIGHT] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},                       
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Check collision function
bool is_wall(float x, float y) {
    int mapX = (int)x;
    int mapY = (int)y;
    
    // Check if the new position is inside the map and if it's a wall
    if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
        return maze_map[mapX][mapY] == 1;
    }
    return true; // If out of bounds, treat as a wall
}

// Movement function
void handle_movement(char input) {
    // Calculate direction vectors
    float dirX = cos(playerAngle);
    float dirY = sin(playerAngle);
    float perpX = -dirY;  // Perpendicular direction (left)
    float perpY = dirX;   // Perpendicular direction (up)
    
    // Variables for collision detection
    float newX, newY;
    float slideX, slideY;
    bool collisionX = false, collisionY = false;
    
    if (input == 'w') {
        // Move forward
        newX = playerX + dirX * moveSpeed;
        newY = playerY + dirY * moveSpeed;
        
        // Collision check
        if (!is_wall(newX, playerY)) playerX = newX; // Check movement in X direction
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY; // Check movement in Y direction
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            slideX = playerX + perpX * moveSpeed;
            slideY = playerY + perpY * moveSpeed;
            if (!is_wall(slideX, playerY)) playerX = slideX;
            if (!is_wall(playerX, slideY)) playerY = slideY;
        } else if (collisionY && !collisionX) {
            slideX = playerX + dirX * moveSpeed;
            slideY = playerY + dirY * moveSpeed;
            if (!is_wall(playerX, slideY)) playerY = slideY;
            if (!is_wall(slideX, playerY)) playerX = slideX;
        }
    }
    
    if (input == 's') {
        // Move backward
        newX = playerX - dirX * moveSpeed;
        newY = playerY - dirY * moveSpeed;
        
        // Collision check
        if (!is_wall(newX, playerY)) playerX = newX;
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY;
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            slideX = playerX - perpX * moveSpeed;
            slideY = playerY - perpY * moveSpeed;
            if (!is_wall(slideX, playerY)) playerX = slideX;
            if (!is_wall(playerX, slideY)) playerY = slideY;
        } else if (collisionY && !collisionX) {
            slideX = playerX - dirX * moveSpeed;
            slideY = playerY - dirY * moveSpeed;
            if (!is_wall(playerX, slideY)) playerY = slideY;
            if (!is_wall(slideX, playerY)) playerX = slideX;
        }
    }
    
    if (input == 'a') {
        // Strafe left
        newX = playerX + perpX * moveSpeed;
        newY = playerY + perpY * moveSpeed;
        
        // Collision check
        if (!is_wall(newX, playerY)) playerX = newX;
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY;
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            slideX = playerX + dirX * moveSpeed;
            slideY = playerY + dirY * moveSpeed;
            if (!is_wall(slideX, playerY)) playerX = slideX;
            if (!is_wall(playerX, slideY)) playerY = slideY;
        } else if (collisionY && !collisionX) {
            slideX = playerX + perpX * moveSpeed;
            slideY = playerY + perpY * moveSpeed;
            if (!is_wall(playerX, slideY)) playerY = slideY;
            if (!is_wall(slideX, playerY)) playerX = slideX;
        }
    }
    
    if (input == 'd') {
        // Strafe right
        newX = playerX - perpX * moveSpeed;
        newY = playerY - perpY * moveSpeed;
        
        // Collision check
        if (!is_wall(newX, playerY)) playerX = newX;
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY;
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            slideX = playerX - dirX * moveSpeed;
            slideY = playerY - dirY * moveSpeed;
            if (!is_wall(slideX, playerY)) playerX = slideX;
            if (!is_wall(playerX, slideY)) playerY = slideY;
        } else if (collisionY && !collisionX) {
            slideX = playerX - perpX * moveSpeed;
            slideY = playerY - perpY * moveSpeed;
            if (!is_wall(playerX, slideY)) playerY = slideY;
            if (!is_wall(slideX, playerY)) playerX = slideX;
        }
    }
    
    // Quit on 'q'
    if (input == 'q') {
        exit(0);
    }
}

// Example of integrating the movement into your main loop
void display() {
    // Clear screen and set up 3D rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Set camera position and orientation
    glTranslatef(-playerX, -playerY, -2.0f); // Move camera to player position
    glRotatef(-playerAngle * (180.0f / M_PI), 0.0f, 0.0f, 1.0f); // Rotate camera
    
    // Draw the maze here
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    handle_movement(key);
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Raycasting Game");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}
