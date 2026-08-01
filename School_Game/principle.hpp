#include "officerPaths.hpp"

enum MoveDir {UP, DOWN, LEFT, RIGHT, STILL};
enum Attacks {JUMP, SWOOP, STAB, GRAB, NONE};
enum Moves {Moving, Attacking};

class Principle : public Entity {
private:
    int time = 1000;   

    int currentDir = LEFT;
    int prev_walking_dir = WALK_RIGHT;
    int currentState = Moving;
    int attackType = NONE;

    int Hitbox_buff_left = 40;
    int Hitbox_buff_right = 60;

    int Health = 200;

    bool started = false;
    bool jumpSet = false;
    bool hit = false;

    SDL_Texture *Normal;
    SDL_Texture *Damaged;

    Uint32 stateTimer = 0; // Tracks state duration
    CollisionMap *map = nullptr;

public:
    Principle(SDL_Texture *tex, SDL_Texture *DM, float x, float y, int w, int h) 
        : Entity(tex, x, y, w, h) {
        srcRect.w = 267;
        srcRect.h = 352;
        currentRow = WALK_UP; 
        Normal = tex;
        Damaged = DM;
        stateTimer = SDL_GetTicks();
    }

    void setMap(CollisionMap *new_map) { this->map = new_map; }


    void move_x(int dir) { 
        float step = 1.0f * dir; 
        int x_nextl = x + w/4 + step;
        int x_nextr = x + (3 * w/4) + step;
        int y_bottom = y + h-12;

        if(!map->isSolid(x_nextl, y_bottom) && !map->isSolid(x_nextr, y_bottom)) { 
            this->x += step; 
            if(dir == 1) currentRow = WALK_RIGHT;
            else currentRow = WALK_LEFT;

        } else currentDir = ++currentDir % 4;
    }

    
    void move_y(int dir) { 
        float step = 1.0f * dir;
        int x_l = x + w/4;
        int x_r = x + (3 * w/4);        
        int y_next = y + h-12 + step;

        if(!map->isSolid(x_l, y_next) && !map->isSolid(x_r, y_next)) { 
            this->y += step;
            if(dir == 1) currentRow = WALK_DOWN;
            else currentRow = WALK_UP;

        } else currentDir = ++currentDir % 4;
    }


    void jumpAttack(std::pair<int, int> coords) { 
        if(currentFrame == 2 && !jumpSet) { 
            x = coords.first - 40; 
            y = coords.second - 40; 
            jumpSet = true;
        } 

        if(currentFrame == 3) {
            Hitbox_buff_left = 0;
            Hitbox_buff_right = 0;
        }

        frameSpeed = 800;
        currentRow = JUMPATTACK; 
    }


    void swoopAttack() {
        currentRow = SWOOPATTACK_R;
        Hitbox_buff_right = 0;
    }


    void stabAttack() {
        currentRow = SWOOPATTACK_L;
        Hitbox_buff_right = 0;
    }


    void grabAttack(std::pair<int, int> coords) {
        if(currentFrame == 2 && !jumpSet) { 
            x = coords.first - 40; 
            y = coords.second - 40; 
            jumpSet = true;
        } 
        frameSpeed = 800;
        currentRow = JUMPATTACK; 
    }


    int pickDirection(int dist_x, int dist_y, bool standStill) {
        
        enum axis {X, Y};
        bool axis = (abs(dist_x) >= abs(dist_y)) ? X : Y;

        if(standStill) {
            if(axis && dist_y > 0) currentRow = DOWN;
            else if(axis) currentRow = UP;

            if(!axis && dist_x > 0) currentRow = RIGHT;
            else if(!axis) currentRow = LEFT; 

            time = 1000 + rand() % 500;
            frameCount = 1;

            return STILL;

        } else {

            frameCount = 4;

            if(axis && dist_y > 0) return DOWN;
            else if(axis) return UP;

            if(!axis && dist_x > 0) return RIGHT;
            else if(!axis) return LEFT; 

            return rand() % 4;

        }

    }


