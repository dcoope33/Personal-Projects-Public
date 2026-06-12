#include "background.hpp"

enum MoveDir {UP = -1, DOWN = 1, LEFT = -1, RIGHT = 1};

// Handles all SDL events
bool GameWindow::handleEvents(std::vector<Entity*>& entities) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) running = false; // close window
        else if(event.type == SDL_MOUSEBUTTONDOWN) {
            //printf("%d\t%d\n", event.button.x, event.button.y);
        } 
    }

    Player *player = (Player*)entities[1]; // player is the second index
    
    // get the keyboard state
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    bool isMoving = false;
    bool new_room = false;



     // Sneaking check (you can't move while sneaking)
    if(currentKeyStates[SDL_SCANCODE_LSHIFT]) {
        player->crouch();

    } else {
        player->un_crouch();

        // check all the inputs and react accordingly
        if(currentKeyStates[SDL_SCANCODE_LCTRL]){
            player->speedUp();
        } else  {
            player->normalSpeed();
        }
        if(currentKeyStates[SDL_SCANCODE_W]) {
            player->move_y(UP);
            player->setState(WALK_UP);
            isMoving = true;
        }
        if(currentKeyStates[SDL_SCANCODE_S]) {
            player->move_y(DOWN);
            player->setState(WALK_DOWN);
            isMoving = true;
        }
        if(currentKeyStates[SDL_SCANCODE_A]) {
            player->move_x(LEFT);
            player->setState(WALK_LEFT);
            isMoving = true;
        }
        if(currentKeyStates[SDL_SCANCODE_D]) {
            player->move_x(RIGHT);
            player->setState(WALK_RIGHT);
            isMoving = true;
        }
        if(currentKeyStates[SDL_SCANCODE_E]) {
            Uint32 currentTime = SDL_GetTicks();
            if(currentTime > timer + 400) {
                new_room = true;
                isMoving = false;

                timer = currentTime;
            }
        }
    }

    if(!isMoving) {
        player->setState(IDLE);
    }
    
    return new_room;
}