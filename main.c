int main(int argc, char* argv[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }

    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0;   //initial direction vector
    double planeX = 0, planeY = 0.66; //camera plane (FOV)

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Raycasting loop
        for (int x = 0; x < screenWidth; x++) {
            // Calculate ray position and direction
            double cameraX = 2 * x / (double)screenWidth - 1; // X coordinate in camera space
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            // Additional raycasting logic here (e.g., detecting walls, distances, etc.)

            // Render each vertical stripe of the screen here
            // Example: SDL_RenderDrawLine(renderer, x, startY, x, endY); 
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Present the final image
        SDL_RenderPresent(renderer);
    }

    close();
    return 0;
}
