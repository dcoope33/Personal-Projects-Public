#pragma once
#include "entity.hpp"

// Player class
class Player : public Entity {
private:
    int initial_x, initial_y;
    int Health = 100;
    float speed = 2.0f;
    CollisionMap *map = nullptr;
    SDL_Texture *Heart;
    Uint32 invincibleTimer = 0; // Tracks invincibility frames
    int invisLength = 2000;
    bool crouched = false;

public:
    Player(SDL_Texture *tex, SDL_Texture *heart, float x, float y, int w, int h) 
        : Entity(tex, x, y, w, h) {
        initial_x = x;
        initial_y = y;
        Heart = heart;
        invincibleTimer = SDL_GetTicks();
    }

    // Method to change the action (IDLE, WALKING, ECT)
    void setState(int row) { if(!crouched) currentRow = row; }

    void setMap(CollisionMap *new_map) { this->map = new_map; }

    void speedUp() { speed = 4.0f; frameSpeed = 100; }
    void normalSpeed() { speed = 2.0f; }

    // Methods for moving the sprite
    void move_x(int dir) { 
        // Look where player moves
        float step = speed * dir; 
        int x_nextl = x + w/4 + step;
        int x_nextr = x + (3 * w/4) + step;
        int y_bottom = y + h-12;

        //printf("%d %d\n", (int)x, (int)y);
        // Move ONLY if it is NOT solid
        if(!map->isSolid(x_nextl, y_bottom) && !map->isSolid(x_nextr, y_bottom)) { 
            this->x += step; 
        }
    }

    void move_y(int dir) { 
        float step = speed * dir;
        int x_l = x + w/4;
        int x_r = x + (3 * w/4);        
        int y_next = y + h-12 + step;

        //printf("%d %d\n", (int)x, (int)y);
        if(!map->isSolid(x_l, y_next) && !map->isSolid(x_r, y_next)) { 
            this->y += step;
        }
    }

    void crouch() { 
        if(!crouched) {
            y = y + 20; 
            crouched = true;
            currentRow = CROUCHING;
            h = 44;
            w = 44;
        } 
    }

    void un_crouch() { 
        if(crouched) {
            y = y - 20;
            crouched = false; 
            realRow = WALK_RIGHT;
            h = 64;
            w = 64;
        }
    }

    bool iscrouched() { return crouched; }

    void attack() {} 

    void takeDamage(int amount) { 
        Uint32 currentTime = SDL_GetTicks();
        if(currentTime > invincibleTimer + invisLength && amount) {
            Health -= amount;
            invincibleTimer = currentTime; 
        }
    }

    bool displayHealth(GameWindow *game, int level, int room) {
        enum Rooms {LEVEL_4 = 4, BossRoom = 2};
        static int first = 0;
        if(Health == 0) return true;
        for(int i = 0; i < Health; i += 20) {
            SDL_Rect srcRect = {0, 0, 225, 225};
            SDL_Rect destRect = {i+i/4, 0, 20, 20};
            SDL_RenderCopy(game->getRenderer(), Heart, &srcRect, &destRect);
            if(!first) { usleep(800000); game->display(); }

        }
        first++;
        return false;
    }

};
