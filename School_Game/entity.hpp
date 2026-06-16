#pragma once
#include "classes.hpp"

#define SPRITE_W 33
#define SPRITE_H 49

enum AnimState {WALK_DOWN, WALK_RIGHT, WALK_UP, WALK_LEFT, CROUCHING, JUMPATTACK, IDLE, CHASING_X, CHASING_Y, SWOOPATTACK = 4};

class Entity {
protected:
    float x, y;
    int w, h;
    int currentFrame;
    int currentRow;
    int realRow;
    int frameSpeed = 400;
    int frameCount = 4;
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    Uint32 frameTimer = 0; // Tracks frame duration

public:
    Entity(SDL_Texture *tex, float x, float y, int w, int h) 
        : texture(tex), x(x), y(y), w(w), h(h) {
        srcRect = { 0, 0, SPRITE_W, SPRITE_H };
        destRect = { (int)x, (int)y, w, h };
        currentFrame = 0;
        currentRow = IDLE;
        realRow = WALK_DOWN;
        frameTimer = SDL_GetTicks();
    }

    // Virtual destructor: essential for cleaning up derived classes
    virtual ~Entity() {}

    // Every entity needs to be updated and drawn
    virtual void update() {
        Uint32 currentTime = SDL_GetTicks();

        // Cycle through frames (X-Offset)
        // Assuming 4 frames per animation, SPRITE_W pixels wide each
        if(currentRow == IDLE) frameCount = 1;
        
        if(currentRow < 6) realRow = currentRow;
        
        if(currentTime > frameTimer + frameSpeed) {
            currentFrame = ++currentFrame % frameCount;
            frameTimer = currentTime;
        }

        // Calculate the location of the sprite in the sheet
        srcRect.y = realRow * srcRect.h;
        srcRect.x = currentFrame * srcRect.w;

        // Update the position
        destRect.x = (int)x;
        destRect.y = (int)y;
        destRect.h = h;
        //destRect.w = w;
        frameSpeed = 400;
        frameCount = 4;
    }

    void render(SDL_Renderer *renderer) {
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    }

    virtual int collCheck(std::pair<int, int> coords, bool crouched) {
        int p_x = coords.first;
        int p_y = coords.second;

        int p_w = 64;
        int p_h = crouched ? 44 : 64;

        // Check if there is NO overlap. If any of these are true, they are NOT colliding.
        if (p_x + p_w <= x ||  // Player is entirely to the left of the obstacle
            p_x >= x + w ||  // Player is entirely to the right of the obstacle
            p_y + p_h <= y ||  // Player is entirely above the obstacle
            p_y >= y + h)    // Player is entirely below the obstacle
        {
            return 0; 
        }

        return 20; 
    }

    virtual void action(std::pair<int, int> coords) { }

    virtual bool look(CollisionMap *active_map, std::pair<int, int> coords, bool crouched) { return false; }

    std::pair<int, int> getCoords() { return {x, y}; }   

    void setPosition(float x_pos, float y_pos) { x = x_pos; y = y_pos; }

    virtual void resetState() { }

};


class Teacher : public Entity {
private:
    int initial_x, initial_y; // important for reseting 
    int walk_time = 4000;
    int idle_time = 3000;   
    bool done_walking = false;
    Uint32 stateTimer = 0; // Tracks state duration
    int prev_walking_dir = WALK_RIGHT;

public:
    Teacher(SDL_Texture *tex, float x, float y, int w, int h) 
        : Entity(tex, x, y, w, h) {
        initial_x = x; 
        initial_y = y; 
        srcRect.w = 96;
        srcRect.h = 217;
        currentRow = WALK_LEFT; // Start left
        stateTimer = SDL_GetTicks();
    }

    void action(std::pair<int, int> coords) override {
        Uint32 currentTime = SDL_GetTicks();

        switch(currentRow) {
            case IDLE:
                // Stay idle for idle_time and switch based off prev_dir
                if(currentTime > stateTimer + idle_time) {
                    switch(prev_walking_dir) {
                        case WALK_DOWN: 
                            currentRow = WALK_UP;
                            idle_time = 0;
                            break;
                        case WALK_UP:
                            currentRow = WALK_RIGHT;
                            idle_time = 3000;
                            break;
                        case WALK_LEFT:
                            currentRow = done_walking ? WALK_DOWN : WALK_LEFT;
                            
                            walk_time = 2000;
                            idle_time = done_walking ? 3000 : 0;

                            done_walking = !done_walking;
                            break;
                        case WALK_RIGHT:
                            // set current walkind direction
                            currentRow = done_walking ? WALK_LEFT : WALK_RIGHT;
                            
                            // set idle and walk times
                            walk_time = 4000;
                            idle_time = 3000;
                            
                            // toggle doen walkfing
                            done_walking = !done_walking;
                            break;
                    }
                    stateTimer = currentTime;
                }
                break;

            case WALK_LEFT:
                x -= 1.0f;
                // Walk left for alloted time
                if(currentTime > stateTimer + walk_time) {
                    realRow =  done_walking ? WALK_LEFT : WALK_UP;
                    prev_walking_dir = WALK_LEFT;
                    currentRow = IDLE;
                    stateTimer = currentTime;
                }
                break;

            case WALK_RIGHT:
                x += 1.0f;
                if(currentTime > stateTimer + walk_time) {
                    realRow =  done_walking ? WALK_DOWN : WALK_UP;
                    prev_walking_dir = WALK_RIGHT;
                    currentRow = IDLE;
                    stateTimer = currentTime;
                }
                break;

            case WALK_DOWN:
                y += .8f;
                if(currentTime > stateTimer + 300) {
                    prev_walking_dir = WALK_DOWN;
                    currentRow = IDLE;
                    stateTimer = currentTime;
                }
                break;

            case WALK_UP:
                y -= .8f;
                if(currentTime > stateTimer + 300) {
                    prev_walking_dir = WALK_UP;
                    currentRow = IDLE;
                    stateTimer = currentTime;
                }
                break;
        }
    }