    int pickAttack(int dist_x, int dist_y) {
        
        if(dist_x > 300 || dist_y > 300) return JUMP;
        
        if(dist_x <= 100 && dist_y <= 100) return rand() % 3 + 1;

        return NONE;
    }

    void action(std::pair<int, int> coords) override {
        Uint32 currentTime = SDL_GetTicks();

        // calculate the distance from player
        int dist_x = coords.first - x; // if negative player is to Left
        int dist_y = coords.second - y; // if negative player is Up

        switch(currentState) {
            case Moving:

                if(currentDir != STILL) currentDir = pickDirection(dist_x, dist_y, 0);

                switch(currentDir) {
                    case UP: move_y(-1); break;
                    case DOWN: move_y(1); break;
                    case LEFT: move_x(-1); break;
                    case RIGHT: move_x(1); break;
                    default: frameCount = 1;
                }

                // after time is up boss attacks
                if(currentTime > stateTimer + time) {
                    currentState = Attacking;

                    attackType = pickAttack(abs(dist_x), abs(dist_y));
                    
                    Start_Animation();
                }
            break;

            case Attacking:
                if(currentFrame == 1) started = true;

                switch(attackType) {
                    case JUMP: jumpAttack(coords); break;
                    case SWOOP: swoopAttack(); break;
                    case STAB: stabAttack(); break;
                    case GRAB: grabAttack(coords); break;
                }
                
                if(attackType == NONE || (currentFrame == 0 && started)) {
                    started = false;
                    jumpSet = false;
                    
                    Hitbox_buff_left = 40;
                    Hitbox_buff_right = 60;

                    currentDir = pickDirection(dist_x, dist_y, rand() % 2);
                    time = rand() % 500 + 500; // random num from 
                    
                    currentState = Moving;
                    stateTimer = currentTime;
                } 
            break;
        }
    }

    int collCheck(std::pair<int, int> coords, bool crouched) override {
        int p_x = coords.first;
        int p_y = coords.second;

        p_x += 15;
        p_y += crouched ? 44-20 : 64-20;

        // player heights set and adjusted if necissary
        int p_w = 64 - 30;
        int p_h = 10;

        // hitbox correction
        int x_left = x + Hitbox_buff_left;
        int x_right = x + w - Hitbox_buff_right;

        // don't check collision when he's in the air on jump attack
        if((attackType == 0 || attackType == 3) && currentFrame == 2) return 0;

        // Check if there is NO overlap. If any of these are true, they are NOT colliding.
        if (p_x + p_w <= x_left ||  // Player is entirely to the left of the obstacle
            p_x >= x_right ||  // Player is entirely to the right of the obstacle
            p_y + p_h <= y ||  // Player is entirely above the obstacle
            p_y >= y + h)    // Player is entirely below the obstacle
        {
            return 0; 
        }

        return 20;
    }

    bool takeDamage(bool reset, std::pair<int, int> coords, int AttackDir) override {
        // player is not attacking
        if(reset) {
            hit = false;
            texture = Normal;
            return false;
        }

        // adjust player hitbox depending on attack direction
        switch(AttackDir) {
            case ATTACK_DOWN: coords.second += 30; break;
            case ATTACK_LEFT: coords.first -= 30; break;
            case ATTACK_UP: coords.second -= 30; break;
            case ATTACK_RIGHT: coords.first += 30; break;
        }

        // if this attack has already registered or the player is not contacting
        // the principle then just return false
        if(hit) return false;
        
        else if(!collCheck(coords, false)) {
            texture = Normal;
            return false;
        }

        // take damage
        Health -= 20;
        hit = true;
        texture = Damaged;
        //printf("%d\n", Health);
        
        // dead
        if(Health == 0) return true;

        // not dead
        return false;
    }

};