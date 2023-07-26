#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h> // Se agrega esta cabecera para utilizar memcpy()
#include <time.h>

// Define the size of the framebuffer
const int FRAMEBUFFER_WIDTH = 100;
const int FRAMEBUFFER_HEIGHT = 100;
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// Define a Color struct to hold the RGB values of a pixel
typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

// Declare a global clearColor of type Color
Color clearColor = {0, 0, 0, 255}; // Initially set to black

// Declare a global currentColor of type Color
Color currentColor = {255, 255, 255, 255}; // Initially set to white

// Function to clear the framebuffer with the clearColor
void clear(Color* framebuffer, int size) {
    for (int i = 0; i < size; i++) {
        framebuffer[i] = clearColor;
    }
}

// Function to set a specific pixel in the framebuffer to the currentColor
void point(Color* framebuffer, int x, int y, int width, int height) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        framebuffer[y * width + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer, Color* framebuffer, int width, int height) {
    // Create a texture
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, 
        height
    );

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
        texture, 
        NULL, 
        framebuffer, 
        width * sizeof(Color)
    );

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

// Function to update the game state based on Conway's rules
void updateGame(Color* currentFrame, Color* nextFrame, int width, int height) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int liveNeighbors = 0;

            // Count live neighbors
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue; // Skip the current cell
                    int nx = x + dx;
                    int ny = y + dy;

                    // Wrap around the edges
                    if (nx < 0) nx = width - 1;
                    if (nx >= width) nx = 0;
                    if (ny < 0) ny = height - 1;
                    if (ny >= height) ny = 0;

                    if (currentFrame[ny * width + nx].r == 255) {
                        liveNeighbors++;
                    }
                }
            }

            // Apply Conway's rules
            if (currentFrame[y * width + x].r == 255) {
                // Cell is alive
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextFrame[y * width + x].r = 0; // Cell dies
                } else {
                    nextFrame[y * width + x].r = 255; // Cell survives
                }
            } else {
                // Cell is dead
                if (liveNeighbors == 3) {
                    nextFrame[y * width + x].r = 255; // Cell becomes alive
                } else {
                    nextFrame[y * width + x].r = 0; // Cell remains dead
                }
            }
        }
    }
}

// Function to place a glider at a specific position in the framebuffer
void drawGlider(Color* framebuffer, int x, int y, int width, int height) {
    point(framebuffer, x + 2, y + 1, width, height);
    point(framebuffer, x, y + 2, width, height);
    point(framebuffer, x + 1, y + 2, width, height);
    point(framebuffer, x + 2, y + 2, width, height);
}

// Function to fill the framebuffer with gliders at random positions
void fillWithRandomGliders(Color* framebuffer, int width, int height, int numGliders) {
    // Initialize random seed
    srand(time(0));

    for (int i = 0; i < numGliders; ++i) {
        // Generate random coordinates within the framebuffer
        int x = rand() % (width - 3);
        int y = rand() % (height - 3);

        // Draw the glider at the random position
        drawGlider(framebuffer, x, y, width, height);
    }
}

void drawLoaf(Color* framebuffer, int x, int y, int width, int height) {
    point(framebuffer, x + 1, y, width, height);
    point(framebuffer, x + 2, y + 1, width, height);
    point(framebuffer, x, y + 1, width, height);
    point(framebuffer, x + 3, y + 2, width, height);
    point(framebuffer, x + 1, y + 3, width, height);
    point(framebuffer, x + 3, y + 3, width, height);
    point(framebuffer, x + 2, y + 4, width, height);
    
}

void drawPulsarPeriod3(Color* framebuffer, int x, int y, int width, int height) {
    int p[24][2] = {{-2, 1}, {-2, 2}, {-2, 3}, {-1, 3}, {0, 3}, {1, 3}, {2, 3}, {2, 2}, {2, 1}, {1, -1}, {1, -2}, {1, -3},
                    {-2, -1}, {-2, -2}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {2, -2}, {2, -1}, {-1, 1}, {0, 1}, {1, 1}};
    for (int i = 0; i < 24; i++) {
        point(framebuffer, x + p[i][0], y + p[i][1], width, height);
    }
}

void drawBlock(Color* framebuffer, int x, int y, int width, int height) {
    point(framebuffer, x, y, width, height);
    point(framebuffer, x + 1, y, width, height);
    point(framebuffer, x, y + 1, width, height);
    point(framebuffer, x + 1, y + 1, width, height);
}

void drawTub(Color* framebuffer, int x, int y, int width, int height) {
    point(framebuffer, x + 1, y, width, height);
    point(framebuffer, x, y + 1, width, height);
    point(framebuffer, x + 2, y + 1, width, height);
    point(framebuffer, x + 1, y + 2, width, height);
}

void generateRandomFigures(Color* framebuffer, int numFigures, int figureWidth, int figureHeight, void (*drawFunction)(Color*, int, int, int, int)) {
    for (int i = 0; i < numFigures; i++) {
        int x = rand() % (FRAMEBUFFER_WIDTH - figureWidth);
        int y = rand() % (FRAMEBUFFER_HEIGHT - figureHeight);
        drawFunction(framebuffer, x, y, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    }
}


int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Conway's Game of Life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    // Declare the framebuffer as a local variable in main
    Color framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

    bool running = true;
    SDL_Event event;

    // Create an auxiliary framebuffer for updating the game state
    Color nextFramebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];

    // Fill the framebuffer with 5 gliders at random positions
    fillWithRandomGliders(framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 200);

    for (int i = 0; i < 100; i++) {
    int x = rand() % (FRAMEBUFFER_WIDTH - 4);
    int y = rand() % (FRAMEBUFFER_HEIGHT - 5);
    drawLoaf(framebuffer, x, y, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
}

    for (int i = 0; i < 100; i++) {
    int x = rand() % (FRAMEBUFFER_WIDTH - 10);
    int y = rand() % (FRAMEBUFFER_HEIGHT - 11);
    drawPulsarPeriod3(framebuffer, x, y, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
}
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update the game state
        updateGame(framebuffer, nextFramebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

        // Copy the contents of nextFramebuffer to framebuffer
        memcpy(framebuffer, nextFramebuffer, sizeof(framebuffer));

        // Render the game
        renderBuffer(renderer, framebuffer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}