    void resetState() override { 
        prev_walking_dir = WALK_RIGHT;
        currentRow = IDLE;
        this->setPosition(initial_x, initial_y);
        stateTimer = SDL_GetTicks(); 
    }
};


class Officer : public Entity {
private:
    int initial_x, initial_y;
    int time;
    int currentNum = 1;
    Uint32 stateTimer = 0; // Tracks how long entity has been in a state
    std::vector<Uint32> path;

public:
    Officer(SDL_Texture *tex, float x, float y, std::vector<Uint32> paths, int w, int h) 
        : Entity(tex, x, y, w, h) {
        initial_x = x;
        initial_y = y;
        srcRect.w = 110;
        srcRect.h = 202;
        
        path = paths;
        currentRow = path[0]; // Initial state
        realRow = path[1];
        time = path[2];

        stateTimer = SDL_GetTicks();
    }

    void action(std::pair<int, int> coords) override { 
        Uint32 currentTime = SDL_GetTicks();

        // speeds for running esentially
        // static is good because speeds are samme for all guards so you only need one copy
        static const float dir_x[4] = {0.0f, 5.0f, 0.0f, -5.0f};
        static const float dir_y[4] = {2.0f, 0.0f, -2.0f, 0.0f};

        if(currentRow == CHASING_X || currentRow == CHASING_Y) {
            // chaseeeee
            frameSpeed = 100;
            x += dir_x[realRow];
            y += dir_y[realRow];

        } else {

            // actually move 
            if(currentRow == WALK_LEFT) x -= 1.0f;
            else if(currentRow == WALK_RIGHT) x += 1.0f;
            else if(currentRow == WALK_UP) y -= .8f; 
            else if(currentRow == WALK_DOWN) y += .8f; 

            if(currentTime > stateTimer + time) {
                    if(currentNum*3 >= path.size()) currentNum = 0;
                    
                    currentRow = path[currentNum*3];
                    realRow = path[currentNum*3 + 1];
                    time = path[currentNum*3 + 2];
                    
                    stateTimer = currentTime;
                    currentNum++;
                }
        }
    }

    bool look(CollisionMap *active_map, std::pair<int, int> coords, bool crouched) override { 
        bool seen = false;
        int p_x = coords.first;
        int p_y = coords.second;

        int p_h = 64;
        int p_w = 64;
        if(crouched) {
            p_h = 44;
            p_w = 44;
        }

        // these are for a future cone like vision model (not in use yet)
        int x_dist = abs(p_x - x); 
        int y_dist = abs(p_y - y);
        switch(realRow) {
            case WALK_LEFT:
                if((p_y < y + h - 20) && (p_y + p_h > y + 20) && (p_x <= x)) {
                    bool hidden = false;
                    for(int currentX = x; currentX > p_x; currentX -= 32) {
        
                        // If vision sees a wall or desk before reaching the player then player is hidden
                        if(active_map->isSolid(currentX, y+h/2)) {
                            hidden = true; 
                            break;
                        }
                    }

                    if(!hidden) currentRow = CHASING_X;
                }
                break;
            case WALK_RIGHT:
                if((p_y < y + h - 20) && (p_y + p_h > y + 20) && (p_x >= x)) {
                    bool hidden = false;
                    for(int currentX = x + w; currentX < p_x; currentX += 32) {
        
                        if(active_map->isSolid(currentX, y+h/2)) {
                            hidden = true; 
                            break;
                        }
                    }
                    
                    if(!hidden) currentRow = CHASING_X;
                }
                break;
            case WALK_DOWN:
                if((p_x < x + w - 20) && (p_x + w > x + 20) && (p_y >= y)) {
                    bool hidden = false;
                    for(int currentY = y + h; currentY < p_y; currentY += 32) {
        
                        if(active_map->isSolid(x+SPRITE_W/3, currentY)) {
                            hidden = true; 
                            break;
                        }
                    }

                    if(!hidden) currentRow = CHASING_Y;
                }
                break;
            case WALK_UP:
                if((p_x < x + w - 20) && (p_x + w > x + 20) && (p_y <= y)) {
                    bool hidden = false;
                    for(int currentY = y; currentY > p_y; currentY -= 32) {
        
                        if(active_map->isSolid(x+w/2, currentY)) {
                            hidden = true; 
                            break;
                        }

                    }

                    if(!hidden) currentRow = CHASING_Y;
                }
                break;
        }
        
        // Check if the player and officer hitboxes actually overlap
        if (currentRow == CHASING_X || currentRow == CHASING_Y) {
            bool x_overlap = (p_x < x + w - 10) && (p_x + w > x);
            bool y_overlap = (p_y < y + h - 20) && (p_y + h > y + 20);
            
            if(x_overlap && !y_overlap) {
                currentRow = CHASING_Y;
                realRow = (p_y > y) ? WALK_DOWN : WALK_UP;
            } else if(y_overlap && !x_overlap) {
                currentRow = CHASING_X;
                realRow = (p_x > x) ? WALK_RIGHT : WALK_LEFT; 
            }

            if (x_overlap && y_overlap) {
                seen = true; // Busted!
            }
        }

        return seen; 
    }

    void resetState() override { 
        currentRow = path[0]; // Initial state
        realRow = path[1];
        time = path[2];
        currentNum = 1;
        this->setPosition(initial_x, initial_y);
        stateTimer = SDL_GetTicks(); 
    }

};
