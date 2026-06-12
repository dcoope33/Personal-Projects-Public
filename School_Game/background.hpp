#pragma once
#include "player.hpp"

// Background class
class Background : public Entity {
private:
    int room;

public:
    // Pass the texture and screen dimensions to the parent constructor
    Background(SDL_Texture *tex, int screenW, int screenH, int myroom) 
        : Entity(tex, 0, 0, screenW, screenH) {

        // room
        room = myroom;
        
        // If the classroom image is a different size than the window
        // adjust the srcRect to read the whole file.
        int imgW, imgH;
        SDL_QueryTexture(tex, NULL, NULL, &imgW, &imgH);
        srcRect = { 0, 0, imgW, imgH };
    }

    // override update to do nothing saving performance.
    void update() override { }   

};