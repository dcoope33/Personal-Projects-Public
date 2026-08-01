#pragma once
#include "entity.hpp"

// Player class
class Player : public Entity {
private:
    int initial_x, initial_y;
    int Health = 100;
    int invisLength = 1500;

    int Hitbox_buff_left = 40;
    int Hitbox_buff_right = 60;

    Uint32 invincibleTimer = 0; // Tracks invincibility frames
    CollisionMap *map = nullptr;
    
    SDL_Texture *Heart;
    SDL_Texture *Normal;
    SDL_Texture *Damaged;

    float speed = 2.0f;
    
    bool crouched = false;
    bool alive = true;
    bool attacking = false;
    bool started = false;

public:
    Player(SDL_Texture *tex, SDL_Texture *heart, SDL_Texture *BW, float x, float y, int w, int h) 
        : Entity(tex, x, y, w, h) {
        initial_x = x;
        initial_y = y;
        Heart = heart;
        srcRect.w = 40;
        srcRect.h = 48;
        Normal = texture;
        Damaged = BW;
        invincibleTimer = SDL_GetTicks();
    }

    int Attacking() { 
        if(attacking) return currentRow;
        
        return false;
    }

    void action(std::pair<int, int> coords) override {
        if(attacking) {
            
            frameSpeed = 100;

            if(currentFrame == 1) started = true;

            if(currentFrame == 0 && started) {
                
                currentRow = currentDir;
                started = false;
                attacking = false;

            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if(currentTime > invincibleTimer + invisLength) texture = Normal;

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
        // I want some leeway in the hitbox so calculate that
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
            y = y + 20; // move down a bit so your head is lower
            crouched = true;
            currentRow = CROUCHING;
            // decrease height so that your feet stay in the same place after moving down
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


    void attack() {
        attacking = true;

        Start_Animation();
        
        switch(currentDir) {
            case WALK_DOWN: currentRow = ATTACK_DOWN; break;
            case WALK_RIGHT: currentRow = ATTACK_RIGHT; break;
            case WALK_UP: currentRow = ATTACK_UP; break;
            case WALK_LEFT: currentRow = ATTACK_LEFT; break;
        }

    } 


    void takeDamage(int amount) { 
        // invincibility frames with timer
        Uint32 currentTime = SDL_GetTicks();
        if(currentTime > invincibleTimer + invisLength && amount) {
            Health -= amount;
            invincibleTimer = currentTime; 
            texture = Damaged;
        }  
    }


    bool displayHealth(GameWindow *game, int level, int room) {
        
        enum Rooms {LEVEL_4 = 4, BossRoom = 2};
        static bool first = true; 

        Uint32 currentTime = SDL_GetTicks();

        if(Health == 0) {
            static Uint32 stateTimer;

            if(alive) {
                stateTimer = SDL_GetTicks();
                alive = false;
            }

            if(currentTime > stateTimer + 200) return true;
        }

        for(int i = 0; i < Health; i += 20) {

            // display each heart
            SDL_Rect srcRect = {0, 0, 225, 225};
            SDL_Rect destRect = {i+i/4, 0, 20, 20};
            SDL_RenderCopy(game->getRenderer(), Heart, &srcRect, &destRect);
            
            // on first time this func is called make a slow health loading in effect
            if(first) { 
                usleep(800000); 
                game->display(); 
                first = false;
            }

        }
        return false;
    }

};
