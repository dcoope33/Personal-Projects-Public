#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <unistd.h>
#include <time.h>

enum Levels {END, LEVEL_1, LEVEL_2, LEVEL_3, LEVEL_4, TUTORIAL};

class Entity; // forward declaration

// Window class
class GameWindow {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 timer = 0; // Tracks how long entity has been in a state
    bool running;

public:
    GameWindow(const char *title, int w, int h) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL Init Failed: " << SDL_GetError() << std::endl;
        }
        IMG_Init(IMG_INIT_PNG);

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderSetLogicalSize(renderer, 800, 600);
        running = true;
    }

    // Destructor: (free())
    ~GameWindow() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool isRunning() { return running; }

    bool handleEvents(std::vector<Entity*>& entities);

    bool handleGameLoss(SDL_Texture *texture, int level_id) { 
        SDL_Event e;
        while(isRunning()) {
            clear();
            SDL_RenderCopy(renderer, texture, 0, 0);
            display();
            while (SDL_PollEvent(&e)) {
                if(e.type == SDL_QUIT) running = false; // close window
                else if(e.type == SDL_MOUSEBUTTONDOWN) {
                        bool inX_play = e.button.x > 120 && e.button.x < 380;
                        bool inY_play = e.button.y > 390 && e.button.y < 455;
                        if(inX_play && inY_play) return true;
                } 
            }
        }
        return END;
    }

    int handleGameWin(SDL_Texture *texture, int level_id) {
        SDL_Event e;
        sleep(1);
        while(isRunning()) {
            clear();
            SDL_RenderCopy(renderer, texture, 0, 0);
            display();
            while (SDL_PollEvent(&e)) {
                if(e.type == SDL_QUIT) running = false; // close window
                else if(e.type == SDL_MOUSEBUTTONDOWN) {
                    if(level_id != LEVEL_4) {
                        bool inX_replay = e.button.x > 120 && e.button.x < 380;
                        bool in_y = e.button.y > 265 && e.button.y < 335;
                        if(inX_replay && in_y) return level_id;
                        
                        bool inX_next = e.button.x > 420 && e.button.x < 695;
                        if(inX_next && in_y) return ++level_id;
                    } else {

                    }
                } 
            }
        }
        return false;
    }


    int Menu(SDL_Texture *texture) {
        SDL_Event e;
        usleep(5000);
        while(isRunning()) {
            clear();
            SDL_RenderCopy(renderer, texture, 0, 0);
            display();
            while (SDL_PollEvent(&e)) {
                if(e.type == SDL_QUIT) running = false; // close window
                else if(e.type == SDL_MOUSEBUTTONDOWN) {
                    bool inX1 = e.button.x > 120 && e.button.x < 375;
                    bool inX2 = e.button.x > 425 && e.button.x < 695;
                    bool inY1_or_2 = e.button.y > 210 && e.button.y < 280;

                    
                    bool inX3 = e.button.x > 120 && e.button.x < 375;
                    bool inX4 = e.button.x > 425 && e.button.x < 695;
                    bool inY3_or_4 = e.button.y > 320 && e.button.y < 385;
                    
                    if(inX1 && inY1_or_2) return LEVEL_1;
                    if(inX2 && inY1_or_2) return LEVEL_2;
                    if(inX3 && inY3_or_4) return LEVEL_3;
                    if(inX4 && inY3_or_4) return LEVEL_4;

                } 
            }
        }
        return END;
    }


    int handleStart(SDL_Texture *texture, SDL_Texture *select) {
        SDL_Event e;
        while(isRunning()) {
            clear();
            SDL_RenderCopy(renderer, texture, 0, 0);
            display();
            while (SDL_PollEvent(&e)) {
                if(e.type == SDL_QUIT) running = false; // close window
                else if(e.type == SDL_MOUSEBUTTONDOWN) {
                    bool inX_play = e.button.x > 180 && e.button.x < 615;
                    bool inY_play = e.button.y > 215 && e.button.y < 315;

                    bool inX_Tut = e.button.x > 230 && e.button.x < 575;
                    bool inY_Tut = e.button.y > 340 && e.button.y < 395;
                    
                    bool inX_Q = e.button.x > 230 && e.button.x < 575;
                    bool inY_Q = e.button.y > 410 && e.button.y < 465;

                    if(inX_play && inY_play) return Menu(select);
                    if(inX_Tut && inY_Tut) return TUTORIAL;
                    if(inX_Q && inY_Q) running = false;
                } 
            }
        }
        return END;
    }

    void clear() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);
    }

    void display() {
        SDL_RenderPresent(renderer);
    }

    SDL_Renderer *getRenderer() { return renderer; }

};


// TextureManager
class TextureManager {
public:
    static SDL_Texture *LoadTexture(const char *fileName, SDL_Renderer *ren) {
        SDL_Surface *tempSurface = IMG_Load(fileName);
        if (tempSurface == NULL) {
            printf("Failed to load image: %s\n", IMG_GetError());
        }
        
        SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);
        SDL_FreeSurface(tempSurface); // free() surface

        if (tex == NULL) {
            printf("SDL_Error: %s\n", SDL_GetError());
        }

        return tex;
    }

    // A helper to draw textures without needing a full Entity instance
    static void Draw(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect src, SDL_Rect dest) {
        SDL_RenderCopy(ren, tex, &src, &dest);
    }

    static SDL_Surface *LoadSurface(const char* filename) {
        SDL_Surface *surface = IMG_Load(filename);
        if (surface == nullptr) {
            std::cout << "Failed to load surface: " << IMG_GetError() << std::endl;
        }
        return surface;
    }
};

// class for creating and using the collision maps
class CollisionMap {
private:
    int width;
    int height;
    std::vector<bool> walkPixels; // 1D vector acting as a 2D array

public:
    // Call ONCE when loading the level
    void loadFromMask(SDL_Surface *rawSurface) {
        if (!rawSurface) return;

        width = rawSurface->w;
        height = rawSurface->h;
        walkPixels.resize(width * height);

        // FORCE the surface into a standard 32-bit RGBA format
        SDL_Surface *optimizedSurface = SDL_ConvertSurfaceFormat(rawSurface, SDL_PIXELFORMAT_RGBA8888, 0);
        
        if (!optimizedSurface) {
            std::cout << "Failed to optimize surface format!" << std::endl;
            return;
        }

        SDL_LockSurface(optimizedSurface);
        Uint32 *pixels = (Uint32*)optimizedSurface->pixels;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Uint32 pixel = pixels[y * width + x];
                Uint8 r, g, b, a;
                
                // This will now unpack perfectly every single time
                SDL_GetRGBA(pixel, optimizedSurface->format, &r, &g, &b, &a);

                // If it is pure black, mark it as solid (true)
                // If it's white, it will register as false (walkable)
                walkPixels[y * width + x] = (r == 0 && g == 0 && b == 0);
            }
        }

        SDL_UnlockSurface(optimizedSurface);
        SDL_FreeSurface(optimizedSurface); // Clean up the temporary optimized surface
    }

    // CallEVERY FRAME
    bool isSolid(int x, int y) {
        // Safety bounds check so game doesn't crash if someone walks off-screen
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return true; 
        }
        // Ultra-fast O(1) boolean lookup
        return walkPixels[y * width + x];
    }
};